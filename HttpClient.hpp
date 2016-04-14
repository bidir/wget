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
 * Commants: Mettre le protocole dans BasicClient
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
        bool _chunked;

        unsigned int _status;
        int _content_length;

        std::string _header;
        std::string _data;
        std::string _path;
        std::string _filename;
        std::string _content_type;
        std::string _status_msg;
        std::string _http_version;
        std::string _connection;
        std::string _accept_ranges;
        std::string _unparsed_header;
        std::string _location;
        std::string _encoding;
        std::string _content_encoding;

        BasicClient* _client;

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
        BasicClient *getTCPClient();
        void connect();
        void close();
        void get(); //Envoyer une requête GET.
        void get(const std::string &file); //Envoyer une requête GET.
        /* Cette fonction permet de décomposer une URL en "nom serveur" et
         * "chemin" vers le fichier à récupérer.*/
        static std::string getServerFromURL(std::string url);
        static std::vector<std::string> parseURL(std::string url, bool strict = false);
        void url(std::string url, bool strict = false);
        virtual void parse();   //Permet de parser la réponse.
        void parseHeader(); //Permet de parser l'en-tête de la réponse.
        void recuperateData(); //Permet de parser l'en-tête de la réponse.
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
