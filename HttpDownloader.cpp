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
    _end(false),
    _depth(depth),
    _nb_d_th(4),
    _nb_p_th(4),
    _d_index(0),
    _p_index(0),
    _path(path),
    _client(),
    _m_put_d_url(),
    _m_get_d_url(),
    _m_put_p_file(),
    _m_get_p_file(),
    _cv_d_url(),
    _cv_p_file(),
    _d_urls(0),
    _p_files(0),
    _d_depths(0),
    _p_depths(0),
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
void HttpDownloader::sGet(HttpDownloader *httpDownloader)
{
    do
    {
        httpDownloader->get();
    }while(!httpDownloader->isEnd());
    httpDownloader->notifyEnd();
}

void HttpDownloader::sParse(HttpDownloader *httpDownloader)
{
    do
    {
        httpDownloader->parse();
    }while(!httpDownloader->isEnd());
    httpDownloader->notifyEnd();
}

void HttpDownloader::notifyDURL()
{
    _cv_d_url.notify_one();
}

void HttpDownloader::notifyPFile()
{
    _cv_p_file.notify_one();
}

void HttpDownloader::notifyEnd()
{
    _end = true;
    _cv_d_url.notify_all();
    _cv_p_file.notify_all();
}

void HttpDownloader::addURL(const string &url, unsigned int depth)
{
    _m_put_d_url.lock();
    if(url != "" && !isAdded(url))
    {
        _d_urls.push_back(url);
        _d_depths.push_back(depth);
        _cv_d_url.notify_one();
    }
    _m_put_d_url.unlock();
}

void HttpDownloader::addFile(const string &url, unsigned int depth)
{
    _m_put_p_file.lock();
    _p_files.push_back(url);
    _p_depths.push_back(depth);
    _cv_p_file.notify_one();
    _m_put_p_file.unlock();
}

string HttpDownloader::getDURL()
{
    unique_lock<mutex> lock(_m_get_d_url);
    while(_d_index  == _d_urls.size() && !_end)
    {
        _cv_d_url.wait(lock);
    }
    string url("");
    if(!_end)
    {
        url = _d_urls[_d_index];
        _th_d_end[this_thread::get_id()] = false;
        _th_depth[this_thread::get_id()] = _d_depths[_d_index];
        _d_index++;
    }
    return url;
}

string HttpDownloader::getPFile()
{
    unique_lock<mutex> lock(_m_get_p_file);
    while(_p_index == _p_files.size() && !_end)
    {
        _cv_p_file.wait(lock);
    }
    string file("");
    if(!_end)
    {
        _th_p_end[this_thread::get_id()] = false;
        _th_depth[this_thread::get_id()] = _p_depths[_p_index];
        file = _p_files[_p_index];
        _p_index++;
    }
    return file;
}

void HttpDownloader::get()
{
    string url = getDURL();
    if(url == "")
    {
        _th_d_end[this_thread::get_id()] = true;
        return;
    }

    unsigned int depth = _th_depth[this_thread::get_id()];
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
            _th_d_end[this_thread::get_id()] = true;
            return;
        }

        if(client.getStatus() == 301 || client.getStatus() == 302)
        {
            Log::i("Redirection vers " + client.getLocation());
            addURL(client.getLocation(), depth);
            _th_d_end[this_thread::get_id()] = true;
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
                _th_d_end[this_thread::get_id()] = true;
                return;
            }
            client.writeInOstream(true, file);
            client.recuperateData();


            Log::i("Ecriture des donnees dans " + filename);
            file.close();
            Log::i("Fermeture de la connexion au serveur " + string(url));
            client.close(); 

            if(tools::toUpper(client.getContentType()) == "TEXT/HTML")
            {
                addFile(filename, depth + 1);
            }
        }
    }
    catch(const Exception &e)
    {
        Log::w(e);
        _th_d_end[this_thread::get_id()] = true;
        return;
    }
    catch(const exception &ex)
    {
        Log::w(ex.what());
        _th_d_end[this_thread::get_id()] = true;
        return;
    }
    _th_d_end[this_thread::get_id()] = true;
}

void HttpDownloader::parse()
{
    string filename = getPFile();
    if(filename == "" || _th_depth[this_thread::get_id()] >= _depth)
    {
        _th_p_end[this_thread::get_id()] = true;
        return;
    }
    unsigned int depth = _th_depth[this_thread::get_id()];
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
            _th_p_end[this_thread::get_id()] = true;
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
            else if(tagname == "A" && tag.isAttributeExists("href") && depth + 1 < _depth)
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
                addURL(s_url, depth);
            }
        }
    }
    catch(const Exception &e)
    {
        Log::w(e);
        _th_p_end[this_thread::get_id()] = true;
        return;
    }
    catch(const exception &ex)
    {
        Log::w(ex.what());
        _th_p_end[this_thread::get_id()] = true;
        return;
    }
    _th_p_end[this_thread::get_id()] = true;
}

void HttpDownloader::download(string url)
{
    try
    {
        _client.url(url);
    }
    catch(const Exception &ex)
    {
        //cerr << ex.what() << endl;
        Log::w(ex);
        return;
    }
    for(unsigned int i = 0; i < _nb_p_th; i++)
    {
        thread th(HttpDownloader::sParse, this);
        _th_p_end[th.get_id()] = true;
        _p_threads.push_back(move(th));
    }

    for(unsigned int i = 0; i < _nb_d_th; i++)
    {
        thread th(HttpDownloader::sGet, this);
        _th_d_end[th.get_id()] = true;
        _d_threads.push_back(move(th));
    }
    addURL(url);
}

bool HttpDownloader::isAdded(const string &url)
{
    for(unsigned int i = 0; i < _d_urls.size(); i++)
    {
        if(_d_urls[i] == url)
        {
            return true;
        }
    }
    return false;
}

bool HttpDownloader::isDEnd()
{
    if(_d_index != _d_urls.size())
    {
        return false;
    }
    for(map<thread::id, bool>::iterator it = _th_d_end.begin(); it != _th_d_end.end(); it++)
    {
        if(!it->second)
        {
            return false;
        }
    }
    return true;
}

bool HttpDownloader::isPEnd()
{
    if(_p_index != _p_files.size())
    {
        return false;
    }
    for(map<thread::id, bool>::iterator it = _th_p_end.begin(); it != _th_p_end.end(); it++)
    {
        if(!it->second)
        {
            return false;
        }
    }
    return true;
}

bool HttpDownloader::isEnd()
{
    return isDEnd() && isPEnd();
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
