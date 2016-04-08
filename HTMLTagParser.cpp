/*
 * ================================== CPP ======================================
 * Filename: HTMLTagParser.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:22:43 PM
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */


#include <fstream>
#include <regex>

#include "tools.hpp"
#include "HTMLTagParser.hpp"
#include "ExHTMLTag.hpp"
#include "Log.hpp"


using namespace std;


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HTMLTagParser
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */

/* ====================  Constructors  ==================== */
HTMLTagParser::HTMLTagParser(istream &in):
    _content(""),
    _in(&in),
    _tags_to_parse(),
    _tags()
{}


/* ====================  Accessors     ==================== */
vector<HTMLTag> &HTMLTagParser::getParsedTags()
{
    return _tags;
}

HTMLTag HTMLTagParser::getParsedTag(int n)
{
    return _tags[n];
}

vector<string> &HTMLTagParser::getTagsToParse()
{
    return _tags_to_parse;
}

string HTMLTagParser::getTagToParse(int n)
{
    return _tags_to_parse[n];
}



/* ====================  Methods       ==================== */
void HTMLTagParser::parse()
{
    string file(""), line("");
    while(tools::getline(*_in, line))
    {
        file = file + line;
    }
    _content = file;

    removeComments();

    searchTags();
}

void HTMLTagParser::searchTags()
{
    for(unsigned int i = 0; i < _tags_to_parse.size(); i++)
    {
        string tagname = _tags_to_parse[i];
        string content(_content);
        bool parse = true;
        while(parse)
        {
            HTMLTag tag;
            try
            {
                content = tag.parse(content, tagname);
                _tags.push_back(tag);
            }
            catch(const ExHTMLTag &e)
            {
                parse = false;
                break;
            }
        }
    }
}

void HTMLTagParser::addTagToParse(string tag)
{
    for(unsigned int i = 0; i < _tags_to_parse.size(); i++)
    {
        if(_tags_to_parse[i] == tag)
            return;
    }
    _tags_to_parse.push_back(tools::toUpper(tag));
}

void HTMLTagParser::removeComments()
{
    regex e("<\\!--((?!<\\!--|-->).)*-->");
    _content = regex_replace(_content, e, "");
}
