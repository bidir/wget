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


#include "BasicClient.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: Client
    Description: Permet de définir un client tcp pour une connexion non cryptée.
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOO00000000000000000000000000000000000000000OOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */ 
class Client : public BasicClient
{
        /* ====================  Data members  ==================== */
    private:
        Boost::Socket *_socket;        //La socket qui servira pour les échanges.
        Boost::IoService _service;


    public:
        /* ====================  Constructors  ==================== */
        Client();
        Client(std::string addr, std::string port);
        Client(const char *addr, const char *port);
        virtual ~Client();


        /* ====================  Accessors     ==================== */


        /* ====================  Mutators      ==================== */


        /* ====================  Operators     ==================== */


        /* ====================  Methods       ==================== */
        virtual void write(const char *msg, size_t size);
        virtual void connect();
        virtual void close();   //Fermer la connexion et la requête

    protected:
        virtual size_t readExactly(Boost::Streambuf &buf, size_t size, Boost::Error &error);
        virtual size_t readUntil(Boost::Streambuf &buf, const std::string &fanion, Boost::Error &error);
};
/* -----************************  end of class  ************************----- \\
         Client
// -----****************************************************************----- */


#endif
