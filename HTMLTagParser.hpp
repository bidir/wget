#ifndef __H_HTML_TAG_PARSER_H__
#define __H_HTML_TAG_PARSER_H__


/*
 * ================================ Header =====================================
 * Filename: HTMLTagParser.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:22:51 PM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <iostream>
#include <vector>

#include "HTMLTag.hpp"


using namespace std;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HTMLTagParser
Description:  
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class HTMLTagParser
{
    private:
        /* ====================  Data members  ==================== */
        string _content;
        istream *_in;

        vector<string> _tags_to_parse;
        vector<HTMLTag> _tags;


    public:
        /* ====================  Constructors  ==================== */
        HTMLTagParser(istream &in);


        /* ====================  Accessors     ==================== */
        vector<HTMLTag> &getParsedTags();
        HTMLTag getParsedTag(int n);
        vector<string> &getTagsToParse();
        string getTagToParse(int n);


        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        void addTagToParse(string tag);
        void parse();
        void removeComments();


    protected:
        /* ====================  Methods       ==================== */
        string readNextTag();
        bool parseHead(const string &file);
        void searchTags();
};
/* -----************************  end of class  ************************----- \\
   HTMLTagParser
// -----****************************************************************----- */


#endif
