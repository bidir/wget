/*
 * ================================== CPP ======================================
 * Filename: HttpDownloader.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:03:42 AM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <iomanip>
#include <chrono>
#include <fstream>
#include <regex>

#include "tools.hpp"
#include "HttpDownloader.hpp"
#include "HTMLTagParser.hpp"
#include "ExHttpClient.hpp"


using namespace std;
using namespace std::chrono;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HttpDownloader
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */


/* ====================  Constructors  ==================== */
HttpDownloader::HttpDownloader():
    HttpDownloader("./", 3)
{}

HttpDownloader::HttpDownloader(string path):
    HttpDownloader(path, 3)
{}

HttpDownloader::HttpDownloader(int depth):
    HttpDownloader("./", depth)
{}

HttpDownloader::HttpDownloader(string path, int depth):
    _only_page(false),
    _print(false),
    _nb_files(0),
    _print_refresh(100),
    _depth(depth),
    _nb_d_th(4),
    _nb_p_th(4),
    _path(path),
    _m_print(),
    _th_print(NULL),
    _client(),
    _tags(0),
    _d_threads(0),
    _p_threads(0),
    _tag_to_attr()
{}

HttpDownloader::~HttpDownloader()
{
    delete _th_print;
}


/* ====================  Accessors     ==================== */
bool HttpDownloader::getOnlyPage()
{
    return _only_page;
}

bool HttpDownloader::getPrint()
{
    return _print;
}

unsigned int HttpDownloader::getNbDownladedFiles()
{
    return _nb_files;
}

unsigned int HttpDownloader::getPrintRefresh()
{
    return _print_refresh;
}

unsigned int HttpDownloader::getDepth()
{
    return _depth;
}

unsigned int HttpDownloader::getNbDownloadThreads()
{
    return _nb_d_th;
}

unsigned int HttpDownloader::getNbParseThreads()
{
    return _nb_p_th;
}

string HttpDownloader::getPath()
{
    return _path;
}

HttpDownloaderQueue &HttpDownloader::getQueue()
{
    return _queue;
}


/* ====================  Mutators      ==================== */
void HttpDownloader::setOnlyPage(bool only)
{
    _only_page = only;
}

void HttpDownloader::setPrint(bool print)
{
    _print = print;
}

void HttpDownloader::setPrintRefresh(unsigned int milliseconds)
{
    _print_refresh = milliseconds;
}

void HttpDownloader::setDepth(unsigned int depth)
{
    _depth = depth;
}

void HttpDownloader::setNbDownloadThreads(unsigned int nb)
{
    _nb_d_th = nb;
}

void HttpDownloader::setNbParseThreads(unsigned int nb)
{
    _nb_p_th = nb;
}

void HttpDownloader::setPath(string path)
{
    _path = path;
    if(_path[_path.size() - 1] != '/')
    {
        _path = _path + "/";
    }
}


/* ====================  Methods       ==================== */
void HttpDownloader::addTag(const string &t, const string &a)
{
    string tag(tools::toUpper(t)), attr(tools::toUpper(a));
    for(unsigned int i = 0; i < _tags.size(); i++)
    {
        if(_tags[i] == tag)
        {
            return;
        }
    }
    _tags.push_back(tag);
    _tag_to_attr[tag] = attr;
}

void HttpDownloader::download(string url)
{
    addTag("a", "href");
    try
    {
        _client.url(url);
    }
    catch(Exception &ex)
    {
        AddTrace(ex);
        Log::w(ex);
        return;
    }

    if(_only_page)
    {
        _nb_d_th = 3;
        _nb_p_th = 1;
    }

    try
    {
        for(unsigned int i = 0; i < _nb_p_th; i++)
        {
            thread th(HttpDownloader::sParse, this);
            _p_threads.push_back(move(th));
        }

        for(unsigned int i = 0; i < _nb_d_th; i++)
        {
            thread th(HttpDownloader::sGet, this);
            _d_threads.push_back(move(th));
        }
        _queue.addURL(url);
    }
    catch(Exception &e)
    {
        AddTrace(e);
        throw e;
    }
}

void HttpDownloader::printInfos()
{
    cout << endl;
    _print = true;
    delete _th_print;
    _th_print = new thread(HttpDownloader::sPrintInfos, this);
}

void HttpDownloader::wait()
{
    for(unsigned int i = 0; i < _d_threads.size(); i++)
    {
        if(_d_threads[i].joinable())
        {
            _d_threads[i].join();
        }
    }

    for(unsigned int i = 0; i < _p_threads.size(); i++)
    {
        if(_p_threads[i].joinable())
        {
            _p_threads[i].join();
        }
    }
    if(_th_print)
    {
        if(_th_print->joinable())
        {
            _th_print->join();
        }
    }
}

