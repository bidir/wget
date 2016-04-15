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
#include <map>
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

        /**
         * @brief Si l'on veut télécharger qu'un fichier sans traitement.
         */
        bool _only_page;
        /**
         * @brief Si afficher le nombre de fichiers téléchargés et l'état des
         * thread en exécution.
         */
        bool _print;

        /**
         * @brief Le nombre de fichiers téléchargées.
         */
        unsigned int _nb_files;

        /**
         * @brief Le temps en millisecondes de rafraichissement de
         * l'impression des informations de traitement.
         */
        unsigned int _print_refresh;
        /**
         * @brief La profondeur de téléchargement.
         */
        unsigned int _depth;
        /**
         * @brief Le nombre de threads de téléchargement à lancer.
         */
        unsigned int _nb_d_th;
        /**
         * @brief Le nombre de threads d'analyse à lancer.
         */
        unsigned int _nb_p_th;

        /**
         * @brief Le répertoire dans lequel seront sauvegardées les pages
         * téléchargée.
         */
        std::string _path;
        /**
         * @brief Le mutex de gestion de l'impression des informations de
         * traitement.
         */
        std::mutex _m_print;
        /**
         * @brief Le thread de l'impression des informations de traitement.
         */
        std::thread *_th_print;

        /**
         * @brief Un client Http qui permettra de sauvegarder certaines
         * informations sur le serveur.
         */
        HttpClient _client;
        /**
         * @brief Une file de gestion des fichiers à télécharger et les
         * fichiers à traiter.
         */
        HttpDownloaderQueue _queue;

        /**
         * @brief La liste des balises HTML à inspecter.
         */
        std::vector<std::string> _tags;
        /**
         * @brief La liste des threads de téléchargement.
         */
        std::vector<std::thread> _d_threads;
        /**
         * @brief La liste des threads d'analyse.
         */
        std::vector<std::thread> _p_threads;

        /**
         * @brief La liste des attributs de chaque balise HTML pour récupérer
         * un fichier à télécharger.
         */
        std::map<std::string, std::string> _tag_to_attr;


    public:
        /* ====================  Constructors  ==================== */
        /**
         * @brief Constructeur par défaut.
         */
        HttpDownloader();
        /**
         * @brief Constructeur avec initialisation du répertoire de
         * sauvegarde.
         *
         * @param path Le répertoire dans lequel sauvegarder les données.
         */
        HttpDownloader(std::string path);
        /**
         * @brief Constructeur avec initialisation de la profondeur de
         * téléchargement.
         *
         * @param depth La profondeur de téléchargement.
         */
        HttpDownloader(int depth);
        /**
         * @brief Constructeur avec initialisation du répertoire et de la profondeur de
         * téléchargement.
         *
         * @param path Répertoire où sauvegarder les données.
         * @param depth Profondeur de téléchargement.
         */
        HttpDownloader(std::string path, int depth);
        /**
         * @brief Constructeur par copie.
         *
         * @param h Rien à dire.
         */
        HttpDownloader(const HttpDownloader &h) = delete;
        ~HttpDownloader();


        /* ====================  Accessors     ==================== */
        bool getOnlyPage();
        bool getPrint();
        unsigned int getNbDownladedFiles();
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
        /**
         * @brief Ajouter le nom d'une balise HTML à inspecter.
         *
         * @param tag Le nom de la balise.
         * @param attr L'attribut qui possède le lien vers la page à
         * télécharger.
         */
        void addTag(const std::string &tag, const std::string &attr);
        /**
         * @brief Lancer le téléchargement des fichiers.
         *
         * @param url L'url de départ.
         */
        void download(std::string url);
        /**
         * @brief Permet d'imprimer les informations de traitement.
         */
        void printInfos();
        /**
         * @brief Mise en attente des threads de traitement.
         */
        void wait();
        

    protected:
        /* ====================  Methods       ==================== */
        /**
         * @brief Méthode statique lancer par le thread d'impression
         * d'information de traitement.
         *
         * @param downloader L'objet de type HttpDownloader que le thread
         * utilisera pour appeler sa fonction printInfos().
         */
        static void sPrintInfos(HttpDownloader *downloader);
        /**
         * @brief Méthode statique lancer par les threads de téléhcargement.
         *
         * @param downloader L'objet de type HttpDownloader que le thread
         * utilisera pour appeler sa fonction get().
         */
        static void sGet(HttpDownloader *httpDownloader);
        /**
         * @brief Méthode statique lancer par les threads d'analyse.
         *
         * @param downloader L'objet de type HttpDownloader que le thread
         * utilisera pour appeler sa fonction parse().
         */
        static void sParse(HttpDownloader *httpDownloader);
        /**
         * @brief Elle permet de récupérer une URL dans la file, la
         * télécharger, la sauvegarder dans fichier dont elle ajoute
         * le nom à la file d'analyse.
         */
        void get();
        /**
         * @brief Elle permet de récupérer un fichier dans la file, 
         * l'analyser et d'ajouter les urls à la file de téléchargement.
         */
        void parse();
        /**
         * @brief Permet de locker le mutex _m_print.
         */
        void lockPrint();
        /**
         * @brief Permet de libérer le mutex _m_print.
         */
        void unlockPrint();
        /**
         * @brief Permet de créer une url à partir de la valeur de l'attribut
         * de l'une des balises inspectées.
         */
        std::string createURL(const std::string &path);
        /**
         * @brief Permet de remplacer toutes les uri pointant vers le serveur
         * de téléchargement avec une url complète.
         *
         * @param data Les text dans lequel effectuer ces remplacements.
         */
        void replaceServer(std::string &data, const std::string &tag, const std::string &label);
        /**
         * @brief Replacer les uri commençant par la racine avec le chemin
         * local.
         *
         * @param data Les text dans lequel effectuer ces remplacements.
         * @param tag La balise à traiter.
         * @param label L'attribut.
         */
        void replaceRoot(std::string &data, const std::string &tag, const std::string &label);
        void replaceURI(const std::string &filename);
};
/* -----************************  end of class  ************************----- \\
   HttpDownloader
// -----****************************************************************----- */


#endif
