#ifndef __H_SSL_CLIENT_H__
#define __H_SSL_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: SSLClient.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 07/04/2016 18:51:30
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */


#include "BasicClient.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: SSLClient
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
/**
 * @brief Définit un client TCP SSL.
 */
class SSLClient : public BasicClient
{
    private:
        /* ====================  Data members  ==================== */
        Boost::SSLStreamSocket *_socket;
        Boost::IoService _service;
        Boost::SSLContext _context;
        char reply_[2048*8];


    public:
        /* ====================  Constructors  ==================== */
        SSLClient();
        SSLClient(std::string addr, std::string port);
        SSLClient(const char *addr, const char *port);
        virtual ~SSLClient();


        /* ====================  Accessors     ==================== */



        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        bool verifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);
        virtual void write(const char *msg, size_t size);
        virtual void connect();
        virtual void close();   //Fermer la connexion et la requête
        void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
        {
            if (!error)
            {
                std::cout << "Reply: ";
                std::cout.write(reply_, bytes_transferred);
                std::cout << "\n";
            }
            else
            {
                std::cout << "Read failed: " << error.message() << "\n";
            }
        }

    protected:
        /* ====================  Methods       ==================== */
        virtual size_t readExactly(Boost::Streambuf &buf, size_t size, Boost::Error &error);
        virtual size_t readUntil(Boost::Streambuf &buf, const std::string &fanion, Boost::Error &error);


};
/* -----************************  end of class  ************************----- \\
         SSLClient
// -----****************************************************************----- */


#endif
