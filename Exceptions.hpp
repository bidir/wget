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


#define OK  0
#define ERR_INVALID_URL 1
#define ERR_READ_SOCKET 2
#define ERR_OUTPUT_FILE 3
#define ERR_HTTP_PARSER 4
#define ERR_OPEN_FILE 5
#define ERR_CREATE_DIR 6
#define ERR_INVALID_PROTOCOLE 6


#include <iostream>
#include <exception>
#include <string>

using namespace std;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: Exception
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class Exception : public exception
{
    private:
        /* ====================  Data members  ==================== */
        int _code;
        int _line;
        string _file;
        string _function;
        string _msg;


    public:
        /* ====================  Constructors  ==================== */
        Exception(int code, const string &msg, const string &file, int line, const string &function) throw();
        virtual ~Exception() throw();


        /* ====================  Accessors     ==================== */
        int getCode() const;
        int getLine() const;
        string getFile() const;
        string getFunction() const;
        string getMessage() const;


        /* ====================  Mutators      ==================== */
        void setLine(int line);
        void setFile(string file);
        void setFunction(string function);




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
                 const string &msg,
                 const string &file,
                 int line,
                 const string &function
            ) throw():Exception(ERR_OPEN_FILE, msg, file, line, function){};
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
                 const string &msg,
                 const string &file,
                 int line,
                 const string &function
            ) throw():Exception(ERR_CREATE_DIR, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
       ExCreatingDir
// -----****************************************************************----- */

#endif
