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
    /*regex e(".*(<head>.*</head>).*(<body>.*</body>).*");
    smatch m;

    if(regex_search(file, m, e))
    {
        string head(m.str(1));
        //string body(m.str(2));
        //_body = body;
        _head = head;
    }  */
    _content = file;

    removeComments();

    ofstream out;
    out.open("/tmp/toto.html");
    if(out.fail())
    {
        Log::w(ExOpeningFile
                (
                     "Erreur pendant l'ouverture du fichier de sortie",
                     __FILE__,
                     __LINE__,
                     __FUNCTION__
                ));
    }
    else
    {
        out << _content;
        out.close();
    }

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
                Log::w(e);
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
    _tags_to_parse.push_back(tag);
}

void HTMLTagParser::removeComments()
{
    regex e("<\\!--((?!<\\!--|-->).)*-->");
    _content = regex_replace(_content, e, "");
}
