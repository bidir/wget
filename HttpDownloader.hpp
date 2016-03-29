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
        unsigned int _depth;
        unsigned int _nb_d_th;
        unsigned int _nb_p_th;
        unsigned int _th_d_end;
        unsigned int _th_p_end;

        unsigned int _d_index;
        unsigned int _p_index;

        string _path;

        HttpClient _client;

        mutex _m_put_d_url;
        mutex _m_get_d_url;
        mutex _m_put_p_file;
        mutex _m_get_p_file;
        condition_variable _cv_d_url;
        condition_variable _cv_p_file;

        vector<string> _d_urls;
        vector<string> _p_files;
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
        vector<HTMLTag> &getTags();
        vector<string> &getDownloadedURLs();


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
        void addDURL(const string &url);
        void addPFile(const string &url);
        string getDURL();
        string getPFile();
        static void sGet(HttpDownloader *httpDownloader);
        static void sParse(HttpDownloader *httpDownloader);
        void get();
        void parse();
        bool isAdded(const string &url);
        bool isDEnd();
        bool isPEnd();
        bool isEnd();
        void notifyDURL();
        void notifyPFile();
        string createURL(const string &path);
};
/* -----************************  end of class  ************************----- \\
   HttpDownloader
// -----****************************************************************----- */


#endif
