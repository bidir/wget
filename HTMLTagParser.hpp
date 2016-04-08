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
        std::string _content;
        std::istream *_in;

        std::vector<std::string> _tags_to_parse;
        std::vector<HTMLTag> _tags;


    public:
        /* ====================  Constructors  ==================== */
        HTMLTagParser(std::istream &in);


        /* ====================  Accessors     ==================== */
        std::vector<HTMLTag> &getParsedTags();
        HTMLTag getParsedTag(int n);
        std::vector<std::string> &getTagsToParse();
        std::string getTagToParse(int n);


        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        void addTagToParse(std::string tag);
        void parse();
        void removeComments();


    protected:
        /* ====================  Methods       ==================== */
        std::string readNextTag();
        bool parseHead(const std::string &file);
        void searchTags();
};
/* -----************************  end of class  ************************----- \\
   HTMLTagParser
// -----****************************************************************----- */


#endif
