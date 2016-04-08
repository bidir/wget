/*
 * ================================== CPP ======================================
 * Filename: HttpClient.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 02/22/16 15:27:16
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <regex>
#include "tools.hpp"
#include "HttpClient.hpp"
#include "ExHttpClient.hpp"


using namespace std;

/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: HttpClient
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */

/* ====================  Data members  ==================== */
//regex e("(/.*)(/([^/]+\\.[^/]+)?)$");
string HttpClient::reg_prot = "((\\w+)://)";
string HttpClient::reg_addr = "([0-9a-zA-Z\\._-]+)";
string HttpClient::reg_port = "(:([0-9]+))";
string HttpClient::reg_path = "(/[^?]*)";
string HttpClient::reg_file = "([^/?]+\\.[^/?]+)";
string HttpClient::reg_quer = "(\\?(.*))";
string HttpClient::reg_queries = "(([^&;]+)=([^&;]+)[&;]?)";


/* ====================  Constructors  ==================== */
HttpClient::HttpClient():
    HttpClient(new Client("", "80"))
{}

HttpClient::HttpClient(string addresse):
    HttpClient(new Client(addresse, "80"))
{}


HttpClient::HttpClient(BasicClient *client):
    _client(client),
    _chunked(false),
    _status(-1),
    _content_length(-1),
    _protocole(""),
    _path(""),
    _filename(""),
    _content_type(""),
    _status_msg(""),
    _http_version(""),
    _connection(""),
    _accept_ranges(""),
    _header(""),
    _unparsed_header(""),
    _data(""),
    _location(""),
    _encoding(""),
    _queries()
{}

HttpClient::HttpClient(string addresse, string port):
    HttpClient(new Client(addresse, port))
{}

HttpClient::~HttpClient()
{}



/* ====================  Accessors     ==================== */
bool HttpClient::isChunked()
{
    return _chunked;
}

unsigned int HttpClient::getStatus()
{
    return _status;
}

int HttpClient::getContentLength()
{
    return _content_length;
}

string HttpClient::getProtocole()
{
    return _protocole;
}

string HttpClient::getPath()
{
    return _path;
}

string HttpClient::getFilename()
{
    return _filename;
}

map<string, string> HttpClient::getQueries()
{
    return _queries;
}

string HttpClient::getQuery(string name)
{
    return _queries[name];
}

string HttpClient::getContentType()
{
    return _content_type;
}

string HttpClient::getStatusMessage()
{
    return _status_msg;
}

string HttpClient::getHttpVersion()
{
    return _http_version;
}

string HttpClient::getConnection()
{
    return _connection;
}

string HttpClient::getAcceptRanges()
{
    return _accept_ranges;
}

string HttpClient::getHeader()
{
    return _header;
}

string HttpClient::getUnparsedHeader()
{
    return _unparsed_header;
}

string HttpClient::getData()
{
    return _data;
}

string HttpClient::getLocation()
{
    return _location;
}

string HttpClient::getEncoding()
{
    return _encoding;
}



/* ====================  Mutators      ==================== */
void HttpClient::setPath(const string &path)
{
    _path = path;
}

void HttpClient::setFilename(const string &filename)
{
    _filename = filename;
}

void HttpClient::setProtocole(const string &protocole)
{
    _protocole = protocole;
}



/* ====================  Methods       ==================== */
void HttpClient::connect()
{
    getTCPClient()->connect();
}

void HttpClient::close()
{
    getTCPClient()->close();
}

void HttpClient::get()
{
    get(_path + _filename);
}

void HttpClient::get(const string &file)
{
    string get =    "GET " + file + " HTTP/1.1\r\n";
    if(getTCPClient()->getPort() == "80")
    {
        get = get + "Host: " + getTCPClient()->getAddress() + "\r\n";
    }
    else
    {
        get = get + "Host: " + getTCPClient()->getAddress() + ":" + getTCPClient()->getPort() + "\r\n";
    }
    get = get + "Accept: */*\r\n";
    get = get + "Accept-Encoding: *\r\n";
    get = get + "Connection: close\r\n\r\n";
    Log::i(get);
    getTCPClient()->write(get);
}

string HttpClient::getServerFromURL(string url)
{
    return HttpClient::parseURL(url)[1];
}

