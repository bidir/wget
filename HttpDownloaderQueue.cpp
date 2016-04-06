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
#include "HttpDownloaderQueue.hpp"


/* ====================  Constructors  ==================== */
HttpDownloaderQueue::HttpDownloaderQueue():
    _stop(false),
    _m_put_url(),
    _m_get_url(),
    _m_put_file(),
    _m_get_file(),
    _cv_url(),
    _cv_file(),
    _d_depths(0),
    _p_depths(0),
    _urls(0),
    _files(0)
{}


/* ====================  Accessors     ==================== */
unsigned int HttpDownloaderQueue::getDepth()
{
    return _th_depth[this_thread::get_id()];
}


/* ====================  Mutators      ==================== */
void HttpDownloaderQueue::setListener(HttpDownloaderQueueListener &listener)
{
    _listener = &listener;
}


/* ====================  Methods       ==================== */
void HttpDownloaderQueue::addURL(const string &url, unsigned int depth)
{
    _m_put_url.lock();
    if(url != "" && !hasURL(url))
    {
        _urls.push_back(url);
        _d_depths.push_back(depth);
        _cv_url.notify_one();
    }
    _m_put_url.unlock();
}

void HttpDownloaderQueue::addFile(const string &url, unsigned int depth)
{
    _m_put_file.lock();
    _files.push_back(url);
    _p_depths.push_back(depth);
    _cv_file.notify_one();
    _m_put_file.unlock();
}

string HttpDownloaderQueue::getURL()
{
    _th_d_end[this_thread::get_id()] = true;
    unique_lock<mutex> lock(_m_get_url);
    while(_urls.empty() && !_stop)
    {
        _cv_url.wait(lock);
    }
    string url("");
    if(!_stop)
    {
        _th_d_end[this_thread::get_id()] = false;
        url = _urls.back();
        _urls.pop_back();
        if(_listener)
        {
            _listener->onGetURL(_d_depths.back());
        }
        _th_depth[this_thread::get_id()] = _d_depths.back();
        _d_depths.pop_back();
    }
    return url;
}

string HttpDownloaderQueue::getFile()
{
    _th_p_end[this_thread::get_id()] = true;
    unique_lock<mutex> lock(_m_get_file);
    while(_files.empty() && !_stop)
    {
        _cv_file.wait(lock);
    }
    string file("");
    if(!_stop)
    {
        _th_p_end[this_thread::get_id()] = false;
        file = _files.back();
        _files.pop_back();
        if(_listener)
        {
            _listener->onGetFile(_p_depths.back());
        }
        _th_depth[this_thread::get_id()] = _p_depths.back();
        _p_depths.pop_back();
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
    for(unsigned int i = 0; i < _urls.size(); i++)
    {
        if(_urls[i] == file)
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

bool HttpDownloaderQueue::empty()
{
    return _urls.empty() && _files.empty();
}

bool HttpDownloaderQueue::isDEnd()
{
    if(!_urls.empty())
    {
        return false;
    }
    for(map<thread::id, bool>::iterator it = _th_d_end.begin(); it != _th_d_end.end(); it++)
    {
        if(!it->second && it->first != this_thread::get_id())
        {
            return false;
        }
    }
    return true;
}

bool HttpDownloaderQueue::isPEnd()
{
    if(!_files.empty())
    {
        return false;
    }
    for(map<thread::id, bool>::iterator it = _th_p_end.begin(); it != _th_p_end.end(); it++)
    {
        if(!it->second && it->first != this_thread::get_id())
        {
            return false;
        }
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
    _cv_url.notify_one();
    _cv_file.notify_one();
}
