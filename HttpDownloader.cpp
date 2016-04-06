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


#include <fstream>

#include "tools.hpp"
#include "HttpDownloader.hpp"
#include "HTMLTagParser.hpp"
#include "ExHttpClient.hpp"


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
    _depth(depth),
    _nb_d_th(4),
    _nb_p_th(4),
    _path(path),
    _client(),
    _d_threads(0),
    _p_threads(0)
{}


/* ====================  Accessors     ==================== */
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
void HttpDownloader::sGet(HttpDownloader *httpDownloader)
{
    do
    {
        httpDownloader->get();
    }while(!httpDownloader->getQueue().isEnd() && ! httpDownloader->getQueue().isStopped());
    httpDownloader->getQueue().notifyEnd();
}

void HttpDownloader::sParse(HttpDownloader *httpDownloader)
{
    do
    {
        httpDownloader->parse();
    }while(!httpDownloader->getQueue().isEnd() && ! httpDownloader->getQueue().isStopped());
    httpDownloader->getQueue().notifyEnd();
}

void HttpDownloader::get()
{
    //string url = getDURL();
    string url = _queue.getURL();
    if(url == "")
    {
        return;
    }

    unsigned int depth = _queue.getDepth();
    cout << "depth = " << _depth << " -> " << depth << endl;
    try
    {
        Log::i("Recuperation de la page " + string(url));
        HttpClient client;
        client.url(string(url));
        client.connect();
        client.get();
        client.setSizeToRead(1000);
        client.parseHeader();
        tools::printHttpClientInfos(client);

        if(
              client.getStatus() != 200 &&
              client.getStatus() != 301 &&
              client.getStatus() != 302
          )
        {
            ExHttpClient ex
                (
                     client.getStatus(),
                     __FILE__,
                     __LINE__,
                     __FUNCTION__
                );
            Log::w(ex);
            return;
        }

        if(client.getStatus() == 301 || client.getStatus() == 302)
        {
            Log::i("Redirection vers " + client.getLocation());
            _queue.addURL(client.getLocation(), depth);
            return;
        }
        else
        {
            string path(getPath() + client.getPath());
            string filename = path + client.getFilename();
            if(client.getFilename() == "")
            {
                filename = filename + "index.html";
            }

            tools::createDir(path);

            ofstream file;
            file.open(filename);
            if(file.fail())
            {
                Exception ex
                    (
                        ERR_OUTPUT_FILE,
                        "Erreur pendant l'ouverture du fichier de sortie",
                        __FILE__,
                        __LINE__,
                        __FUNCTION__
                    );
                Log::w(ex);
                return;
            }
            client.writeInOstream(true, file);
            client.recuperateData();


            Log::i("Ecriture des donnees dans " + filename);
            file.close();
            Log::i("Fermeture de la connexion au serveur " + string(url));
            client.close(); 

            if(tools::toUpper(client.getContentType()) == "TEXT/HTML" && depth + 1 < _depth)
            {
                _queue.addFile(filename, depth + 1);
            }
        }
    }
    catch(const Exception &e)
    {
        Log::w(e);
        return;
    }
    catch(const exception &ex)
    {
        Log::w(ex.what());
        return;
    }
}

void HttpDownloader::parse()
{
    //string filename = getPFile();
    string filename = _queue.getFile();
    unsigned int depth = _queue.getDepth();
    if(filename == "")
    {
        return;
    }
    cout << "depth = " << _depth << " -> " << depth << endl;

    try
    {
        ifstream in;
        in.open(filename);
        if(in.fail())
        {
            Log::w(tools::getException
                    (
                        "Erreur pendant l'ouverture du fichier de sortie",
                        __FILE__,
                        __LINE__,
                        __FUNCTION__
                    ));
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
            string tagname = tools::toUpper(tag.getName());
            if(tagname == "IMG" && tag.isAttributeExists("src"))
            {
                s_url = createURL(tag.getAttribute("src"));
            }
            else if(tagname == "A" && tag.isAttributeExists("href") && depth <= _depth)
            {
                s_url = createURL(tag.getAttribute("href"));
            }

            else if(tagname == "LINK" && tag.isAttributeExists("href"))
            {
                s_url = createURL(tag.getAttribute("href"));
            }
            else if(tagname == "SCRIPT" && tag.isAttributeExists("src"))
            {
                s_url = createURL(tag.getAttribute("src"));
            }

            if(HttpClient::parseURL(s_url)[1] == _client.getAddress())
            {
                _queue.addURL(s_url, depth + 1);
            }
        }
    }
    catch(const Exception &e)
    {
        Log::w(e);
        return;
    }
    catch(const exception &ex)
    {
        Log::w(ex.what());
        return;
    }
}

void HttpDownloader::download(string url)
{
    try
    {
        _client.url(url);
    }
    catch(const Exception &ex)
    {
        Log::w(ex);
        return;
    }
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

string HttpDownloader::createURL(const string &path)
{
    string url("");
    try
    {
        HttpClient clt;
        clt.url(path, true);
        url = path;
    }
    catch(const ExInvalidURL ex)
    {
        if(path.find("/") != 0 || path.find("/") == string::npos)
        {
            url = _client.getProtocole() + "://" +
                _client.getAddress() +
                ":" + _client.getPort() +
                _client.getPath() + 
                path;
        }
        else
        {
            url = _client.getProtocole() + "://" +
                _client.getAddress() +
                ":" + _client.getPort() +
                path;
        }
    }

    return url;
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
}
