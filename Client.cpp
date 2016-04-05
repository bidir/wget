/*
 * ================================== CPP ======================================
 * Filename: Client.cpp
 *
 * Description: none 
 *
 * Version0.0
 * Created: 02/21/16 00:14:28
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
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

#include "Client.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: HttpClient
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */ 

/* ====================  Constructors  ==================== */
Client::Client():
    Client("", "")
{}

Client::Client(string addr, string port):
    Client(addr.c_str(), port.c_str())
{}

Client::Client(const char * addr, const char *port):
    _write_in_ostream(false),
    _end_of_socket(false),
    _size_to_read(20),
    _read_size(0),
    _addr(addr),
    _port(port),
    _socket(NULL),
    _socket_listener(NULL),
    _out(NULL)
{}

Client::~Client()
{
    delete _socket;
}



/* ====================  Accessors     ==================== */
bool Client::isSocketEndReached()
{
    return _end_of_socket;
}

size_t Client::getSizeToRead()
{
    return _size_to_read;
}

size_t Client::getReadSize()
{
    return _read_size;
}

size_t Client::getBufSize()
{
    return _message.size();
}

Streambuf &Client::getMessage()
{
    return _message;
}


/* Cette fonction transforme le buffer _message en string. Si
 * l'utilisateur a utilisé readUntil, qui lit dans la soquet
 * jusqu'à trouver le fanion, mais peut le dépasser, alors il
 * transformer en string que la quantité de bytes qu'il veut. */
string Client::getString(int size)
{
    if(size < 0)//si size < 0, transformer la totalité du buffer en string
    {
        istream istr(&getMessage());
        string str = "", line;
        while(getline(istr, line))
        {
            str = str + line + "\n";
        }
        return str;
    }
    else //sinon lire et ransformer en string que size bytes
    {
        char *buff = new char[size + 1];
        istream istr(&_message);
        istr.read(buff, size);
        buff[size] = '\0';
        return string(buff);
    }

}

string Client::getAddress()
{
    return _addr;
}

string Client::getPort()
{
    return _port;
}

bool Client::getWriteInOstream()
{
    return _write_in_ostream;
}

ostream &Client::getOstream()
{
    return *_out;
}


/* ====================  Mutators      ==================== */
void Client::setSizeToRead(size_t msg_len)
{
    _size_to_read = msg_len;
}

void Client::setAddress(string addr)
{
    _addr = addr;
}

void Client::setPort(string port)
{
    _port = port;
}

void Client::setSocketListener(SocketListener &socketListener)
{
    _socket_listener = &socketListener;
}

void Client::writeInOstream(bool read, ostream &out)
{
    _write_in_ostream = read;
    _out = &out;
}



/* ====================  Operators     ==================== */
ostream &operator<<(ostream &out, Client &client)
{
    client.readSome();
    out.write(buffer_cast<const char *>(client.getMessage().data()), client.getBufSize());

    return out;
}



/* ====================  Methods       ==================== */
void Client::connect()
{
    _end_of_socket = false;

    _socket = new Socket(_service);
    Resolver resolver(_service);
    Query query(_addr, _port);

    Error error;

    Iterator endpoint_iterator = resolver.resolve(query);
    Iterator end;

    while(endpoint_iterator != end)
    {
        try
        {
            Endpoint endpoint = *endpoint_iterator;
            _socket->connect(endpoint, error);
            if(!error)
            {
                ostringstream oss;
                oss << "Tentative de connexion a " << endpoint << " reussie." << endl;
                setPort(boost::lexical_cast<string> (endpoint.port()));
                Log::i(oss.str());
                break;
            }

            ostringstream oss2;
            oss2 << "Tentative de connexion a " << endpoint << " echouee." << endl;
            Log::i(oss2.str());
            _socket->close();
            endpoint_iterator++;
        }
        catch(const exception &e)
        {
            throw ExInvalidURL(e.what(), __FILE__, __LINE__, __FUNCTION__);
        }
    }

    if(endpoint_iterator == end)
    {
        SystemError se(error);
        throw ExInvalidURL(se.what(), __FILE__, __LINE__, __FUNCTION__);
    }
}

void Client::write(const char *msg, int size)
{
    boost::asio::write(*_socket, buffer(msg, size));
}

void Client::write(const string &msg)
{
    write(msg.c_str(), msg.size());
}

void Client::saveInFile(const string &filename)
{
    ofstream file;
    file.open(filename);
    writeInOstream(true, file);
}

void Client::readSome()
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
    Error error;
    _read_size = read(*_socket, _message, transfer_exactly(_size_to_read - _message.size()), error);

    if(error)
    {
        //cerr << "Probleme de lecture sur la socket: " << error.message() << endl;
        SystemError es(error);
        throw ExReadSocket(es.what(), __FILE__, __LINE__, __FUNCTION__);
    }
}

/* Lire jusqu'au fanion, comme read_until fait appel à read_some jusqu'à
 * trouver la fanion, elle va donc lire généralement au delà de ce dernier */
void Client::readUntil(const string &fanion)
{
    Error error;
    _read_size = read_until(*_socket, _message, fanion, error);
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
        SystemError es(error);
        throw ExReadSocket(es.what(), __FILE__, __LINE__, __FUNCTION__);
    }
}

void Client::close()
{
    if(_socket->is_open())
    {
        ErrorCode ec;
        _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if(ec)
        {
            SystemError es(ec);
            throw ExCloseConnection(es.what(), __FILE__, __LINE__, __FUNCTION__);
        }

        _socket->close(ec);

        if(ec)
        {
            SystemError es(ec);
            throw ExCloseConnection(es.what(), __FILE__, __LINE__, __FUNCTION__);
        }
    }
}
/* -----************************  end of class  ************************----- \\
         Client
// -----****************************************************************----- */

