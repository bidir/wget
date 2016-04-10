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
#include <string>

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
        bool _only_page;
        bool _print;
        unsigned int _nb_pages;
        unsigned int _print_refresh;
        unsigned int _depth;
        unsigned int _nb_d_th;
        unsigned int _nb_p_th;

        std::string _path;
        std::mutex _m_print;
        std::thread *_th_print;

        HttpClient _client;
        HttpDownloaderQueue _queue;

        std::vector<std::thread> _d_threads;
        std::vector<std::thread> _p_threads;


    public:
        /* ====================  Constructors  ==================== */
        HttpDownloader();
        HttpDownloader(std::string path);
        HttpDownloader(int depth);
        HttpDownloader(std::string path, int depth);
        HttpDownloader(const HttpDownloader &h) = delete;
        ~HttpDownloader();


        /* ====================  Accessors     ==================== */
        bool getOnlyPage();
        bool getPrint();
        unsigned int getNbDownloadedPages();
        unsigned int getPrintRefresh();
        unsigned int getDepth();
        unsigned int getNbDownloadThreads();
        unsigned int getNbParseThreads();
        std::string getPath();
        HttpDownloaderQueue &getQueue();


        /* ====================  Mutators      ==================== */
        void setOnlyPage(bool only = false);
        void setPrint(bool print);
        void setPrintRefresh(unsigned int milliseconds);
        void setDepth(unsigned int depth);
        void setNbDownloadThreads(unsigned int nb);
        void setNbParseThreads(unsigned int nb);
        void setPath(std::string path);


        /* ====================  Operators     ==================== */
        HttpDownloader operator=(const HttpDownloader &h) = delete;


        /* ====================  Methods       ==================== */
        void download(std::string url);
        void printInfos();
        void wait();
        

    protected:
        /* ====================  Methods       ==================== */
        static void sPrintInfos(HttpDownloader *downloader);
        static void sGet(HttpDownloader *httpDownloader);
        static void sParse(HttpDownloader *httpDownloader);
        void get();
        void parse();
        void lockPrint();
        void unlockPrint();
        std::string createURL(const std::string &path);
};
/* -----************************  end of class  ************************----- \\
   HttpDownloader
// -----****************************************************************----- */


#endif
