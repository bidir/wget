/*
 * ================================== CPP ======================================
 * Filename: BasicClient.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 07/04/2016 18:53:33
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <sstream>

#include "BasicClient.hpp"
#include "tools.hpp"

using namespace std;
using namespace boost::asio;

/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: BasicClient
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */ 

/* ====================  Constructors  ==================== */
BasicClient::BasicClient():
    BasicClient("", "")
{}

BasicClient::BasicClient(string addr, string port):
    BasicClient(addr.c_str(), port.c_str())
{}

BasicClient::BasicClient(const char * addr, const char *port):
    _is_gzip(false),
    _write_in_ostream(false),
    _end_of_socket(false),
    _size_to_read(20),
    _read_size(0),
    _addr(addr),
    _port(port),
    _protocole(""),
    _socket_listener(NULL),
    _out(NULL)
{}

BasicClient::~BasicClient()
{
}



/* ====================  Accessors     ==================== */
bool BasicClient::getGZip()
{
    return _is_gzip;
}

bool BasicClient::getWriteInOstream()
{
    return _write_in_ostream;
}

bool BasicClient::isSocketEndReached()
{
    return _end_of_socket;
}

size_t BasicClient::getSizeToRead()
{
    return _size_to_read;
}

size_t BasicClient::getReadSize()
{
    return _read_size;
}

size_t BasicClient::getBufSize()
{
    return _message.size();
}

Boost::Streambuf &BasicClient::getMessage()
{
    return _message;
}

/* Cette fonction transforme le buffer _message en string. Si
 * l'utilisateur a utilisé readUntil, qui lit dans la soquet
 * jusqu'à trouver le fanion, mais peut le dépasser, alors il
 * transformer en string que la quantité de bytes qu'il veut. */
string BasicClient::getString(unsigned int size)
{
    char *buff = new char[size + 1];
    istream istr(&_message);
    istr.read(buff, size);
    const char *buff2 = tools::ungzip(buff, size).data();
    delete buff;
    return string(buff2, size) + "\0";
}

string BasicClient::getString()
{
    readSome();
    istream istr(&getMessage());
    string str = "", line;
    while(getline(istr, line))
    {
        str = str + line + "\n";
    }
    if(getGZip())
    {
        vector<char> data = tools::ungzip(str.c_str(), str.size());
        return string(data.data(), data.size());
    }
    else
    {
        return str;
    }
}

string BasicClient::getAddress()
{
    return _addr;
}

string BasicClient::getPort()
{
    return _port;
}

string BasicClient::getProtocole()
{
    return _protocole;
}

ostream &BasicClient::getOstream()
{
    return *_out;
}


/* ====================  Mutators      ==================== */
void BasicClient::setGZip(bool zip)
{
    _is_gzip = zip;
}

void BasicClient::setSizeToRead(size_t msg_len)
{
    _size_to_read = msg_len;
}

void BasicClient::setAddress(string addr)
{
    _addr = addr;
}

void BasicClient::setPort(string port)
{
    _port = port;
}

void BasicClient::setProtocole(const string &protocole)
{
    _protocole = protocole;
}

void BasicClient::setSocketListener(SocketListener &socketListener)
{
    _socket_listener = &socketListener;
}

void BasicClient::setEndOfSocket(bool val)
{
    _end_of_socket = val;
}



/* ====================  Operators     ==================== */
ostream &operator<<(ostream &out, BasicClient &client)
{
    client.readSome();
    out.flush();
    if(client.getGZip())
    {
        vector<char> data = tools::ungzip(buffer_cast<const char *>(client.getMessage().data()), client.getBufSize());
        out.write(data.data(), data.size());
    }
    else
    {
        out.write(buffer_cast<const char *>(client.getMessage().data()), client.getBufSize());
    }
    out.flush();
    return out;
}



/* ====================  Methods       ==================== */
void BasicClient::writeInOstream(bool read, ostream &out)
{
    _write_in_ostream = read;
    _out = &out;
}

void BasicClient::write(const string &msg)
{
    write(msg.c_str(), msg.size());
}

void BasicClient::saveInFile(const string &filename)
{
    ofstream file;
    file.open(filename);
    writeInOstream(true, file);
}

void BasicClient::readSome()
{
    /* Si le nombre de bytes à lire est inférieur ou égal aux nombre
     * de bytes déjà lus et stockés dans le buffer, alors on lit rien.
     * Cette condition permet de s'assurer d'être synchroniser avec le
     * buffer , si l'utilisateur à fait appel à readUntil (qui fait
     * plusieurs appels à read_some et peut donc lire au delà du fanion) */
    if(getSizeToRead() <= _message.size())
    {
        return;
    }
    Boost::Error error;
    _read_size = readExactly(_message, _size_to_read - _message.size(), error);

    if(error)
    {
        Boost::SystemError es(error);
        throw GenEx(ExReadSocket, es.what());
    }
}

/* Lire jusqu'au fanion, comme read_until fait appel à read_some jusqu'à
 * trouver la fanion, elle va donc lire généralement au delà de ce dernier */
void BasicClient::readUntil(const string &fanion)
{
    Boost::Error error;
    _read_size = readUntil(_message, fanion, error);
    if(error == error::eof)
    {
        _read_size = 0;
        _end_of_socket = true;
        return;
    }
    else
    {
        _end_of_socket = false;
        return;
    }

    if(error)
    {
        Boost::SystemError es(error);
        throw GenEx(ExReadSocket, es.what());
    }
}

/* -----************************  end of class  ************************----- \\
         BasicClient
// -----****************************************************************----- */

