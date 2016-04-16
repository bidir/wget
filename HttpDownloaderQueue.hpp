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
/**
 * @brief Cette classe représente une file qui gère le téléchargement d'un
 * site. Elle gère le multithreading. Contrairement à une file normale,
 * celle-ci garde toutes les urls et les fichiers en mémoire, et pointe vers
 * la prochaine url ou le prochain fichier à traiter. Ainsi, elle garantit la
 * non redondance dans le téléchargement de fichier HTML.
 */
class HttpDownloaderQueue
{
    private:
        /* ====================  Data members  ==================== */
        /**
         * @brief Permet de signaler qu'il n'y plus rien à télécharger.
         */
        bool _stop;

        /**
         * @brief Le nombre d'urls prises dans la lsite.
         *
         * Cet attribut sert à connaitre la prochaine case de la liste des
         * téléchargement à traiter. Il est donc incrémenter lorsque une url
         * est prise dans la liste.
         */
        unsigned int _d_index;
        /**
         * @brief Le nombre de fichiers pris dans la liste.
         *
         * Cet attribut sert à connaitre la prochaine case de la liste des
         * fichiers à traiter. Il est donc incrémenter lorsque un fichier est
         * pris dans la liste.
         * 
         */
        unsigned int _p_index;
        /**
         * @brief Le nombre total d'urls.
         *
         * Cet attribut sert à connaitre la dernière case de la liste des
         * téléchargements. Il est donc incrémenter lorsque une url
         * est ajoutée dans la liste. Il sert à éviter des push_back excessifs
         * sur le vecteur _urls.
         */
        unsigned int _d_add_index;
        /**
         * @brief Le nombre total de fichiers.
         *
         * Cet attribut sert à connaitre la dernière case de la liste des
         * fichiers. Il est donc incrémenter lorsque un fichier est
         * ajouté dans la liste. Il sert à éviter des push_back excessifs sur
         * le vecteur _files
         * 
         */
        unsigned int _p_add_index;
        /**
         * @brief La dernière profondeur de téléchargement utilisée.
         */
        unsigned int _last_d_depth;
        /**
         * @brief La dernière profondeur d'analyse utilisée.
         */
        unsigned int _last_p_depth;

        /**
         * @brief Mutex d'ajout d'url.
         */
        std::mutex _m_put_url;
        /**
         * @brief Mutex pour prendre une url dans la file.
         */
        std::mutex _m_get_url;
        /**
         * @brief Mutex pour l'ajout à la file d'un fichier à analyser.
         */
        std::mutex _m_put_file;
        /**
         * @brief Mutex pour prendre un fichier dans la file.
         */
        std::mutex _m_get_file;
        /**
         * @brief Variable de condition pour gérer l'ajout d'url.
         */
        std::condition_variable _cv_url;
        /**
         * @brief Variable de condition pour gérer l'ajout de fichier.
         */
        std::condition_variable _cv_file;

        /**
         * @brief Liste des profondeurs à laquelle des urls.
         */
        std::vector<unsigned int> _d_depths;
        /**
         * @brief Liste des profondeurs à laquelle des fichiers.
         */
        std::vector<unsigned int> _p_depths;
        /**
         * @brief Liste des urls.
         */
        std::vector<std::string> _urls;
        /**
         * @brief Liste des fichiers.
         */
        std::vector<std::string> _files;

        /**
         * @brief Un listener (à faire un jour).
         */
        HttpDownloaderQueueListener *_listener;

        /**
         * @brief Permet de savoir si un thread de téléchargement
         * travaille ou en attente.
         */
        std::map<std::thread::id, bool> _th_d_end;
        /**
         * @brief Permet de savoir si un thread d'analyse travaille
         * ou en attente.
         */
        std::map<std::thread::id, bool> _th_p_end;
        /**
         * @brief Permet de définir la profondeur à laquelle un thread
         * travaille.
         */
        std::map<std::thread::id, unsigned int> _th_depth;


