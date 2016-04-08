#ifndef __H_EX_CLIENT_H__
#define __H_EX_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: ExClient.hpp
 *
 * Description: Sont définis dans ce fichier:
 *  ->Les classes:
 *      |-> ExInvalidURL
 *      |-> ExReadSocket
 *      |-> ExCloseConnection
 *
 * Version: 1.0
 * Created: 03/01/16 16:34:05
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include "Exceptions.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExInvalidURL
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExInvalidURL : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExInvalidURL
            (
             const std::string &msg,
             const std::string &file,
             int line,
             const std::string &function
            ) throw():Exception(ERR::INVALID_URL, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
           ExInvalidURL
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExReadSocket
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExReadSocket : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExReadSocket
            (
             const std::string &msg,
             const std::string &file,
             int line,
             const std::string &function
            )
            throw():Exception(ERR::READ_SOCKET, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
           ExReadSocket
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExCloseConnection
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExCloseConnection : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExCloseConnection
            (
             const std::string &msg,
             const std::string &file,
             int line,
             const std::string &function
            )
            throw():Exception(ERR::READ_SOCKET, msg, file, line, function){};
};
/* -----************************  end of class  ************************----- \\
           ExCloseConnection
// -----****************************************************************----- */


#endif
