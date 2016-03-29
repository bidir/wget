#ifndef __H_HTTP_DOWNLOADER_H__
#define __H_HTTP_DOWNLOADER_H__

/*
 * ================================ Header =====================================
 * Filename: HttpDownloader.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:03:54 AM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "HttpClient.hpp"
#include "HTMLTag.hpp"



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HttpDownloader
Description:  
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HttpDownloader
{
    private:
        /* ====================  Data members  ==================== */
        bool _wait;

        unsigned int _depth;
        unsigned int _nb_d_th;
        unsigned int _nb_p_th;

        string _path;

        mutex _add_url;
        condition_variable _cv_parse;
        condition_variable _cv_download;

        vector<string> _d_urls;
        vector<string> _p_urls;
        vector<thread> _threads;


    public:
        /* ====================  Constructors  ==================== */
        HttpDownloader();
        HttpDownloader(string path);
        HttpDownloader(int depth);
        HttpDownloader(string path, int depth);
        HttpDownloader(const HttpDownloader &h) = delete;


        /* ====================  Accessors     ==================== */
        unsigned int getDepth();
        unsigned int getNbDownloadThreads();
        unsigned int getNbParseThreads();
        string getPath();
        vector<HTMLTag> &getTags();
        vector<string> &getDownloadedURLs();


        /* ====================  Mutators      ==================== */
        void setDepth(unsigned int depth);
        void setNbDownloadThreads(unsigned int nb);
        void setNbParseThreads(unsigned int nb);
        void setPath(string path);
        void download(string url);
        void wait(bool wait = true);


        /* ====================  Operators     ==================== */
        HttpDownloader operator=(const HttpDownloader &h) = delete;


        /* ====================  Methods       ==================== */
        void addTag(const string &tag, const string &attr = "");
        void removeTag(const string &balise);
        void parseData();
        bool isDownloaded(const string &url);
        string createURL(HttpClient &client, const string &path);
        

    protected:
        /* ====================  Methods       ==================== */
        static void start(HttpDownloader *httpDownloader, const string &url);

};
/* -----************************  end of class  ************************----- \\
   HttpDownloader
// -----****************************************************************----- */


#endif
