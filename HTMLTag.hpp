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
/**
 * @brief Cette classe représente une balise HTML. À partir d'un contenu texte
 * HTML, elle analyse et récupère les information d'une balise HTML donnée.
 */
class HTMLTag
{
    public:
        /**
         * @brief L'expression de la regex d'une balise HTML.
         */
        static std::string reg_tag;

    private:
        /* ====================  Data members  ==================== */
        /**
         * @brief Le nom de la balise.
         */
        std::string _name;
        /**
         * @brief La liste des attributs de la balise.
         */
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
        /**
         * @brief Recherche la première occurence de la balise HTML _name.
         *
         * @param str La chaine de caractère dans laquelle chercher.
         *
         * @return Renvoie le texte nom analyser qui vient après la première
         * balise trouvée.
         */
        std::string parse(const std::string &str);
        /**
         * @brief Recherche la première occurence de la balise HTML tagname.
         *
         * @param str La chaine de caractère dans laquelle chercher.
         * @param tagname Le nom de la balise HTML à chercher.
         *
         * @return Renvoie le texte nom analyser qui vient après la première
         * balise trouvée.
         */
        std::string parse(const std::string &str, const std::string &tagname);
        /**
         * @brief Vérifie si un attribut a été trouvée.
         *
         * @param attr Le nom de l'attribut.
         *
         * @return true si l'attribut a été trouvé.
         */
        bool isAttributeExists(std::string attr);


    protected:
        /* ====================  Methods       ==================== */
        /**
         * @brief Analyse une balise.
         *
         * @param str Le contenu HTML à analyser.
         *
         * @return Renvoie le texte nom analyser qui vient après la première
         * balise trouvée.
         */
        std::string parseTag(const std::string &str);
        /**
         * @brief Recherche les tags dans une balise.
         *
         * @param str Le texte contenant la balise à anaylser.
         */
        void parseAttrs(const std::string &str);
};
/* -----************************  end of class  ************************----- \\
   HTMLTag
// -----****************************************************************----- */


#endif
