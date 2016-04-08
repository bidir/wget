#ifndef __H_EXCEPTIONS_H__
#define __H_EXCEPTIONS_H__

/*
 * ================================ Header =====================================
 * Filename: Exceptions.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> Exception
 *      |-> ExOpeningFile
 *      |-> ExCreatingDir
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

#define GenEx(x, ...) x(__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)


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
    /* 010 */     CREATE_DIR,
    /* 011 */     INVALID_PROTOCOLE

} ERR;


#include <iostream>
#include <exception>
#include <string>


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
        int _line;
        std::string _file;
        std::string _function;
        std::string _msg;


    public:
        /* ====================  Constructors  ==================== */
        Exception(int code, const std::string &msg, const std::string &file, int line, const std::string &function) throw();
        Exception(int code, const char *msg, const char *file, int line, const char *function) throw();
        virtual ~Exception() throw();


        /* ====================  Accessors     ==================== */
        int getCode() const;
        int getLine() const;
        std::string getFile() const;
        std::string getFunction() const;
        std::string getMessage() const;


        /* ====================  Mutators      ==================== */
        void setLine(int line);
        void setFile(std::string file);
        void setFunction(std::string function);




        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
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
                 int line,
                 const std::string &function
            ) throw():Exception(ERR::OPEN_FILE, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
       ExOpeningFile
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExCreatingDir
    Description:
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExCreatingDir : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExCreatingDir
            (
                 const std::string &msg,
                 const std::string &file,
                 int line,
                 const std::string &function
            ) throw():Exception(ERR::CREATE_DIR, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
       ExCreatingDir
// -----****************************************************************----- */

#endif
