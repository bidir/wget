#ifndef __H_EXCEPTIONS_H__
#define __H_EXCEPTIONS_H__

/*
 * ================================ Header =====================================
 * Filename: Exceptions.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> ExceptionInfo
 *      |-> Exception
 *      |-> ExOpeningFile
 *      |-> ExFilesystem
 *      |-> ExNullPointer
 *  ->Les constantes:
 *      |-> Les codes d'erreurs.
 *
 * Version: 1.0
 * Created: 03/01/16 16:18:02
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */

#define EXINFO ExceptionInfo(__FILE__, __FUNCTION__, __LINE__)
#define GenEx(x, ...) x(__VA_ARGS__, EXINFO)
#define AddTrace(x) x.addInfo(EXINFO)


typedef enum
{
    /* 000 */     OK,
    /* 001 */     NOT_OK,
    /* 002 */     WAR,
    /* 003 */     INF,
    /* 004 */     DEB,
    /* 005 */     INVALID_URL,
    /* 006 */     READ_SOCKET,
    /* 007 */     OUTPUT_FILE,
    /* 008 */     HTTP_PARSER,
    /* 009 */     OPEN_FILE,
    /* 010 */     FILE_SYSTEM,
    /* 011 */     INVALID_PROTOCOLE,
    /* 012 */     NULL_POINTER

} ERR;


#include <iostream>
#include <vector>
#include <exception>
#include <string>



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExceptionInfo
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExceptionInfo
{
    private:
        /* ====================  Data members  ==================== */
        int _line;
        std::string _function;
        std::string _file;


    public:

        /* ====================  Constructors  ==================== */
        ExceptionInfo(const ExceptionInfo &info) throw();
        ExceptionInfo(const std::string &file, const std::string &function, int line) throw();
        ExceptionInfo(const char *file, const char *function, int line) throw();
        virtual ~ExceptionInfo() throw();


        /* ====================  Accessors     ==================== */
        int getLine();
        const int &getLine() const;
        std::string getFile();
        const std::string getFile() const;
        std::string getFunction();
        const std::string getFunction() const;


        /* ====================  Mutators      ==================== */
        void setLine(int line);
        void setFile(std::string file);
        void setFunction(std::string function);


        /* ====================  Operators     ==================== */


        /* ====================  Methods       ==================== */



    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
       ExceptionInfo
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: Exception
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class Exception : public std::exception
{
    private:
        /* ====================  Data members  ==================== */
        int _code;
        std::string _msg;
        ExceptionInfo _info;

        std::vector<ExceptionInfo> _traces;


    public:
        /* ====================  Constructors  ==================== */
        Exception(int code, const std::string &msg, const ExceptionInfo &info) throw();
        Exception(int code, const std::string &msg, const std::string &file, const std::string &function, int line) throw();
        Exception(int code, const char *msg, const char *file, const char *function, int line) throw();
        virtual ~Exception() throw();


        /* ====================  Accessors     ==================== */
        int getCode() const;
        ExceptionInfo &getTrace(unsigned int i);
        const ExceptionInfo &getTrace(unsigned int i) const;
        std::vector<ExceptionInfo> &getTraces();
        const std::vector<ExceptionInfo> &getTraces() const;
        const std::string &getMessage() const;
        ExceptionInfo &getInfo();
        const ExceptionInfo &getInfo() const;


        /* ====================  Mutators      ==================== */


        /* ====================  Operators     ==================== */


        /* ====================  Methods       ==================== */
        void addInfo(const ExceptionInfo &info);
        virtual const char* what() const throw();
        static inline void init(Exception &ex);


    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
       Exception
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExOpeningFile
    Description:
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExOpeningFile : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExOpeningFile
            (
                const std::string &msg,
                const std::string &file,
                const std::string &function,
                int line
            ) throw():Exception(ERR::OPEN_FILE, msg, file, function, line)
        {};

        ExOpeningFile
            (
                const std::string &msg,
                const ExceptionInfo &info
            ) throw():Exception(ERR::OPEN_FILE, msg, info)
        {};
};
/* -----************************  end of class  ************************----- \\
       ExOpeningFile
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExFilesystem
    Description:
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExFilesystem : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExFilesystem
            (
                const std::string &msg,
                const std::string &file,
                const std::string &function,
                int line
            ) throw():Exception(ERR::FILE_SYSTEM, msg, file, function, line)
        {};

        ExFilesystem
            (
                const std::string &msg,
                const ExceptionInfo &info
            ) throw():Exception(ERR::FILE_SYSTEM, msg, info)
        {};
};
/* -----************************  end of class  ************************----- \\
       ExFilesystem
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExNullPointer
    Description:
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExNullPointer : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExNullPointer
            (
                const std::string &msg,
                const std::string &file,
                const std::string &function,
                int line
            ) throw():Exception(ERR::NULL_POINTER, msg, file, function, line)
        {};

        ExNullPointer
            (
                const std::string &msg,
                const ExceptionInfo &info
            ) throw():Exception(ERR::NULL_POINTER, msg, info)
        {};
};
/* -----************************  end of class  ************************----- \\
       ExNullPointer
// -----****************************************************************----- */

#endif
