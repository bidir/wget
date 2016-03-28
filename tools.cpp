/*
 * ================================== CPP ======================================
 * Filename: tools.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 02/22/16 16:48:44
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <regex>

#include <boost/filesystem.hpp>

#include "tools.hpp"
#include "Log.hpp"


using namespace boost::filesystem; 
typedef boost::filesystem::path Path;
typedef boost::system::error_code ErrorCode;
typedef boost::system::system_error SystemError;


namespace tools
{


    string getCurrentTime()
    {
        time_t result = std::time(NULL);
        char *time = asctime(localtime(&result));
        char *pch = strtok (time, " :");
        pch = strtok (NULL, " :");
        string MM(pch);
        pch = strtok (NULL, " :");
        string dd(pch);
        pch = strtok (NULL, " :");
        string hh(pch);
        pch = strtok (NULL, " :");
        string mm(pch);
        pch = strtok (NULL, " :");
        string ss(pch);
        pch = strtok (NULL, " :");
        string yy(pch);
        yy[yy.size()-1] = '\0';
        return yy + "/" + MM + "/" + dd + " " + hh + ":" + mm + ":" + ss;
    }

    void toUpper(string *str)
    {
        transform(str->begin(), str->end(), str->begin(), ::toupper);
    }

    string toUpper(string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    istream &getline(istream &in, string &line)
    {
        if(
                std::getline(in, line) &&
                line.find("\r") != string::npos &&
                line.size() > 1 &&
                line.substr(line.size() - 1) == "\r"
          )
        {
            line = line.substr(0, line.size() - 1);
        }


        return in;
    }

    string removeSpaces(const string &str)
    {
        regex e("\\s");
        return regex_replace(str, e, "");
    }

    Exception getException(const string &msg, const char *file, int line)
    {
        return getException(msg, file, line, "");
    }

    Exception getException(const string &msg, const char *file, int line, const char *function)
    {
        return Exception(0, msg, string(file), line, string(function));
    }

    void createDir(const string &path)
    {
        Path boost_path(path);
        ErrorCode ec;

        cout << "creation dir = " << path << endl;
        if(!exists(boost_path))
        {
            create_directories(boost_path, ec);
            if(ec)
            {
                SystemError es(ec);
                throw ExCreatingDir
                    (
                         es.what(),
                         __FILE__,
                         __LINE__,
                         __FUNCTION__
                    );
            }
        }
        /*else
        {
            ExCreatingDir ex
                    (
                        "Le dossier \"" + path + "\" existe deja",
                        __FILE__,
                        __LINE__,
                        __FUNCTION__
                    );
            Log::w(ex);
        }  */
    }
}

