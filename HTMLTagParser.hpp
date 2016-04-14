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
/**
 * @brief Cette classe est une boite à outils pour analyser un contenu HTML.
 * Elle permet de constuire et de chercher des balises HTML.
 */
class HTMLTagParser
{
    private:
        /* ====================  Data members  ==================== */
        /**
         * @brief Le texte à analyser.
         */
        std::string _content;
        /**
         * @brief Le flux contenant le HTML à analyser.
         */
        std::istream *_in;

        /**
         * @brief La liste des balises à chercher.
         */
        std::vector<std::string> _tags_to_parse;
        /**
         * @brief La liste des balises trouvées.
         */
        std::vector<HTMLTag> _tags;


    public:
        /* ====================  Constructors  ==================== */
        /**
         * @brief Un constructeur
         *
         * @param in Le flux contenant le texte HTML à analyser.
         */
        HTMLTagParser(std::istream &in);


        /* ====================  Accessors     ==================== */
        /**
         * @brief Récupérer les balises trouvées.
         *
         * @return La liste des balises trouvées.
         */
        std::vector<HTMLTag> &getParsedTags();
        /**
         * @brief Récupérer une balise trouvée.
         *
         * @param n L'indice de _tags
         *
         * @return Une balise trouvée.
         */
        HTMLTag getParsedTag(int n);
        std::vector<std::string> &getTagsToParse();
        std::string getTagToParse(int n);


        /* ====================  Mutators      ==================== */



        /* ====================  Operators     ==================== */



        /* ====================  Methods       ==================== */
        /**
         * @brief Ajouter le nom d'une balise HTML à chercher.
         *
         * @param tag Le nom de la balise HTML.
         */
        void addTagToParse(std::string tag);
        /**
         * @brief Lancer l'analyse du contenu HTML.
         */
        void parse();
        /**
         * @brief Supprimer les commentaire HTML du contenu à analyser.
         */
        void removeComments();


    protected:
        /* ====================  Methods       ==================== */
        void searchTags();
};
/* -----************************  end of class  ************************----- \\
   HTMLTagParser
// -----****************************************************************----- */


#endif