void HttpDownloader::sPrintInfos(HttpDownloader *d)
{
    d->lockPrint();
    do
    {
        cout << "\r"
             << "T:"
                    << setw(4) << setfill('0') << right
                    << d->getQueue().getDIndex() 
                 << "/" << setw(4) << setfill('0') << right
                    << d->getQueue().getDCount()
             << "  A:"
                    << setw(4) << setfill('0') << right
                    << d->getQueue().getPIndex()
                 << "/"
                    << setw(4) << setfill('0') << right
                    << d->getQueue().getPCount()
             << "  TH-T:"
                    << setw(2) << setfill('0') << right
                    << d->getQueue().getNbRunningDThreads() 
                 << "/"
                    << setw(2) << setfill('0') << right
                    << d->getNbDownloadThreads()
             << "  TH-A:"
                    << setw(2) << setfill('0') << right
                    << d->getQueue().getNbRunningPThreads()
                  << "/"
                    << setw(2) << setfill('0') << right
                    << d->getNbParseThreads()
             << "  PT:"
                    << setw(3) << setfill('0') << right
                    << d->getQueue().getLastDDepth()
             << "  PA:"
                    << setw(3) << setfill('0') << right
                    << d->getQueue().getLastPDepth();
        this_thread::sleep_for(milliseconds(d->getPrintRefresh()));
        cout.flush();
    }while(d->getPrint() && !d->getQueue().isStopped());
    d->unlockPrint();
}

void HttpDownloader::sGet(HttpDownloader *httpDownloader)
{
    do
    {
        try
        {
            httpDownloader->get();
        }
        catch(Exception &e)
        {
            AddTrace(e);
            httpDownloader->getQueue().notifyEnd();
            HttpDownloader::sPrintInfos(httpDownloader);
            throw(e);
        }
    }while(!httpDownloader->getQueue().isEnd() && !httpDownloader->getQueue().isStopped());
    httpDownloader->getQueue().notifyEnd();
    HttpDownloader::sPrintInfos(httpDownloader);
}

void HttpDownloader::sParse(HttpDownloader *httpDownloader)
{
    do
    {
        try
        {
            httpDownloader->parse();
        }
        catch(Exception &e)
        {
            AddTrace(e);
            httpDownloader->getQueue().notifyEnd();
            HttpDownloader::sPrintInfos(httpDownloader);
            throw(e);
        }
    }while(!httpDownloader->getQueue().isEnd() && ! httpDownloader->getQueue().isStopped());
    httpDownloader->getQueue().notifyEnd();
    HttpDownloader::sPrintInfos(httpDownloader);
}

void HttpDownloader::get()
{
    string url = _queue.getURL();
    if(url == "")
    {
        return;
    }

    unsigned int depth = _queue.getDepth();
    try
    {
        LogI("Recuperation de la page " + string(url));
        HttpClient client;
        client.url(string(url));
        client.connect();
        client.get();
        client.getTCPClient()->setSizeToRead(1000);
        client.parseHeader();
        client.printInfos();

        if(
              client.getStatus() != 200 &&
              client.getStatus() != 301 &&
              client.getStatus() != 302
          )
        {
            if(_only_page || _nb_files == 0)
            {
                throw GenEx(ExHttpClient, client.getStatus());
            }
            else
            {
                Log::w(GenEx(ExHttpClient, client.getStatus()));
                return;
            }
        }

        if(client.getStatus() == 301 || client.getStatus() == 302)
        {
            LogI("Redirection vers " + client.getLocation());
            _queue.addURL(client.getLocation(), depth);
            return;
        }
        else
        {
            string path = getPath();
            string filename = getPath();

            if(!_only_page)
            {
                path = path + client.getPath();
            }
            tools::createDir(path);
            if(client.getFilename() == "")
            {
                filename = path + "index.html";
            }
            else
            {
                filename = path + client.getFilename();
            }

            regex("//", regex_constants::icase);
            filename = regex_replace(filename, regex("//"), "/");
            ofstream file;
            file.open(filename, ostream::trunc);
            if(file.fail())
            {
                Log::w(GenEx(
                        ExOpeningFile, 
                        "Erreur pendant l'ouverture du fichier " + filename
                    ));
                return;
            }
            LogI("Ecriture des donnees dans " + filename);
            stringstream ss;
            if(tools::toUpper(client.getContentType()) == "TEXT/HTML")
            {
                client.getTCPClient()->writeInOstream(true, ss);
            }
            else
            {
                client.getTCPClient()->writeInOstream(true, file);
            }
            client.recuperateData();
            if(tools::toUpper(client.getContentType()) == "TEXT/HTML")
            {
                string data = ss.str();
                file << data;
                file.flush();
                if(depth <= _depth && !_only_page)
                {
                    _queue.addFile(filename, depth);
                }
            }

            LogI("Fermeture de la connexion au serveur " + _client.getTCPClient()->getAddress());
            client.close(); 
            file.close();
        }
    }
    catch(Exception &e)
    {
        AddTrace(e);
        if(_only_page || _nb_files == 0)
        {
            throw e;
        }
        else
        {
            Log::w(e);
            return;
        }
    }
    catch(const exception &ex)
    {
        if(_only_page || _nb_files == 0)
        {
            throw ex;
        }
        else
        {
            Log::w(ex.what());
            return;
        }
    }
    _nb_files++;
}

