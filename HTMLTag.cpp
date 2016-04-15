/*
 * ================================== CPP ======================================
 * Filename: HTMLTag.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/08/2016 12:16:54 PM
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
#include <regex>

#include "tools.hpp"
#include "HTMLTag.hpp"
#include "ExHTMLTag.hpp"


using namespace std;
using namespace regex_constants;


string HTMLTag::reg_tag = "(\\w*)\\s*=\\s*\"([^\"<>]*(\\\\\"|\\\\<|\\\\>)*[^\"<>]*)\"";


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: HTMLTag
Description:  
// |....----------------------------------------------------------------....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */

/* ====================  Constructors  ==================== */
HTMLTag::HTMLTag():
    _name(""),
    _attrs()
{}


/* ====================  Accessors     ==================== */
string HTMLTag::getName()
{
    return _name;
}

map<string, string> HTMLTag::getAttributes()
{
    return _attrs;
}

string HTMLTag::getAttribute(string name)
{
    return _attrs[tools::toUpper(name)];
}



/* ====================  Methods       ==================== */
string HTMLTag::parse(const string &str)
{
    smatch m;
    regex e("^.*<(\\w*)\\s");
    if(!regex_search(str, m, e))
    {
        throw GenEx(ExHTMLTag, "\"" + str + "\" ne contient pas de balise.");
    }
    string name_str(m.str(1));
    _name = tools::toUpper(name_str);
    return parseTag(str);
}

string HTMLTag::parse(const string &str, const string &tagname)
{
    _name = tools::toUpper(tagname);
    return parseTag(str);
}

string HTMLTag::parseTag(const string &str)
{
    regex e("<" + _name + "\\s+(([^>])*)\\s*/?>", regex_constants::icase);
    smatch m;

    if(!regex_search(str, m, e))
    {
        throw GenEx(ExHTMLTag, "\"" + str + "\" n'est pas bien pour le balise \"" + _name + "\"");
    }

    string attrs(m.str(1));
    parseAttrs(attrs);
    string retour(m.suffix().str());
    return retour;
}

void HTMLTag::parseAttrs(const string &str)
{
    if(tools::removeSpaces(str) == "")
    {
        return;
    }
    string s = str;
    regex e1(reg_tag);
    smatch m;

    while(regex_search(s, m, e1))
    {
        string name_str(m.str(1)), val_str(m.str(2));
        _attrs[tools::toUpper(name_str)] = val_str;
        s = m.suffix().str();
    }
}

bool HTMLTag::isAttributeExists(string attr)
{
    if(_attrs.find(attr) == _attrs.end())
    {
        return false;
    }
    return true;
}
