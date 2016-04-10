/*
 * ================================== CPP ======================================
 * Filename: SSLClient.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 07/04/2016 18:51:23
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */


#include <openssl/ssl.h>
#include <openssl/err.h>
#include <boost/bind.hpp>

#include "SSLClient.hpp"
#include "tools.hpp"


using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: SSLClient
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */

/* ====================  Constructors  ==================== */
SSLClient::SSLClient():
    SSLClient("", "")
{}

SSLClient::SSLClient(string addr, string port):
    SSLClient(addr.c_str(), port.c_str())
{}

SSLClient::SSLClient(const char *addr, const char *port):
    BasicClient(addr, port),
    _socket(NULL),
    _context(boost::asio::ssl::context::sslv23)
{}

SSLClient::~SSLClient()
{
    delete _socket;
}



/* ====================  Accessors     ==================== */
/* ====================  Mutators      ==================== */
/* ====================  Operators     ==================== */
/* ====================  Methods       ==================== */
void SSLClient::connect()
{
    Boost::Error ec;
    _context.set_default_verify_paths(ec);

    _socket = new Boost::SSLStreamSocket(_service, _context);
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
        try
        {
            Boost::Endpoint endpoint = *endpoint_iterator;
            oss << "Tentative de connexion a " << endpoint << " ..." << endl;;
            LogI(oss.str());
            oss.str("");

            _socket->lowest_layer().connect(endpoint, error);
            _socket->lowest_layer().set_option(tcp::no_delay(true));

            _socket->set_verify_mode(ssl::verify_peer);
            //_socket->set_verify_mode(ssl::verify_none);
            _socket->set_verify_callback(boost::bind(&SSLClient::verifyCertificate, this, _1, _2));
            _socket->set_verify_callback(ssl::rfc2818_verification(getAddress()));
            Boost::Error ec;
            if(!error)
            {
                oss << "Connexion a " << endpoint << " reussie" << endl;
                //setPort(tools::toString(endpoint.port()));
                LogI(oss.str());
                oss.str("");
                _socket->handshake(Boost::SSLStreamSocket::client);
                break;
            }

            oss << "Connexion a " << endpoint << " echouee" << endl;
            LogW(oss.str());
            oss.str("");
            _socket->next_layer().close();
            endpoint_iterator++;
        }
        catch(const exception &e)
        {
            throw GenEx(ExInvalidURL, e.what());
        }
    }

    if(endpoint_iterator == end)
    {
        Boost::SystemError se(error);
        throw ExInvalidURL(se.what(), EXINFO);
    }
}

void SSLClient::write(const char *msg, size_t size)
{
    boost::asio::write(_socket->next_layer(), buffer(msg, size));
}

size_t SSLClient::readExactly(Boost::Streambuf &buf, size_t size, Boost::Error &error)
{
    return read(_socket->next_layer(), buf, transfer_exactly(size), error);
}

size_t SSLClient::readUntil(Boost::Streambuf &buf, const string &fanion, Boost::Error &error)
{
    return read_until(_socket->next_layer(), buf, fanion, error);
}

void SSLClient::close()
{
    if(_socket->next_layer().is_open())
    {
        Boost::ErrorCode ec;
        //_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        _socket->next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if(ec)
        {
            Boost::SystemError es(ec);
            throw GenEx(ExCloseConnection, es.what());
        }

        _socket->next_layer().close(ec);

        if(ec)
        {
            Boost::SystemError es(ec);
            throw GenEx(ExCloseConnection, es.what());
        }
    }
}

bool SSLClient::verifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    LogI("Verifying " + string(subject_name));

    return preverified;
}
/* -----************************  end of class  ************************----- \\
   SSLClient
// -----****************************************************************----- */
