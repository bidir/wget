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
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <map>
#include "Client.hpp"



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
class HttpClient : public Client
{
        /* ====================  Data members  ==================== */
    public:
        static string reg_prot;
        static string reg_addr;
        static string reg_port;
        static string reg_path;
        static string reg_file;
        static string reg_quer;
        static string reg_queries;
    private:
        bool _chunked;

        unsigned int _status;
        int _content_length;

        string _protocole;
        string _path;
        string _filename;
        string _content_type;
        string _status_msg;
        string _http_version;
        string _connection;
        string _accept_ranges;
        string _header;
        string _unparsed_header;
        string _data;
        string _location;
        string _encoding;

        map<string, string> _queries;


    public:
        /* ====================  Constructors  ==================== */
        HttpClient();
        HttpClient(string addr);
        HttpClient(string addr, string port);


        /* ====================  Accessors     ==================== */
        bool isChunked();
        unsigned int getStatus();
        int getContentLength();
        string getProtocole();
        string getPath();
        string getFilename();
        string getContentType();
        string getStatusMessage();
        string getHttpVersion();
        string getConnection();
        string getAcceptRanges();
        string getHeader();
        string getUnparsedHeader();
        string getData();
        string getLocation();
        string getEncoding();

        map<string, string> getQueries();
        string getQuery(string name);


        /* ====================  Mutators      ==================== */
        void setPath(const string &path);
        void setFilename(const string &filename);
        void setProtocole(const string &protocole);


        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        void get(); //Envoyer une requête GET.
        /* Cette fonction permet de décomposer une URL en "nom serveur" et
         * "chemin" vers le fichier à récupérer.*/
        static string getServerFromURL(string url);
        static vector<string> parseURL(string url, bool strict = false);
        void url(string url, bool strict = false);
        virtual void parse();   //Permet de parser la réponse.
        void parseHeader(); //Permet de parser l'en-tête de la réponse.
        void recuperateData(); //Permet de parser l'en-tête de la réponse.


    protected:
        /* ====================  Methods       ==================== */
        void parseChunkedData(); //Permet de parser une réponse chunked.
        void parsePath();
        void parseQuery(const string &queries);
        int hexToDec(const string &line);//Conversion Hexadécimal => Décimal.
};
/* -----************************  end of class  ************************----- \\
        HttpClient
// -----****************************************************************----- */


#endif
