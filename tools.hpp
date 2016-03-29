#ifndef __H_TOOLS_H__
#define __H_TOOLS_H__

/*
 * ================================ Header =====================================
 * Filename: tools.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> Log
 *
 * Version: 1.0
 * Created: 02/22/16 16:48:55
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
#include <string>

#include "Exceptions.hpp"
#include "HttpClient.hpp"

using namespace std;


namespace tools
{
    void toUpper(string *str);
    string toUpper(string str);
    string getCurrentTime();
    string to_string();
    istream &getline(istream &in, string &line);
    string removeSpaces(const string &str);
    Exception getException(const string &msg, const char *file, int line);
    Exception getException(const string &msg, const char *file, int line, const char *func);
    void createDir(const string &path);
    void printHttpClientInfos(HttpClient &httpClient);

}

#endif
