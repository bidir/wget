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
#include <boost/lexical_cast.hpp>

#include "tools.hpp"
#include "Log.hpp"


using namespace boost::filesystem; 
typedef boost::filesystem::path Path;
typedef boost::system::error_code ErrorCode;
typedef boost::system::system_error SystemError;


namespace tools
{
    using boost::lexical_cast;
    using boost::bad_lexical_cast;
    void toUpper(string *str)
    {
        transform(str->begin(), str->end(), str->begin(), ::toupper);
    }

    int toInt(const char *str)
    {
        int ret;
        try
        {
            ret = lexical_cast<int>(str);
        }
        catch(bad_lexical_cast &e)
        {
            throw getException(e.what(), __FILE__, __LINE__);
        }
        return ret;
    }

    int toInt(const string &str)
    {
        return toInt(str.c_str());
    }

    unsigned int toUInt(const char *str)
    {
        if(str[0] == '-')
        {
            throw getException(
                                    string(str) + " n'est pas un entier non signe",
                                    __FILE__,
                                    __LINE__
                                );
        }
        unsigned int ret;
        try
        {
            ret = lexical_cast<unsigned int>(str);
        }
        catch(bad_lexical_cast &e)
        {
            throw getException(e.what(), __FILE__, __LINE__);
        }
        return ret;
    }

    unsigned int toUInt(const string &str)
    {
        return toUInt(str.c_str());
    }

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

    void createDir(const char *path)
    {
        Path boost_path(path);
        ErrorCode ec;

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
    }

    void createDir(const string &path)
    {
        createDir(path.c_str());
    }

    bool isDirExists(const char *path)
    {
        return exists(Path(path));
    }

    bool isDirExists(const string &path)
    {
        return isDirExists(path.c_str());
    }

    void printHttpClientInfos(HttpClient &client)
    {
        ostringstream oss;
        oss << "----------- Infos recuperees dans l'en-tete ------------ " << endl;
        if(client.isChunked())
        {
            oss << "chunked = true" << endl;
        }
        else
        {
            oss << "chunked = false" << endl;
        }
        oss << "statut = " << client.getStatus() << endl;
        oss << "message du statut = " << client.getStatusMessage() << endl;
        oss << "Taille des donnees = " << client.getContentLength() << endl;
        oss << "Type des donnees = " << client.getContentType() << endl;
        oss << "Version http = " << client.getHttpVersion() << endl;
        oss << "Unite donnees = " << client.getAcceptRanges() << endl;
        oss << "Location = " << client.getLocation() << endl;
        oss << "Encoding = " << client.getEncoding() << endl;
        if(client.getUnparsedHeader() != "")
        {
            oss << "----------------- Le reste de l'en-tete ---------------- ";
            oss << endl << client.getUnparsedHeader() << endl;
        }
        oss << "-------------------------------------------------------- " << endl;
        Log::i(oss.str());
    }
}
