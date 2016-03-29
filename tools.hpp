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
    int toInt(const char *sttr);
    int toInt(const string &sttr);
    unsigned int toUInt(const char *sttr);
    unsigned int toUInt(const string &sttr);
    string toUpper(string str);
    string getCurrentTime();
    istream &getline(istream &in, string &line);
    string removeSpaces(const string &str);
    Exception getException(const string &msg, const char *file, int line);
    Exception getException(const string &msg, const char *file, int line, const char *func);
    void createDir(const char *path);
    void createDir(const string &path);
    bool isDirExists(const string &path);
    bool isDirExists(const string &path);
    void printHttpClientInfos(HttpClient &httpClient);

}

#endif