    public:
        /* ====================  Constructors  ==================== */
        HttpDownloaderQueue();


        /* ====================  Accessors     ==================== */
        unsigned int getDIndex();
        unsigned int getPIndex();
        /**
         * @brief Renvoie le nombre total d'urls dans la liste
         *
         * @return Le nombre total d'urls dans la liste.
         */
        unsigned int getDCount();
        /**
         * @brief Renvoie le nombre total de fichiers dans la liste.
         *
         * @return Le nombre total de fichiers dans la liste.
         */
        unsigned int getPCount();
        unsigned int getDepth();
        unsigned int getLastDDepth();
        unsigned int getLastPDepth();
        std::vector<std::string> &getURLs();
        std::vector<std::string> &getFiles();


        /* ====================  Mutators      ==================== */
        void setListener(HttpDownloaderQueueListener &listener);


        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        /**
         * @brief Permet de chercher si une url est dans la file ou pas.
         *
         * @param url L'url à chercher.
         *
         * @return true si l'url est dans la file.
         */
        bool hasURL(const std::string &url);
        /**
         * @brief Permet de chercher si un fichier est dans la file ou pas.
         *
         * @param url Le fichier à chercher.
         *
         * @return true si le fichier est dans la file.
         */
        bool hasFile(const std::string &url);
        /**
         * @brief Ajouter une url à la file. Cette méthode lock le mutex
         * _m_put_url, vérifie si l'url n'est pas déjà ajoutée.
         *
         * @param url L'url à ajouter.
         * @param depth La profondeur de l'url.
         */
        void addURL(const std::string &url, unsigned int depth = 0);
        /**
         * @brief Ajouter un fichier à la file. Cette méthode lock le mutex
         * _m_put_file, vérifie si le fichier n'est pas déjà ajouté.
         *
         * @param file le fichier à ajouter.
         * @param depth la profondeur de téléchargement.
         */
        void addFile(const std::string &file, unsigned int depth = 0);
        /**
         * @brief Prendre une url dans la file. Cette méthode lock le mutex
         * _m_get_url, récupère l'url dans la liste des urls grâce à _d_index
         * et incrémente _d_index.
         *
         * @return _urls[_d_index].
         */
        std::string getURL();
        /**
         * @brief Prendre un fichier dans la file. Cette méthode lock le mutex
         * _m_get_file, récupère le fichier dans la liste des urls grâce à
         * _p_index et incrémente _p_index.
         *
         * @return _files[_p_index].
         */
        std::string getFile();
        /**
         * @brief _stop=true. Cette méthode n'arrête pas les traitement et ne
         * libère pas les mutex (il faut utiliser notifyURL pour ça).
         */
        void stop();
        /**
         * @brief Vérifier tous les threads de téléchargement sont en attente.
         */
        bool isDEnd();
        /**
         * @brief Vérifier tous les threads d'analyse sont en attente.
         */
        bool isPEnd();
        /**
         * @brief Vérifier tous les threads de téléchargementd'analyse sont
         * en attente.
         */
        bool isEnd();
        /**
         * @brief si un thread à demander l'arrêt des téléchargement.
         */
        bool isStopped();

        /**
         * @brief Signaler aux thread de téléchargement qu'une url vient
         * d'être ajoutée.
         */
        void notifyURL();
        /**
         * @brief Signaler aux threads d'analyse qu'un fichier vient d'être
         * ajouté.
         */
        void notifyFile();
        /**
         * @brief Libérer tous les mutex et mettre _stop=true.
         */
        void notifyEnd();

        /**
         * @brief Récupérer le nombre de threads de téléchargement qui
         * travaillent
         *
         * @return Le nombre de threads de téléchargement qui travaillent.
         */
        unsigned int getNbRunningDThreads();
        /**
         * @brief Récupérer le nombre de threads d'analyse qui travaillent
         *
         * @return Le nombre de threads d'analyse qui travaillent.
         */
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
