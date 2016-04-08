/*
 * ================================== CPP ======================================
 * Filename: Exceptions.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/01/16 16:17:54
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */

#include <sstream>
#include "Exceptions.hpp"


using namespace std;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: Exception
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */


/* ====================  Constructors  ==================== */
Exception::Exception
    (
         int code,
         const string &msg,
         const string &file,
         int line,
         const string &function
    ) throw():
    _code(code),
    _line(line),
    _file(file),
    _function(function),
    _msg(msg)
{}
Exception::Exception
    (
        int code,
        const char *msg,
        const char *file,
        int line,
        const char *function
        ) throw():
    _code(code),
    _line(line),
    _file(file),
    _function(function),
    _msg(msg)
{}

Exception::~Exception() throw()
{}


/* ====================  Accessors     ==================== */
int Exception::getCode() const
{
    return _code;
}

int Exception::getLine() const
{
    return _line;
}

string Exception::getFile() const
{
    return _file;
}

string Exception::getFunction() const
{
    return _function;
}

string Exception::getMessage() const
{
    return _msg;
}


/* ====================  Mutators      ==================== */
void Exception::setLine(int line)
{
    _line = line;
}

void Exception::setFile(string file)
{
    _file = file;
}

void Exception::setFunction(string function)
{
    _function = function;
}


/* ====================  Methods       ==================== */
const char* Exception::what() const throw()
{
    ostringstream oss;
    oss << "[" << _file << ":" << _line << "|" << _function << "] " << _msg;
    return oss.str().c_str();
}

/* -----************************  end of class  ************************----- \\
       Exception
// -----****************************************************************----- */