vector<string> HttpClient::parseURL(string url, bool strict)
{
    vector<string> data;
    string reg = "^" + reg_prot;
    if(!strict)
    {
        reg = reg + "?";
    }

    reg = reg +
          reg_addr +
          reg_port + "?" +
          reg_path + "?" +
          reg_quer + "?$";
    regex e(reg);
    smatch m;

    if(regex_search(url, m, e))
    {
        string protocole(m.str(2)),
               addr(m.str(3)),
               port(m.str(5)),
               path(m.str(6)),
               queries(m.str(8));
        data.push_back(protocole);
        data.push_back(addr);
        data.push_back(port);
        data.push_back(path);
        data.push_back(queries);
        return data;
    }
    else
    {
        throw GenEx(ExInvalidURL, "L'url \"" + url + "\" est invalide.");
    }

}

void HttpClient::url(string url, bool strict)
{
    vector<string> data = parseURL(url, strict);

    string protocole(data[0]);
    string server(data[1]);

    setProtocole(data[0]);
    getTCPClient()->setAddress(data[1]);
    getTCPClient()->setPort(data[2]);
    setPath(data[3]);
    parsePath();
    parseQuery(data[4]);

    if(getProtocole() != "http" && getProtocole() != "https")
    {
        throw GenEx(ExInvalidProtocole, _protocole);
    }


    if(getProtocole() == "")
    {
        setProtocole("http");
    }
    if(getProtocole() == "https")
    {
        _client = new SSLClient(server, protocole);
    }
    if(getTCPClient()->getPort() == "")
    {
        getTCPClient()->setPort("80");
    }

}

void HttpClient::parsePath()
{
    regex e("^" + reg_path + "?/" + reg_file + "$");
    smatch m;
    if(regex_search(getPath(), m, e))
    {
        string path(m.str(1));
        string filename(m.str(2));
        setPath(path);
        setFilename(filename);
    }

    if(getPath() == "" || getPath().substr(getPath().size() - 1) != "/")
    {
        setPath(getPath() + "/");
    }
}

void HttpClient::parseQuery(const string &queries)
{
    string s = queries;
    regex e(reg_queries);
    smatch m;

    while(regex_search(s, m, e))
    {
        string varname(m.str(2));
        string varval(m.str(3));
        _queries[varname] = varval;
        s = m.suffix().str();
    }
}

void HttpClient::parseHeader()
{
    getTCPClient()->readUntil("\r\n");
    istream is(&getTCPClient()->getMessage());
    string line;
    is >> _http_version;
    is >> _status;
    tools::getline(is, _status_msg);

    _header = "";
    _unparsed_header = "";

    size_t pos;
    string str = "";

    while(1)
    {
        istream in(&getTCPClient()->getMessage());
        while(tools::getline(in, line))
        {
            if(line == "\r")
            {
                return;
            }
            str = "CONTENT-TYPE: ";
            if((pos = tools::toUpper(line).find(str)) != string::npos)
            {
                size_t pos2;
                string str2 = "; CHARSET=";
                if((pos2 = tools::toUpper(line).find(str2)) != string::npos)
                {
                    _content_type = line.substr(pos+str.size(), pos2-pos-str.size());
                    _encoding = line.substr(pos2 + str2.size());
                }
                else
                {
                    _content_type = line.substr(pos + str.size());
                }
                _header = _header + line + "\n";
                continue;
            }

            str = "CONTENT-LENGTH: ";
            if((pos = tools::toUpper(line).find(str)) != string::npos)
            {
                istringstream(line.substr(pos + str.size())) >> _content_length;
                _header = _header + line + "\n";
                continue;
            }

            str = "CONNECTION: ";
            if((pos = tools::toUpper(line).find(str)) != string::npos)
            {
                _connection = line.substr(pos + str.size());
                _header = _header + line + "\n";
                continue;
            }

            str = "ACCEPT-RANGES: ";
            if((pos = tools::toUpper(line).find(str)) != string::npos)
            {
                _accept_ranges = line.substr(pos + str.size());
                _header = _header + line + "\n";
                continue;
            }

            str = "TRANSFER-ENCODING: ";
            if(tools::toUpper(line).find(str) != string::npos && tools::toUpper(line).find("CHUNKED") != string::npos)
            {
                _chunked = true;
                _header = _header + line + "\n";
                continue;
            }
            str = "LOCATION: ";
            if((pos = tools::toUpper(line).find(str)) != string::npos)
            {
                _location = line.substr(pos + str.size());
                _header = _header + line + "\n";
                continue;
            }

            _unparsed_header = _unparsed_header + line + "\n";
        }
        if(!getTCPClient()->isSocketEndReached())
        {
            getTCPClient()->readUntil("\r\n");
        }
        else
        {
            return;
        }
    }
}

