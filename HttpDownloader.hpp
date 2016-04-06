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
#include "HttpDownloaderQueue.hpp"



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
        unsigned int _depth;
        unsigned int _nb_d_th;
        unsigned int _nb_p_th;

        string _path;

        HttpClient _client;

        HttpDownloaderQueue _queue;

        vector<thread> _d_threads;
        vector<thread> _p_threads;


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
        HttpDownloaderQueue &getQueue();


        /* ====================  Mutators      ==================== */
        void setDepth(unsigned int depth);
        void setNbDownloadThreads(unsigned int nb);
        void setNbParseThreads(unsigned int nb);
        void setPath(string path);


        /* ====================  Operators     ==================== */
        HttpDownloader operator=(const HttpDownloader &h) = delete;


        /* ====================  Methods       ==================== */
        void addTag(const string &tag, const string &attr = "");
        void removeTag(const string &balise);
        void download(string url);
        void wait();
        

    protected:
        /* ====================  Methods       ==================== */
        static void sGet(HttpDownloader *httpDownloader);
        static void sParse(HttpDownloader *httpDownloader);
        void get();
        void parse();
        string createURL(const string &path);
};
/* -----************************  end of class  ************************----- \\
   HttpDownloader
// -----****************************************************************----- */


#endif
