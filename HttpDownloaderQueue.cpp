/*
 * ================================== CPP ======================================
 * Filename: HttpDownloaderQueue.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 05/04/2016 18:41:57
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */


#include <iostream>
#include <sstream>
#include "HttpDownloaderQueue.hpp"
#include "Log.hpp"


using namespace std;


/* ====================  Constructors  ==================== */
HttpDownloaderQueue::HttpDownloaderQueue():
    _stop(false),
    _d_index(0),
    _p_index(0),
    _m_put_url(),
    _m_get_url(),
    _m_put_file(),
    _m_get_file(),
    _cv_url(),
    _cv_file(),
    _d_depths(0),
    _p_depths(0),
    _urls(0),
    _files(0),
    _listener(NULL)
{}


/* ====================  Accessors     ==================== */
unsigned int HttpDownloaderQueue::getDIndex()
{
    return _d_index;
}

unsigned int HttpDownloaderQueue::getPIndex()
{
    return _p_index;
}

unsigned int HttpDownloaderQueue::getDepth()
{
    return _th_depth[this_thread::get_id()];
}

vector<string> &HttpDownloaderQueue::getURLs()
{
    return _urls;
}

vector<string> &HttpDownloaderQueue::getFiles()
{
    return _files;
}


/* ====================  Mutators      ==================== */
void HttpDownloaderQueue::setListener(HttpDownloaderQueueListener &listener)
{
    _listener = &listener;
}


/* ====================  Methods       ==================== */
void HttpDownloaderQueue::addURL(const string &url, unsigned int depth)
{
    //Bloquer l'ajout des urls.
    unique_lock<mutex>(_m_put_url);
    //Si l'url n'est pas déjà ajoutée et n'est pas vide.
    if(url != "" && !hasURL(url))
    {
        _urls.push_back(url); //Ajouter à la liste
        _d_depths.push_back(depth);//Ajoute sa profondeur
        //Libérer un thread de récupération d'url.
        LogD("Ajout de l'url \"" + url + "\"");
        _cv_url.notify_one();
    }
    //Libérer le mutex d'ajout d'urls.
}

void HttpDownloaderQueue::addFile(const string &file, unsigned int depth)
{
    //Bloquer l'ajout des fichiers.
    unique_lock<mutex>(_m_put_file);
    //Si le fichier n'est pas déjà ajouté et n'est pas vide.
    if(file != "" && !hasFile(file))
    {
        _files.push_back(file);
        _p_depths.push_back(depth);
        //Libérer un thread d'analyse.
        LogD("Ajout du fichier \"" + file + "\"");
        _cv_file.notify_one();
    }
}

string HttpDownloaderQueue::getURL()
{
    //Marquer le thread actuel fini.
    _th_d_end[this_thread::get_id()] = true;
    //Locker le mutex de récupération d'url
    unique_lock<mutex> lock(_m_get_url);
    //Tant que l'index des urls pointe vers la dernière case
    while(_urls.size() == _d_index && !_stop)
    {
        //Attente qu'une url soit ajoutée.
        _cv_url.wait(lock);
    }
    stringstream ss;
    string url("");
    //Si un thread n'a pas signaler que c'est fini.
    if(!_stop)
    {
        //Marquer le thread actuel en exécution.
        _th_d_end[this_thread::get_id()] = false;
        if(_listener)
        {
            _listener->onGetURL(_d_depths[_d_index]);
        }
        //Récupérer l'url à l'index de téléchargement
        url = _urls[_d_index];
        //Attribuer la profondeur à laquelle travaillera le thread.
        _th_depth[this_thread::get_id()] = _d_depths[_d_index];
        //Incrémenter l'indice de téléchargement.
        _d_index++;
        LogD("Sortie de l'url \"" + url + "\"");
    }
    else
    {
        //Sinon on marque le thrad fini.
        _th_d_end[this_thread::get_id()] = true;
    }
    return url;
}

//voir getURL.
string HttpDownloaderQueue::getFile()
{
    _th_p_end[this_thread::get_id()] = true;
    unique_lock<mutex> lock(_m_get_file);
    while(_files.size() == _p_index && !_stop)
    {
        _cv_file.wait(lock);
    }

    string file("");
    if(!_stop)
    {
        _th_p_end[this_thread::get_id()] = false;
        file = _files[_p_index];
        if(_listener)
        {
            _listener->onGetFile(_p_depths[_p_index]);
        }
        _th_depth[this_thread::get_id()] = _p_depths[_p_index];
        _p_index++;
        LogD("Sortie du fichier \"" + file + "\"");
    }
    return file;
}

bool HttpDownloaderQueue::hasURL(const string &url)
{
    for(unsigned int i = 0; i < _urls.size(); i++)
    {
        if(_urls[i] == url)
        {
            return true;
        }
    }
    return false;
}

bool HttpDownloaderQueue::hasFile(const string &file)
{
    for(unsigned int i = 0; i < _files.size(); i++)
    {
        if(_files[i] == file)
        {
            return true;
        }
    }
    return false;
}

void HttpDownloaderQueue::stop()
{
    _stop = true;
}

bool HttpDownloaderQueue::isDEnd()
{
    //S'il rest des url à télécharger alors ce n'est pas fini.
    if(_urls.size() != _d_index)
    {
        return false;
    }
    unsigned int size = 0;
    //Si tous les thread sont fini, alors c'est fini.
    for(
            map<thread::id, bool>::iterator it = _th_d_end.begin();
            size < _th_d_end.size();
            it++
       )
    {
        if(!it->second && it->first != this_thread::get_id())
        {
            return false;
        }
        size++;
    }
    //Sinon, un thread travaille encore.
    return true;
}

//voir isDEnd()
bool HttpDownloaderQueue::isPEnd()
{
    if(_files.size() != _p_index)
    {
        return false;
    }
    unsigned int size = 0;
    for(
            map<thread::id, bool>::iterator it = _th_p_end.begin();
            size < _th_p_end.size();
            it++
       )
    {
        if(!it->second && it->first != this_thread::get_id())
        {
            return false;
        }
        size++;
    }
    return true;
}

bool HttpDownloaderQueue::isEnd()
{
    return isDEnd() && isPEnd();
}

bool HttpDownloaderQueue::isStopped()
{
    return _stop;
}

void HttpDownloaderQueue::notifyURL()
{
    _cv_url.notify_one();
}

void HttpDownloaderQueue::notifyFile()
{
    _cv_file.notify_one();
}

void HttpDownloaderQueue::notifyEnd()
{
    stop();
    _th_d_end[this_thread::get_id()] = true;
    _cv_url.notify_all();
    _cv_file.notify_all();
}

unsigned int HttpDownloaderQueue::getNbRunningDThreads()
{
    if(_stop)
    {
        return 0;
    }
    unsigned int nb(0);
    unsigned int size = 0;
    for(
            map<thread::id, bool>::iterator it = _th_d_end.begin();
            size < _th_d_end.size();
            it++
       )
    {
        if(!it->second)
        {
            nb++;
        }
        size++;
    }
    return nb;
}

unsigned int HttpDownloaderQueue::getNbRunningPThreads()
{
    if(_stop)
    {
        return 0;
    }
    unsigned int nb(0);
    unsigned int size = 0;
    for(
            map<thread::id, bool>::iterator it = _th_p_end.begin();
            size < _th_p_end.size();
            it++
       )
    {
        if(!it->second)
        {
            nb++;
        }
        size++;
    }
    return nb;
}