int HttpClient::hexToDec(const string &line)
{
    return strtol(line.c_str(), NULL, 16);
}

void HttpClient::parseChunkedData()
{
    _data = "";
    string line;

    int length;
    getTCPClient()->readUntil("\r\n");
    istream in_first(&getTCPClient()->getMessage());
    if(tools::getline(in_first, line))
    {
        length = hexToDec(line);
        getTCPClient()->setSizeToRead(length);
        getTCPClient()->readSome();
        if(getTCPClient()->getWriteInOstream())
        {
            getTCPClient()->getOstream() << *getTCPClient();
            return;
        }
        else
        {
            _data = _data + getTCPClient()->getString(length);
        }
    }

    do
    {
        length = 0;
        getTCPClient()->readUntil("\r\n");
        istream in1(&getTCPClient()->getMessage());
        if(tools::getline(in1, line) && line == "\r")
        {
            getTCPClient()->readUntil("\r\n");
            istream in2(&getTCPClient()->getMessage());
            tools::getline(in2, line);
        }
        length = hexToDec(line);

        if(length != 0)
        {
            getTCPClient()->setSizeToRead(length);
            getTCPClient()->readSome();
            if(getTCPClient()->getWriteInOstream())
            {
                getTCPClient()->getOstream() << *getTCPClient();
                return;
            }
            else
            {
                _data = _data + getTCPClient()->getString(length);
            }
        }
    }while(length != 0);
}

void HttpClient::parse()
{
    parseHeader();
    recuperateData();
}

void HttpClient::recuperateData()
{
    _data = "";
    if(isChunked())
    {
        parseChunkedData();    
    }
    else
    {
        getTCPClient()->setSizeToRead(_content_length);

        if(getTCPClient()->getWriteInOstream())
        {
            getTCPClient()->getOstream() << *getTCPClient();
            return;
        }
        getTCPClient()->readSome();
        _data = _data + getTCPClient()->getString();
    }
}

void HttpClient::printInfos()
{
    ostringstream oss;
    oss << "----------- Infos recuperees dans l'en-tete ------------ " << endl;
    if(isChunked())
    {
        oss << "chunked = true" << endl;
    }
    else
    {
        oss << "chunked = false" << endl;
    }
    oss << "statut = " << getStatus() << endl;
    oss << "message du statut = " << getStatusMessage() << endl;
    oss << "Taille des donnees = " << getContentLength() << endl;
    oss << "Type des donnees = " << getContentType() << endl;
    oss << "Version http = " << getHttpVersion() << endl;
    oss << "Unite donnees = " << getAcceptRanges() << endl;
    oss << "Location = " << getLocation() << endl;
    oss << "Encoding = " << getEncoding() << endl;
    if(getUnparsedHeader() != "")
    {
        oss << "----------------- Le reste de l'en-tete ---------------- ";
        oss << endl << getUnparsedHeader() << endl;
    }
    oss << "-------------------------------------------------------- " << endl;
    Log::i(oss.str());
}

BasicClient *HttpClient::getTCPClient()
{
    return _client;
}
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

/* ====================  Constructors  ==================== */
HttpsClient::HttpsClient():
    HttpsClient(new SSLClient("", "80"))
{}

HttpsClient::HttpsClient(string addresse):
    HttpsClient(new SSLClient(addresse, "80"))
{}

HttpsClient::HttpsClient(BasicClient *client):
    HttpClient(client)
{}

HttpsClient::HttpsClient(string addresse, string port):
    HttpsClient(new SSLClient(addresse, port))
{}

HttpsClient::~HttpsClient()
{}

