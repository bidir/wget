#ifndef __H_EX_HTTP_CLIENT_H__
#define __H_EX_HTTP_CLIENT_H__

/*
 * ================================ Header =====================================
 * Filename: ExHttpClient.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 13/03/2016 17:50:31
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (idirux), idirux.ouchen@gmail.com
 * =============================================================================
 */


#include "Exceptions.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: ExHttpClient
    Description: 
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExHttpClient : public Exception
{
    private:
        /* ====================  Data members  ==================== */


    public:

        /* ====================  Constructors  ==================== */
        ExHttpClient
            (
             int code,
             const string &file,
             int line,
             const string &function
            ) throw():Exception(code, "Erreur HTTP", file, line, function){};


        /* ====================  Accessors     ==================== */



        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */



    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
        ExHttpClient   
// -----****************************************************************----- */



/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
// |....°°°°°°°°°°°°°°°                                 °°°°°°°°°°°°°°°°....| \\
    Class: ExInvalidProtocole
    Description: 
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExInvalidProtocole : public Exception
{
    private:
        /* ====================  Data members  ==================== */


    public:

        /* ====================  Constructors  ==================== */
        ExInvalidProtocole
            (
                 string msg,
                 const string &file,
                 int line,
                 const string &function
            ) throw():Exception(ERR_INVALID_PROTOCOLE, "Le Protocole " + msg + " n'est pas pris en charge.", file, line, function){};


        /* ====================  Accessors     ==================== */



        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */



    protected:

        /* ====================  Methods       ==================== */

};
/* -----************************  end of class  ************************----- \\
        ExInvalidProtocole   
// -----****************************************************************----- */
#endif
