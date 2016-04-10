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


#include <chrono>
#include <fstream>

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
    _nb_pages(0),
    _print_refresh(100),
    _depth(depth),
    _nb_d_th(4),
    _nb_p_th(4),
    _path(path),
    _m_print(),
    _th_print(NULL),
    _client(),
    _d_threads(0),
    _p_threads(0)
{}

HttpDownloader::~HttpDownloader()
{
    if(_th_print)
    {
        delete _th_print;
    }
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

unsigned int HttpDownloader::getNbDownloadedPages()
{
    return _nb_pages;
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
}


/* ====================  Methods       ==================== */
void HttpDownloader::download(string url)
{
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
        _nb_d_th = 1;
        _nb_p_th = 0;
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

void HttpDownloader::sPrintInfos(HttpDownloader *downloader)
{
    downloader->lockPrint();
    do
    {
        cout.flush();
        cout << "\rT: " << downloader->getQueue().getDIndex() 
             << "/" << downloader->getQueue().getURLs().size()
             << "   A: " << downloader->getQueue().getPIndex()
             << "/" << downloader->getQueue().getFiles().size()
             << "   TH-T: " << downloader->getQueue().getNbRunningDThreads() 
             << "/" << downloader->getNbDownloadThreads()
             << "   TH-A: " << downloader->getQueue().getNbRunningPThreads()
             << "/" << downloader->getNbParseThreads();
             //<< "   P: " << downloader->getNbDownloadedPages();
        this_thread::sleep_for(milliseconds(downloader->getPrintRefresh()));
    }while(downloader->getPrint() && !downloader->getQueue().isStopped());
    downloader->unlockPrint();
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
            if(_only_page || _nb_pages == 0)
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
            if(url != client.getLocation())
                _queue.addURL(client.getLocation(), depth);
            return;
        }
        else
        {
            string path("");
            string filename = getPath();

            if(!_only_page)
            {
                path = getPath() + client.getPath();
                tools::createDir(path);
                if(client.getFilename() == "")
                {
                    filename = path + "index.html";
                }
                else
                {
                    filename = path + client.getFilename();
                }
            }

            ofstream file;
            file.open(filename);
            if(file.fail())
            {
                Log::w(GenEx(
                        ExOpeningFile, 
                        "Erreur pendant l'ouverture du fichier " + filename
                    ));
                return;
            }
            LogI("Ecriture des donnees dans " + filename);
            client.getTCPClient()->writeInOstream(true, file);
            client.recuperateData();


            file.close();
            LogI("Fermeture de la connexion au serveur " + _client.getTCPClient()->getAddress());
            client.close(); 

            if(tools::toUpper(client.getContentType()) == "TEXT/HTML" && depth <= _depth && !_only_page)
            {
                _queue.addFile(filename, depth);
            }
        }
    }
    catch(Exception &e)
    {
        AddTrace(e);
        if(_only_page || _nb_pages == 0)
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
        if(_only_page || _nb_pages == 0)
        {
            throw ex;
        }
        else
        {
            Log::w(ex.what());
            return;
        }
    }
    _nb_pages++;
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
        parser.addTagToParse("link");
        parser.addTagToParse("a");
        parser.addTagToParse("img");
        parser.addTagToParse("script");
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
            else if(tagname == "IMG" && tag.isAttributeExists("SRC"))
            {
                s_url = createURL(tag.getAttribute("SRC"));
            }
            else if(tagname == "A" && tag.isAttributeExists("HREF"))
            {
                s_url = createURL(tag.getAttribute("HREF"));
            }

            else if(tagname == "LINK" && tag.isAttributeExists("HREF"))
            {
                s_url = createURL(tag.getAttribute("HREF"));
            }
            else if(tagname == "SCRIPT" && tag.isAttributeExists("SRC"))
            {
                s_url = createURL(tag.getAttribute("SRC"));
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

