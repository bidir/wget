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


using namespace std;


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
        string _name;
        map<string, string> _attrs;


    public:
        /* ====================  Constructors  ==================== */
        HTMLTag();


        /* ====================  Accessors     ==================== */
        string getName();
        map<string, string> getAttributes();
        string getAttribute(string name);


        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        string parse(const string &str);
        string parse(const string &str, const string &tagname);
        bool isAttributeExists(string attr);


    protected:
        /* ====================  Methods       ==================== */
        string parseTag(const string &str);
        void parseAttrs(const string &str);
};
/* -----************************  end of class  ************************----- \\
   HTMLTag
// -----****************************************************************----- */


#endif
