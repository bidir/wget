#ifndef __H_HTTP_DOWNLOADER_QUEUE_H__
#define __H_HTTP_DOWNLOADER_QUEUE_H__


/*
 * ================================ Header =====================================
 * Filename: HttpDownloaderQueue.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 05/04/2016 18:42:52
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */



#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <map>


class HttpDownloaderQueueListener;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HttpDownloaderQueue
Description:
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HttpDownloaderQueue
{
    private:
        /* ====================  Data members  ==================== */
        bool _stop;

        std::mutex _m_put_url;
        std::mutex _m_get_url;
        std::mutex _m_put_file;
        std::mutex _m_get_file;
        std::condition_variable _cv_url;
        std::condition_variable _cv_file;

        std::vector<unsigned int> _d_depths;
        std::vector<unsigned int> _p_depths;
        std::vector<std::string> _urls;
        std::vector<std::string> _files;

        HttpDownloaderQueueListener *_listener;

        std::map<std::thread::id, bool> _th_d_end;
        std::map<std::thread::id, bool> _th_p_end;
        std::map<std::thread::id, unsigned int> _th_depth;


    public:
        /* ====================  Constructors  ==================== */
        HttpDownloaderQueue();


        /* ====================  Accessors     ==================== */
        unsigned int getDepth();


        /* ====================  Mutators      ==================== */
        void setListener(HttpDownloaderQueueListener &listener);


        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        bool hasURL(const std::string &url);
        bool hasFile(const std::string &url);
        void addURL(const std::string &url, unsigned int depth = 0);
        void addFile(const std::string &url, unsigned int depth = 0);
        std::string getURL();
        std::string getFile();
        void stop();
        bool empty();
        bool isDEnd();
        bool isPEnd();
        bool isEnd();
        bool isStopped();

        void notifyURL();
        void notifyFile();
        void notifyEnd();

        unsigned int getNbRunningDThreads();
        unsigned int getNbRunningPThreads();


    protected:
        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
   HttpDownloaderQueue
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HttpDownloaderQueueListener
Description:
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HttpDownloaderQueueListener
{
    private:
        /* ====================  Data members  ==================== */


    public:

        /* ====================  Constructors  ==================== */
        HttpDownloaderQueueListener();


        /* ====================  Accessors     ==================== */



        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        virtual void onGetURL(unsigned int depth) = 0;
        virtual void onGetFile(unsigned int depth) = 0;


    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
   HttpDownloaderQueueListener
// -----****************************************************************----- */

#endif
