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
/**
 * @brief Classe de gestion d'un client TCP.
 *
 * Cette classe est abstraite. Elle contient les outils nécessaire à une
 * connexion TCP. Pour s'en servir, il faut définir les méthodes
 * readUntil() et readExactly().
 */
class BasicClient
{
        /* ====================  Data members  ==================== */
    private:
        /**
         * @brief Si l'option gzip est activée.
         */
        bool _is_gzip;
        /**
         * @brief Si l'on veut écrire directement dans un flux de sortie.
         */
        bool _write_in_ostream;
        bool _end_of_socket;

        /**
         * @brief La taille des dernières données lues dans la socket.
         */
        size_t _size_to_read; 
        /**
         * @brief La taille des données à lire.
         */
        size_t _read_size;

        /**
         * @brief L'adresse IP ou l'url du serveur à joindre
         */
        std::string _addr;
        /**
         * @brief Le port de l'application à joindre.
         *
         * Si le port n'est pas définit, alors le protocole sera utilisé à sa
         * place.
         */
        std::string _port;
        /**
         * @brief Le protocole à joindre.
         *
         * Le protocole n'est utilisé que si le port n'est pas défini.
         */
        std::string _protocole;

        /**
         * @brief Le buffer qui sert à lire les données dans la socket.
         *
         * Lorsque l'on appelle la méthode readUntil() ou readSome(), les
         * données lues sont enregistrées dans ce buffer.
         */
        Boost::Streambuf _message;
        /**
         * @brief Un listener du traitement d'une connexion.
         *
         * Ce n'est pas encore implémenter, mais l'idée était de pouvoir
         * offrir un moyen à l'utilisateur de suivre l'évolution de l'échange
         * entre le client et le serveur.
         */
        SocketListener *_socket_listener;
        /**
         * @brief Le flux de sortie dans lequel écrire les données. 
         *
         * Si l'on veut directement écrire dans un fichier, il faut définir
         * cet attribut et mettre _write_in_ostream à vrai.
         */
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
        /**
         * @brief Surcharge de l'opérateur <<.
         *
         * Cette opérateur fait appel à readSome() puis écrit dans le flux de
         * sortie. Ça permet d'écrire directement dans un fichier sans
         * convertir en chaine de caractères.
         *
         * @param out Le flux de sortie.
         * @param client Le client TCP.
         *
         * @return Renvoie out après modification.
         */
        friend std::ostream &operator<<(std::ostream &out, BasicClient &client);


        /* ====================  Methods       ==================== */
        /**
         * @brief Écrire dans la socket.
         *
         * @param msg Le message à écrire.
         * @param size La taille du message.
         */
        virtual void write(const char *msg, size_t size) = 0;
        /**
         * @brief Lancer la connexion.
         */
        virtual void connect() = 0;
        /**
         * @brief Fermer la connexion.
         */
        virtual void close() = 0;

        /**
         * @brief Permet de définir le flux de sortie _out.
         *
         * @param write Si vrai, l'on va écrire dans le flux de sortie out.
         * @param out Le flux de sortie dans lequel l'on veut écrire.
         */
        void writeInOstream(bool write, std::ostream &out);
        /**
         * @brief Permet d'écrire dans la socket.
         *
         * @param msg
         */
        void write(const std::string &msg);
        /**
         * @brief Ecrire directement dans le fichier filename
         *
         * @param filename Le nom du fichier.
         */
        void saveInFile(const std::string &filename);//TODO.
        /**
         * @brief Lire une taille fixe dans la socket.
         *
         * Cette fonction lit _size_to_read bytes. Il faut donc définir la taille
         * à lire grace setSizeToRead(size_t size).
         */
        void readSome();
        /**
         * @brief Lire jusqu'à un fanion.
         *
         * @param fanion Le fanion.
         */
        void readUntil(const std::string &fanion);//Lire en incluant le fanion.

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
