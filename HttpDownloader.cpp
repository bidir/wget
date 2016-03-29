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
Description:  
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
    _wait(true),
    _depth(depth),
    _nb_d_th(4),
    _nb_p_th(4),
    _path(path),
    _add_url(),
    _d_urls(4),
    _p_urls(4),
    _threads(4)
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

vector<string> &HttpDownloader::getDownloadedURLs()
{
    return _d_urls;
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
void HttpDownloader::start(HttpDownloader *httpDownloader, const string &url)
{
    httpDownloader->download(url);
}

void HttpDownloader::download(string url)
{
    try
    {
        Log::i("Recuperation de la page " + string(url));
        HttpClient client;
        _add_url.lock();
        _d_urls.push_back(url);
        _add_url.unlock();
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
            start(this, client.getLocation());
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
            }
            client.writeInOstream(true, file);
            client.recuperateData();


            Log::i("Ecriture des donnees dans " + filename);
            file.close();
            Log::i("Fermeture de la connexion au serveur " + string(url));
            client.close(); 

            if(tools::toUpper(client.getContentType()) == "TEXT/HTML")
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
                        s_url = createURL(client, tag.getAttribute("src"));
                    }
                    else if(tagname == "A" && tag.isAttributeExists("href"))
                    {
                        s_url = createURL(client, tag.getAttribute("href"));
                    }

                    else if(tagname == "LINK" && tag.isAttributeExists("href"))
                    {
                        s_url = createURL(client, tag.getAttribute("href"));
                    }

                    else if(tagname == "SCRIPT" && tag.isAttributeExists("src"))
                    {
                        s_url = createURL(client, tag.getAttribute("src"));
                    }

                    if(s_url != "" && !isDownloaded(s_url))
                    {
                        thread th(HttpDownloader::start, this, s_url);
                        if(_wait)
                        {
                            th.join();
                        }
                        _threads.push_back(std::move(th));
                    }
                }
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

bool HttpDownloader::isDownloaded(const string &url)
{
    _add_url.lock();
    for(unsigned int i = 0; i < _d_urls.size(); i++)
    {
        if(_d_urls[i] == url)
        {
            return true;
        }
    }
    _add_url.unlock();
    return false;
}

string HttpDownloader::createURL(HttpClient &client, const string &path)
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
            url = client.getProtocole() + "://" +
                client.getAddress() +
                ":" + client.getPort() +
                client.getPath() + 
                path;
        }
        else
        {
            url = client.getProtocole() + "://" +
                client.getAddress() +
                ":" + client.getPort() +
                path;
        }
    }

    return url;
}

void HttpDownloader::wait(bool wait)
{
    _wait = wait;
}
