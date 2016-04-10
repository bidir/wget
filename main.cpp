/*
 * ================================== CPP ======================================
 * Filename: main.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 02/20/16 23:30:49
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <iostream>
#include <fstream>

#include "tools.hpp"
#include "HttpClient.hpp"
#include "HttpDownloader.hpp"

using namespace std;


void http_get(const char *url, const char *nom_fichier);
void help();
void end();


ofstream log_file;
ofstream d_file;

int main(int argc, char *argv[])
{
    log_file.open("/tmp/output.log", ofstream::out|ofstream::app);
    d_file.open("/tmp/output.debug", ofstream::out |ofstream::app);

    Log::add(log_file);
    Log::setDebugOut(d_file);
    try
    {
        bool only_page(false);
        unsigned int nb_get(3), nb_par(3), depth(3);
        string path(""), url("");

        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "-h" || string(argv[i]) == "--help")
            {
                help();
                end();
                return 0;
            }
            else if(string(argv[i]) == "-s" || string(argv[i]) == "--seul-page")
            {
                only_page = true;
            }
            else if(string(argv[i]) == "-p" || string(argv[i]) == "--prof")
            {
                if(i+1 > argc)
                {
                    cerr << "ERREUR: L'option \"" << argv[i] << "\" doit etre suivi d'un nombre." << endl;
                    end();
                    return EXIT_FAILURE;
                }
                i++;
                depth = tools::toUInt(argv[i]);
            }
            else if(string(argv[i]) == "-d" || string(argv[i]) == "--debug")
            {
                Log::add(cout);
            }
            else if(string(argv[i]) == "--nb-th-get")
            {
                if(i+1 > argc)
                {
                    cerr << "ERREUR: L'option \"" << argv[i] << "\" doit etre suivi du nombre de threads." << endl;
                    end();
                    return EXIT_FAILURE;
                }
                i++;
                nb_get = tools::toUInt(argv[i]);
            }
            else if(string(argv[i]) == "--nb-th-analyse")
            {
                if(i+1 > argc)
                {
                    cerr << "ERREUR: L'option \"" << argv[i] << "\" doit etre suivi du nombre de threads." << endl;
                    end();
                    return EXIT_FAILURE;
                }
                i++;
                nb_par = tools::toUInt(argv[i]);
            }
            else
            {
                if(url == "")
                {
                    url = argv[i];
                }
                else if(path == "")
                {
                    path = argv[i];
                }
                else
                {
                    cerr << "ERREUR: L'option \"" << argv[i] << "\" est inconnue." << endl;
                    end();
                    return EXIT_FAILURE;
                }
            }
        }
        if(url == "")
        {
            cerr << "ERREUR: il faut preciser une url." << endl;
            return EXIT_FAILURE;
        }
        if(path == "")
        {
            cerr << "ERREUR: il faut preciser un repertoire." << endl;
            return EXIT_FAILURE;
        }
        if(!tools::isDirExists(path) && !only_page)
        {
            cerr << "ERREUR: Le repertoire \"" <<  argv[argc - 1] << "\" n'existe pas" << endl;
            end();
            return EXIT_FAILURE;
        }

        Log::init();
        HttpDownloader downloader;

        downloader.setPath(path);
        downloader.setOnlyPage(only_page);
        downloader.setNbDownloadThreads(nb_get);
        downloader.setNbParseThreads(nb_par);
        downloader.setDepth(depth);
        downloader.setPrintRefresh(100);

        downloader.download(url);
        downloader.printInfos();
        downloader.wait();
        end();
        return 0; 
    }
    catch(Exception &e)
    {
        AddTrace(e);
        Log::e(e);
        end();
        return e.getCode();
    }
    catch(const exception &e)
    {
        Log::e(e.what());
        end();
        return -1;
    }

    cerr << "ERREUR: il faut donner l'URL et un nom de dossier." << endl;
    end();
    return EXIT_FAILURE;
}

void end()
{
    cout << endl;
    log_file.close();
    d_file.close();
}

void help()
{
    ifstream is;
    is.open("help.txt");
    string str;
    cout << endl;
    while(getline(is, str))
    {
        cout << str << endl;
    }
    is.close();
}
