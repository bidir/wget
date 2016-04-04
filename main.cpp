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
//#include <unistd.h>
//#include <cstring>
//#include <sys/socket.h>
//#include <netdb.h>
#include <fstream>
#include <regex>

#include "tools.hpp"
#include "HttpClient.hpp"
#include "HttpDownloader.hpp"

using namespace std;


void http_get(const char *url, const char *nom_fichier);
void help();

int main(int argc, char *argv[])
{
    ofstream log_file;
    log_file.open("/tmp/output.log", ofstream::out|ofstream::app);
    Log::add(log_file);
    try
    {
        /*
        HttpTag httpTag;
        cout << "---------------------------------" << endl;
        httpTag.parse("<merde attr1=\"merde1\" attr2=\"merde2\" />");
        cout << "---------------------------------" << endl;
        httpTag.parse("<merde attr1=\"merde1\" attr2=\"merde2\">salut la merde</merde>");
        */

        if(argc >= 3)
        {
            if(!tools::isDirExists(argv[argc - 1]))
            {
                cerr << "ERREUR: Le repertoire \"" <<  argv[argc - 1] << "\" n'existe pas" << endl;
                return EXIT_FAILURE;
            }
            HttpDownloader downloader(argv[argc-1]);
            for(int i = 1; i < argc - 2; i++)
            {
                //int depth(3);
                if(string(argv[i]) == "-h" || string(argv[i]) == "--help")
                {
                    help();
                    return 0;
                }
                else if(string(argv[i]) == "-p" || string(argv[i]) == "--prof")
                {
                }
                else if(string(argv[i]) == "-d" || string(argv[i]) == "--debug")
                {
                    Log::add(cout);
                }
                else if(string(argv[i]) == "--nb-th-get")
                {
                    if(i >= argc - 3)
                    {
                        cerr << "ERREUR: L'option \"" << argv[i] << "\" doit etre suivi du nombre de threads." << endl;
                        return EXIT_FAILURE;
                    }
                }
                else if(string(argv[i]) == "--nb-th-analyse")
                {
                }
                else
                {
                    cerr << "ERREUR: L'option \"" << argv[i] << "\" est inconnue." << endl;
                    return EXIT_FAILURE;
                }
            }

            Log::init();
            ///http_get(argv[1], argv[2]);
            downloader.download(argv[argc-2]);
            downloader.wait();
            log_file.close();
            return 0; 
        }
    }
    catch(const Exception &e)
    {
        Log::e(e);
        log_file.close();
        return e.getCode();
    }
    catch(const exception &e)
    {
        Log::e(e.what());
        log_file.close();

        return -1;
    }

    cerr << "ERREUR: il faut donner l'URL et un nom de dossier." << endl;
    log_file.close();
    return EXIT_FAILURE;
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

void http_get(const char *url, const char *nom_fichier)
{
    HttpClient client;
    client.url(string(url));
    Log::i("Envoi de la requete " + string(url));
    try
    {
        client.connect();
    }
    catch(const exception &e)
    {
        cerr << "ERREUR: " << e.what() << endl;
        throw;
    }

    client.get();
    client.setSizeToRead(1000);
    client.parseHeader();
    ostringstream oss;
    oss << "-------------- Infos recuperees dans l'en-tete -------------- " << endl;
    oss << "chunked = " << client.isChunked() << endl;
    oss << "statut = " << client.getStatus() << endl;
    oss << "message du statut = " << client.getStatusMessage() << endl;
    oss << "Taille des donnees = " << client.getContentLength() << endl;
    oss << "Type des donnees = " << client.getContentType() << endl;
    oss << "Version http = " << client.getHttpVersion() << endl;
    oss << "Unite donnees = " << client.getAcceptRanges() << endl;
    oss << "Location = " << client.getLocation() << endl;
    oss << "Encoding = " << client.getEncoding() << endl;
    oss << "------------------------------------------------------------- " << endl;
    if(client.getUnparsedHeader() != "")
    {
        oss << "------------------- Le reste de l'en-tete ------------------- " << endl
            << client.getUnparsedHeader() << endl
            << "------------------------------------------------------------- " << endl;
    }
    Log::i(oss.str());
    if(client.getStatus() != 200 && client.getStatus() != 302)
    {
        cerr << "ERREUR " << client.getStatus() << " message '" << client.getStatusMessage() << "'" <<endl;
        throw Exception(client.getStatus(), "Erreur HTTP: " + client.getStatusMessage(), __FILE__, __LINE__, __FUNCTION__);
    }
    if(client.getStatus() == 302)
    {
        Log::i("Redirection vers " + client.getLocation());
        http_get(client.getLocation().c_str(), nom_fichier);
    }
    else
    {
        Log::i("Ecriture des donnees dans " + string(nom_fichier));
        ofstream file;
        file.open(nom_fichier);
        if(file.fail())
        {
            cerr << "ERREUR: impossible d'ouvrir le fichier de sortie \"" << nom_fichier << "\"" << endl;
            throw Exception(ERR_OUTPUT_FILE, "Erreur pendant l'ouverture du fichier de sortie \"" + string(nom_fichier) +  "\"", __FILE__, __LINE__, __FUNCTION__);
        }
        client.writeInOstream(true, file);
        client.recuperateData();
        file.close();
        Log::i("Fermeture de la connexion au serveur " + string(url));
        client.close(); 
    }
}
