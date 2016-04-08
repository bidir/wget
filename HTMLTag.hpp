#ifndef __H_HTML_TAG_H__
#define __H_HTML_TAG_H__


/*
 * ================================ Header =====================================
 * Filename: HTMLTag.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:17:12 PM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <string>
#include <map>


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HTMLTag
Description:  
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HTMLTag
{
    private:
        /* ====================  Data members  ==================== */
        std::string _name;
        std::map<std::string, std::string> _attrs;


    public:
        /* ====================  Constructors  ==================== */
        HTMLTag();


        /* ====================  Accessors     ==================== */
        std::string getName();
        std::map<std::string, std::string> getAttributes();
        std::string getAttribute(std::string name);


        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        std::string parse(const std::string &str);
        std::string parse(const std::string &str, const std::string &tagname);
        bool isAttributeExists(std::string attr);


    protected:
        /* ====================  Methods       ==================== */
        std::string parseTag(const std::string &str);
        void parseAttrs(const std::string &str);
};
/* -----************************  end of class  ************************----- \\
   HTMLTag
// -----****************************************************************----- */


#endif
