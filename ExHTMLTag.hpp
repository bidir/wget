#ifndef __H_EX_HTML_TAG_H__
#define __H_EX_HTML_TAG_H__

/*
 * ================================ Header =====================================
 * Filename: ExHTMLTag.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/09/2016 12:38:20 AM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include "Exceptions.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: ExHTMLTag
    Description:  
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class ExHTMLTag : public Exception
{
    public:
        /* ====================  Constructors  ==================== */
        ExHTMLTag
            (
                 const std::string &msg,
                 const std::string &file,
                 const std::string &function,
                 int line
            ) throw():Exception(ERR::HTTP_PARSER, msg, file, function, line)
        {};

        ExHTMLTag
            (
                 const std::string &msg,
                 const ExceptionInfo &info
            ) throw():Exception(ERR::HTTP_PARSER, msg, info)
        {};
};
/* -----************************  end of class  ************************----- \\
           ExHTMLTag
// -----****************************************************************----- */

#endif
