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
#include <unistd.h>
#include <sstream>

#include "Client.hpp"
#include "tools.hpp"

using namespace std;
using namespace boost::asio;

/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: Client
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
    BasicClient(addr, port),
    _socket(NULL)
{}

Client::~Client()
{
    delete _socket;
}



/* ====================  Accessors     ==================== */
/* ====================  Mutators      ==================== */
/* ====================  Operators     ==================== */
/* ====================  Methods       ==================== */
void Client::connect()
{
    try
    {
        _socket = new Boost::Socket(_service);
        Boost::Resolver resolver(_service);
        string port = getPort();
        if(port == "")
        {
            port = getProtocole();
        }
        Boost::Query query(getAddress(), port);

        Boost::Error error;

        Boost::Iterator endpoint_iterator = resolver.resolve(query);
        Boost::Iterator end;

        ostringstream oss;
        while(endpoint_iterator != end)
        {
            Boost::Endpoint endpoint = *endpoint_iterator;
            oss << "Tentative de connexion a " << endpoint << " ..." << endl;;
            LogI(oss.str());
            oss.str("");
            _socket->connect(endpoint, error);
            if(!error)
            {
                oss << "Connexion a " << endpoint << " reussie" << endl;
                //setPort(tools::toString(endpoint.port()));
                LogI(oss.str());
                oss.str("");
                break;
            }

            oss << "Connexion a " << endpoint << " echouee" << endl;
            LogW(oss.str());
            oss.str("");
            _socket->close();
            endpoint_iterator++;
        }

        if(endpoint_iterator == end)
        {
            Boost::SystemError se(error);
            throw GenEx(ExInvalidURL, se.what());
        }
    }
    catch(const exception &e)
    {
        throw GenEx(ExInvalidURL, e.what());
    }
}

void Client::write(const char *msg, size_t size)
{
    boost::asio::write(*_socket, buffer(msg, size));
}

size_t Client::readExactly(Boost::Streambuf &buf, size_t size, Boost::Error &error)
{
    return read(*_socket, buf, transfer_exactly(size), error);
}

size_t Client::readUntil(Boost::Streambuf &buf, const string &fanion, Boost::Error &error)
{
    return read_until(*_socket, buf, fanion, error);
}

void Client::close()
{
    if(_socket->is_open())
    {
        Boost::ErrorCode ec;
        _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if(ec)
        {
            Boost::SystemError es(ec);
            throw GenEx(ExCloseConnection, es.what());
        }

        _socket->close(ec);

        if(ec)
        {
            Boost::SystemError es(ec);
            throw GenEx(ExCloseConnection, es.what());
        }
    }
}
/* -----************************  end of class  ************************----- \\
         Client
// -----****************************************************************----- */