void HttpDownloader::parse()
{
    string filename = _queue.getFile();
    unsigned int depth = _queue.getDepth();
    if(filename == "")
    {
        return;
    }

    try
    {
        ifstream in;
        in.open(filename);
        if(in.fail())
        {
            LogW("Erreur pendant l'ouverture du fichier de sortie");
            return;
        }
        HTMLTagParser parser(in);
        for(unsigned int i = 0; i < _tags.size(); i++)
        {
            parser.addTagToParse(_tags[i]);
        }
        parser.parse();
        in.close();

        for(unsigned int i = 0; i < parser.getParsedTags().size(); i++)
        {
            HTMLTag tag = parser.getParsedTag(i);
            string s_url = "";
            string tagname = tag.getName();

            if(tagname == "A" && tag.isAttributeExists("HREF") && depth >= _depth)
            {
                continue;
            }

            for(unsigned int j = 0; j < _tags.size(); j++)

            {
                if(tagname == _tags[j] && tag.isAttributeExists(_tag_to_attr[_tags[j]]))
                {
                    s_url = createURL(tag.getAttribute(_tag_to_attr[_tags[j]]));
                    break;
                }
            }

            if(s_url == "")
            {
                continue;
            }

            try
            {
                if(HttpClient::parseURL(s_url)[1] == _client.getTCPClient()->getAddress())
                {
                    _queue.addURL(s_url, depth + 1);
                }
            }
            catch(ExInvalidURL &e)
            {
                AddTrace(e);
                Log::w(e);
            }
        }
    }
    catch(Exception &e)
    {
        AddTrace(e);
        Log::w(e);
        return;
    }
    catch(exception &ex)
    {
        Log::w(ex.what());
        return;
    }
    replaceURI(filename);
}

void HttpDownloader::lockPrint()
{
    _m_print.lock();
}

void HttpDownloader::unlockPrint()
{
    _m_print.unlock();
}

string HttpDownloader::createURL(const string &path)
{
    string url("");
    try
    {
        HttpClient clt;
        clt.url(path, true);
        url = path;
    }
    catch(ExInvalidURL ex)
    {
        if(path.find("/") != 0 || path.find("/") == string::npos)
        {
            url = _client.getProtocole() + "://" +
                  _client.getTCPClient()->getAddress();
            if(_client.getTCPClient()->getPort() != "")
            {
                url = url + ":" + _client.getTCPClient()->getPort();
            }
            url = url +_client.getPath() + path;
        }
        else
        {
            url = _client.getProtocole() + "://" +
                  _client.getTCPClient()->getAddress();
            if(_client.getTCPClient()->getPort() != "")
            {
                url = url + ":" + _client.getTCPClient()->getPort();
            }
            url = url + path;
        }
    }

    return url;
}

string HttpDownloader::replaceServer(string &data, const string &tag, const string &label)
{
    string reg_bef = "<" + tag  + "\\s+.*" + label + "\\s*=\\s*\"";
    string reg_pro = _client.getProtocole();
    string reg_url = _client.getTCPClient()->getAddress();
    string reg_por = _client.getTCPClient()->getPort();
    string reg_aft = "(/?[^\"]*\")";

    string reg = "(" + reg_bef + ")" +
                 reg_pro +
                 reg_url +
                 reg_por +
                 "(" + reg_aft + ")";

    regex e(reg, regex_constants::icase);
    data = regex_replace(data, e, "$1$2");
    return data;
}

string HttpDownloader::replaceRoot(string &data, const string &tag, const string &label)
{
    string e_reg = "(<" + tag + "\\s.*" + label + "\\s*=\\s*\")/";
    regex e(e_reg, regex_constants::icase);
    data = regex_replace(data, e, "$1" + getPath());

    string e_reg2 = "(<" + tag + "\\s.*" + label + "\\s*=\\s*\".*)/\"";
    regex e2(e_reg2, regex_constants::icase);
    string x = "/index.html\"";
    data = regex_replace(data, e2, "$1" + x);

    return data;
}

void HttpDownloader::replaceURI(const string &filename)
{
    ifstream i_file;
    i_file.open(filename);
    if(i_file.fail())
    {
        Log::w(GenEx(
                    ExOpeningFile, 
                    "Erreur pendant l'ouverture du fichier " + filename
                    ));
        return;
    }

    string line(""), data("");
    while(tools::getline(i_file, line))
    {
        data = data + line + "\n";
    }

    i_file.close();

    for(unsigned int i = 0; i < _tags.size(); i++)
    {
        data = replaceServer(data, _tags[i], _tag_to_attr[_tags[i]]);
    }

    for(unsigned int i = 0; i < _tags.size(); i++)
    {
        data = replaceRoot(data, _tags[i], _tag_to_attr[_tags[i]]);
    }

    ofstream o_file;
    o_file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if(o_file.fail())
    {
        Log::w(GenEx(
                    ExOpeningFile, 
                    "Erreur pendant l'ouverture du fichier " + filename
                    ));
        return;
    }

    //o_file << data;
    o_file.write(data.c_str(), data.size());
    o_file.close();
}
