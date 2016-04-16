#ifndef __H_HTTP_CLIENT_H__
#define __H_HTTP_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: HttpClient.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> HttpClient
 *
 * Version: 1.0
 * Created: 02/22/16 15:27:27
 * Revision: none
 *
 * Commants: 
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <map>
#include "Client.hpp"
#include "SSLClient.hpp"



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: HttpClient
    Description: Cette classe, qui hérite de Client, permet de définir une
    connexin TCP avec un serveur HTTP.
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
/**
 * @brief Classe qui définit un client HTTP.
 */
class HttpClient
{
        /* ====================  Data members  ==================== */
    public:
        static std::string reg_prot;
        static std::string reg_addr;
        static std::string reg_port;
        static std::string reg_path;
        static std::string reg_file;
        static std::string reg_quer;
        static std::string reg_queries;
    private:
        /**
         * @brief Si les données lues sont chunckées.
         */
        bool _chunked;

        /**
         * @brief Le code de HTTP renvoyé par le serveur.
         */
        unsigned int _status;
        /**
         * @brief La taille des données envoyées par le serveru.
         */
        int _content_length;

        /**
         * @brief L'en-tête HTTP.
         */
        std::string _header;
        /**
         * @brief Les données renvoyées par le serveur.
         */
        std::string _data;
        /**
         * @brief Le chemin dans l'url.
         */
        std::string _path;
        /**
         * @brief Le nom du fichier à demander.
         */
        std::string _filename;
        /**
         * @brief Le type des données.
         */
        std::string _content_type;
        /**
         * @brief Le message d'état HTTP.
         */
        std::string _status_msg;
        /**
         * @brief La version HTTP utilisée par le serveur.
         */
        std::string _http_version;
        std::string _connection;
        std::string _accept_ranges;
        std::string _unparsed_header;
        /**
         * @brief Correspond au champ location dans l'en-tête HTTP.
         */
        std::string _location;
        std::string _encoding;
        std::string _content_encoding;

        /**
         * @brief Le client TCP.
         */
        BasicClient* _client;

        /**
         * @brief Les attributs ajoutés à l'url.
         */
        std::map<std::string, std::string> _queries;


    public:
        /* ====================  Constructors  ==================== */
        HttpClient();
        HttpClient(BasicClient *client);
        HttpClient(std::string addr);
        HttpClient(std::string addr, std::string port);
        virtual ~HttpClient();


        /* ====================  Accessors     ==================== */
        bool isChunked();
        unsigned int getStatus();
        int getContentLength();
        std::string getHeader();
        std::string &data();
        std::string getProtocole();
        std::string getPath();
        std::string getFilename();
        std::string getContentType();
        std::string getStatusMessage();
        std::string getHttpVersion();
        std::string getConnection();
        std::string getAcceptRanges();
        std::string getUnparsedHeader();
        std::string getLocation();
        std::string getEncoding();
        std::string getContentEncoding();

        std::map<std::string, std::string> getQueries();
        std::string getQuery(std::string name);


        /* ====================  Mutators      ==================== */
        void setPath(const std::string &path);
        void setFilename(const std::string &filename);


        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        /**
         * @brief Renvoie le client TCP.
         *
         * @return Le client TCP _client.
         */
        BasicClient *getTCPClient();
        /**
         * @brief Lancer la connexion TCP.
         */
        void connect();
        /**
         * @brief Fermer la connexion TCP.
         */
        void close();
        /**
         * @brief Envoyer une requête HTTP GET.
         */
        void get();
        void get(const std::string &file);
        /* Cette fonction permet de décomposer une URL en "nom serveur" et
         * "chemin" vers le fichier à récupérer.*/
        /**
         * @brief Permet d'analyser une URL et de renvoyer le nom du serveur.
         *
         * Cette fonction fait appel à parseURL().
         *
         * @param url L'url à analyser.
         *
         * @return Le nom du serveur.
         */
        static std::string getServerFromURL(std::string url);
        /**
         * @brief Permet d'analyser une URL.
         *
         * Cette fonction renvoie un tableau avec dans l'ordre: le protocole,
         * le nom du serveur, le port, le chimin, le fichier et les paramètres.
         *
         * @param url L'url à analyser.
         * @param strict Si l'on doit exiger un protocole ou l'ajouter s'il y
         * en a pas.
         *
         * @return Un tableau contenant la décomposition de l'url.
         */
        static std::vector<std::string> parseURL(std::string url, bool strict = false);
        /**
         * @brief Permet de définir l'url à contacter.
         *
         * @param url L'url à contacter.
         * @param strict Si l'on doit exiger un protocole ou l'ajouter si'l y
         * en a pas.
         */
        void url(std::string url, bool strict = false);
        /**
         * @brief Lancer l'analyser de la réponse.
         *
         * Cette fonction appelle simplement ::parseHeader() et
         * ::recuperateData().
         */
        virtual void parse();   //Permet de parser la réponse.
        /**
         * @brief Lancer l'analyse de l'en-tête HTTP.
         */
        void parseHeader(); //Permet de parser l'en-tête de la réponse.
        /**
         * @brief Lancer la récupération des données.
         */
        void recuperateData(); //Permet de parser l'en-tête de la réponse.
        /**
         * @brief Imprimer les information sur la réponse dans les fichiers de
         * log.
         */
        void printInfos();



    protected:
        /* ====================  Methods       ==================== */
        void parseChunkedData(); //Permet de parser une réponse chunked.
        void parsePath();
        void parseQuery(const std::string &queries);
        int hexToDec(const std::string &line);//Conversion Hexadécimal => Décimal.
};
/* -----************************  end of class  ************************----- \\
        HttpClient
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: HttpsClient
    Description: Cette classe, qui hérite de Client, permet de définir une
    connexin TCP avec un serveur HTTPS.
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HttpsClient : public HttpClient
{
    /* ====================  Data members  ==================== */
        Client _s_client;


    public:
        /* ====================  Constructors  ==================== */
        HttpsClient();
        HttpsClient(BasicClient *client);
        HttpsClient(std::string addr);
        HttpsClient(std::string addr, std::string port);
        virtual ~HttpsClient();


        /* ====================  Accessors     ==================== */


        /* ====================  Mutators      ==================== */


        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */


    protected:
        /* ====================  Methods       ==================== */
};
/* -----************************  end of class  ************************----- \\
        HttpClient
// -----****************************************************************----- */
#endif
