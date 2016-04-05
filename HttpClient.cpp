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
regex e("(/.*)(/([^/]+\\.[^/]+)?)$");
string HttpClient::reg_prot = "((\\w+)://)";
string HttpClient::reg_addr = "([0-9a-zA-Z\\._-]+)";
string HttpClient::reg_port = "(:([0-9]+))";
string HttpClient::reg_path = "(/[^?]*)";
string HttpClient::reg_file = "([^/?]+\\.[^/?]+)";
string HttpClient::reg_quer = "(\\?(.*))";
string HttpClient::reg_queries = "(([^&;]+)=([^&;]+)[&;]?)";


/* ====================  Constructors  ==================== */
HttpClient::HttpClient():
    HttpClient("", "80")
{}

HttpClient::HttpClient(string addresse):
    HttpClient(addresse, "80")
{}

HttpClient::HttpClient(string addresse, string port):
    Client(addresse, port),
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
void HttpClient::get()
{
    string get =    "GET " + _path + _filename + " HTTP/1.1\r\n";
    if(getPort() == "80")
    {
        get = get + "Host: " + getAddress() + "\r\n";
    }
    else
    {
        get += get + "Host: " + getAddress() + ":" + getPort() + "\r\n";
    }
    get = get + "Accept: */*\r\n" +
    "Connection: close\r\n\r\n";
    Log::i("get = " + get);
    write(get);
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
        throw ExInvalidURL
            (
                 "L'url \"" + url + "\" est invalide.",
                 __FILE__,
                 __LINE__,
                 __FUNCTION__
            );
    }

}

void HttpClient::url(string url, bool strict)
{
    vector<string> data = parseURL(url, strict);
    setProtocole(data[0]);
    setAddress(data[1]);
    setPort(data[2]);
    setPath(data[3]);
    parsePath();
    parseQuery(data[4]);
    if(getPort() == "")
    {
        setPort("80");
    }
    if(getProtocole() == "")
    {
        setProtocole("http");
    }

    if(getProtocole() != "http")
    {
        throw ExInvalidProtocole
            (
             _protocole,
             __FILE__,
             __LINE__,
             __FUNCTION__
            );
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
    readUntil("\r\n");
    istream is(&getMessage());
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
        istream in(&getMessage());
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
            if(tools::toUpper(line).find(str) != string::npos && tools::toUpper(line).find("chunked") != string::npos)
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
        if(!isSocketEndReached())
        {
            readUntil("\r\n");
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
    readUntil("\r\n");
    istream in_first(&getMessage());
    if(tools::getline(in_first, line))
    {
        length = hexToDec(line);
        setSizeToRead(length);
        readSome();
        if(getWriteInOstream())
        {
            getOstream() << *this;
            return;
        }
        else
        {
            _data = _data + getString(length);
        }
    }

    do
    {
        length = 0;
        readUntil("\r\n");
        istream in1(&getMessage());
        if(tools::getline(in1, line) && line == "\r")
        {
            readUntil("\r\n");
            istream in2(&getMessage());
            tools::getline(in2, line);
        }
        length = hexToDec(line);

        if(length != 0)
        {
            setSizeToRead(length);
            readSome();
            if(getWriteInOstream())
            {
                getOstream() << *this;
                return;
            }
            else
            {
                _data = _data + getString(length);
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
        setSizeToRead(_content_length);

        if(getWriteInOstream())
        {
            getOstream() << *this;
            return;
        }
        readSome();
        _data = _data + getString();
    }
}

/* -----************************  end of class  ************************----- \\
          HttpClient
// -----****************************************************************----- */
