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



namespace tools
{
    void toUpper(std::string *str);
    std::string toString(const int &val);
    std::string toString(const unsigned int &val);
    std::string toString(size_t &val);
    int toInt(const char *sttr);
    int toInt(const std::string &sttr);
    unsigned int toUInt(const char *sttr);
    unsigned int toUInt(const std::string &sttr);
    std::string toUpper(std::string str);
    std::string getCurrentTime();
    std::istream &getline(std::istream &in, std::string &line);
    std::string removeSpaces(const std::string &str);
    void createDir(const char *path);
    void createDir(const std::string &path);
    bool isDirExists(const std::string &path);
    bool isDirExists(const std::string &path);
    std::vector<char> ungzip(const char *compr, unsigned int size);
}

#endif
