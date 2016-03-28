#ifndef __H_CLIENT_H__
#define __H_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: Client.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> Client
 *      |-> SocketListener
 *
 * Version: 1.0
 * Created: 02/21/16 00:14:20
 * Revision: none
 *
 * Commants: 
 *      |-> mercredi 24 février 2016, 19:29:34 (UTC+0100)
 *        |-> Ajouter les traitements asynchrones de la socket.
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <boost/asio.hpp>
#include <string>
#include <iostream>

#include "Log.hpp"
#include "ExClient.hpp"


//Rédifinition de certains types et utilisation de certains espaces de noms.
using namespace std;
using namespace boost::asio;
using ip::tcp;
typedef tcp::resolver Resolver;
typedef tcp::resolver::query Query;
typedef tcp::resolver::iterator Iterator;
typedef tcp::socket Socket;
typedef tcp::endpoint Endpoint;
typedef boost::asio::io_service IoService;
typedef boost::asio::streambuf Streambuf;
typedef boost::system::error_code Error;
typedef boost::system::system_error SystemError;
typedef boost::system::error_code ErrorCode;

class SocketListener;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: HttpClient
    Description: Permet de définir un client tcp. Elle contient les outils
    nécessaire à la création d'une socket, l'écriture et lecture dans cette
    socket.
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */ 
class Client
{
        /* ====================  Data members  ==================== */
    private:
        bool _write_in_ostream;
        bool _end_of_socket;    //Vrai si la socket est entièrement lue.

        size_t _size_to_read;   //Nombre de bytes à lire.
        size_t _read_size;      //Nombre de bytes lus.

        string _addr;           //Adresse ip ou url du serveur.
        string _port;           //Le port ou le service à utiliser.

        Streambuf _message;     //Buffer pour la lecture de la socket.
        IoService _service;
        Socket *_socket;        //La socket qui servira pour les échanges.
        SocketListener *_socket_listener; //Serira au traitement asynchrone.
        ostream *_out;


    public:
        /* ====================  Constructors  ==================== */
        Client();
        Client(string addr, string port);
        Client(const char *addr, const char *port);
        virtual ~Client();


        /* ====================  Accessors     ==================== */
        bool getWriteInOstream();
        bool isSocketEndReached();
        size_t getSizeToRead();
        size_t getReadSize();
        size_t getBufSize();
        Streambuf &getMessage();
        string getString(int size = -1);
        string getAddress();
        string getPort();
        void writeInOstream(bool read, ostream &out);
        ostream &getOstream();


        /* ====================  Mutators      ==================== */
        void setSizeToRead(size_t msg_len);
        void setAddress(string addr);
        void setPort(string port);
        void setSocketListener(SocketListener &socketListener);


        /* ====================  Operators     ==================== */
        friend ostream &operator<<(ostream &out, Client &client);


        /* ====================  Methods       ==================== */
        void connect();
        void write(const string &msg);//Permet d'écrire dans la socket
        void write(const char *msg, int size);
        void saveInFile(const string &filename);//TODO.
        void readSome();    //lire _size_to_read bytes dans la socket.
        void readUntil(const string &fanion);//Lire en incluant le fanion.
        size_t find(const string &fanion);
        void close();   //Fermer la connexion et la requête
};
/* -----************************  end of class  ************************----- \\
         Client
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
        virtual bool onReceive(const Client &client) = 0;
        virtual bool onError(const Client &client) = 0;
        virtual bool onConnect(const Client &client, ErrorCode e) = 0;
        virtual bool on(const Client &client) = 0;


    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
                   SocketListener
// -----****************************************************************----- */


#endif
