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
             const std::string &file,
             const std::string &function,
             int line
            ) throw():Exception(code, "Erreur HTTP", file, function, line)
        {};

        ExHttpClient
            (
             int code,
             const ExceptionInfo &info
            ) throw():Exception(code, "Erreur HTTP", info)
        {};


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
                std::string msg,
                const std::string &file,
                const std::string &function,
                int line
            ) throw():Exception(ERR::INVALID_PROTOCOLE, "Le protocole " + msg + " n'est pas pris en charge.", file, function, line)
        {};

        ExInvalidProtocole
            (
                 std::string msg,
                 const ExceptionInfo &info
            ) throw():Exception(ERR::INVALID_PROTOCOLE, "Le protocole " + msg + " n'est pas pris en charge.", info)
        {};

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
