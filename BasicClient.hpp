#ifndef __H_BASIC_CLIENT_H__
#define __H_BASIC_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: BasicClient.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 07/04/2016 18:53:40
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */




#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>

#include "Log.hpp"
#include "ExClient.hpp"


//Rédifinition de certains types et utilisation de certains espaces de noms.
namespace Boost
{
    typedef boost::asio::ip::tcp::resolver Resolver;
    typedef boost::asio::ip::tcp::resolver::query Query;
    typedef boost::asio::ip::tcp::resolver::iterator Iterator;
    typedef boost::asio::ip::tcp::socket Socket;
    typedef boost::asio::ip::tcp::endpoint Endpoint;
    typedef boost::asio::io_service IoService;
    typedef boost::asio::streambuf Streambuf;
    typedef boost::asio::ssl::stream<Socket> SSLStreamSocket;
    typedef boost::asio::ssl::context SSLContext;
    typedef boost::system::error_code Error;
    typedef boost::system::system_error SystemError;
    typedef boost::system::error_code ErrorCode;
}


class SocketListener;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: BasicClient
    Description: Permet de définir un client tcp. Elle contient les outils
    nécessaire à la création d'une socket, l'écriture et lecture dans cette
    socket.
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */ 
class BasicClient
{
        /* ====================  Data members  ==================== */
    private:
        bool _is_gzip;
        bool _write_in_ostream;
        bool _end_of_socket;    //Vrai si la socket est entièrement lue.

        size_t _size_to_read;   //Nombre de bytes à lire.
        size_t _read_size;      //Nombre de bytes lus.

        std::string _addr;           //Adresse ip ou url du serveur.
        std::string _port;           //Le port ou le service à utiliser.
        std::string _protocole;      //Le port ou le service à utiliser.

        Boost::Streambuf _message;     //Buffer pour la lecture de la socket.
        SocketListener *_socket_listener; //Serira au traitement asynchrone.
        std::ostream *_out;


    public:
        /* ====================  Constructors  ==================== */
        BasicClient();
        BasicClient(std::string addr, std::string port);
        BasicClient(const char *addr, const char *port);
        virtual ~BasicClient();


        /* ====================  Accessors     ==================== */
        bool getGZip();
        bool getWriteInOstream();
        bool isSocketEndReached();
        bool getSSL();
        size_t getSizeToRead();
        size_t getReadSize();
        size_t getBufSize();
        Boost::Streambuf &getMessage();
        std::string getString();
        std::string getAddress();
        std::string getPort();
        std::string getProtocole();
        std::ostream &getOstream();


        /* ====================  Mutators      ==================== */
        void setGZip(bool zip);
        void setSizeToRead(size_t msg_len);
        void setAddress(std::string addr);
        void setPort(std::string port);
        void setProtocole(const std::string &protocole);
        void setSocketListener(SocketListener &socketListener);
    protected:
        void setEndOfSocket(bool val);


        /* ====================  Operators     ==================== */
    public:
        friend std::ostream &operator<<(std::ostream &out, BasicClient &client);


        /* ====================  Methods       ==================== */
        virtual void write(const char *msg, size_t size) = 0;
        virtual void connect() = 0;
        virtual void close() = 0;   //Fermer la connexion et la requête

        void writeInOstream(bool read, std::ostream &out);
        void write(const std::string &msg);//Permet d'écrire dans la socket
        void saveInFile(const std::string &filename);//TODO.
        void readSome();    //lire _size_to_read bytes dans la socket.
        void readUntil(const std::string &fanion);//Lire en incluant le fanion.
        size_t find(const std::string &fanion);

    protected:
        virtual size_t readExactly(Boost::Streambuf &buf, size_t size, Boost::Error &error) = 0;
        virtual size_t readUntil(Boost::Streambuf &buf, const std::string &fanion, Boost::Error &error) = 0;

};
/* -----************************  end of class  ************************----- \\
         BasicClient
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: SocketListener
    Description:    
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class SocketListener
{
    private:
        /* ====================  Data members  ==================== */


    public:
        /* ====================  Constructors  ==================== */
        SocketListener(){};
        virtual ~SocketListener(){};


        /* ====================  Accessors     ==================== */



        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        virtual bool onReceive(const BasicClient *client) = 0;
        virtual bool onError(const BasicClient *client) = 0;
        virtual bool onConnect(const BasicClient *client, Boost::ErrorCode e) = 0;
        virtual bool on(const BasicClient *client) = 0;


    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
                   SocketListener
// -----****************************************************************----- */


#endif
