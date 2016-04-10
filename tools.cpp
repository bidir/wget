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
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/lexical_cast.hpp>

#include "tools.hpp"
#include "Log.hpp"

using namespace std;

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

    string toString(const int &val)
    {
        try
        {
            return lexical_cast<string>(val);
        }
        catch(bad_lexical_cast &e)
        {
            throw GenEx(Exception, 0, e.what());
        }
    }

    string toString(const unsigned int &val)
    {
        try
        {
            return lexical_cast<string>(val);
        }
        catch(bad_lexical_cast &e)
        {
            throw GenEx(Exception, 0, e.what());
        }
    }

    string toString(size_t &val)
    {
        try
        {
            return lexical_cast<string>(val);
        }
        catch(bad_lexical_cast &e)
        {
            throw GenEx(Exception, 0, e.what());
        }
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
            throw GenEx(Exception, 0, e.what());
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
            throw GenEx(Exception, 0, string(str) + " n'est pas un entier non signe");
        }
        unsigned int ret;
        try
        {
            ret = lexical_cast<unsigned int>(str);
        }
        catch(bad_lexical_cast &e)
        {
            throw GenEx(Exception, 0, e.what());
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
                throw GenEx(ExCreatingDir, es.what());
            }
        }
    }

    void createDir(const string &path)
    {
        createDir(path.c_str());
    }

    bool isDirExists(const char *path)
    {
        return exists(Path(path)) && is_directory(Path(path));
    }

    bool isDirExists(const string &path)
    {
        return isDirExists(path.c_str());
    }

    vector<char> ungzip(const char *compr, unsigned int size)
    {
        std::vector<char> compressed = std::vector<char>(compr, compr + size);
        std::vector<char> decompressed = std::vector<char>();

        boost::iostreams::filtering_ostream os;
        os.push(boost::iostreams::gzip_decompressor());
        os.push(boost::iostreams::back_inserter(decompressed));
        boost::iostreams::write(os, &compressed[0], compressed.size());

        return decompressed;
    }
}
