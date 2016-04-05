#ifndef __H__log_H__
#define __H__log_H__

/*
 * ================================ Header =====================================
 * Filename: Log.hpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/04/2016 16:58:09
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
#include <map>
#include <string>
#include <thread>
#include <mutex>

#include "Exceptions.hpp"


/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
    Class: Log
    Description:
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */
class Log
{
    private:
        /* ====================  Data members  ==================== */
        static bool _log;

        static string _e;
        static string _w;
        static string _i;
        static string _d;

        static mutex _m_write;

        static ostream *_d_out;
        static vector<ostream *> _out;

        /* ====================  Methods       ==================== */
    public:
        static void init();
        static void add(ostream &out);
        static void setDebugOut(ostream &out);
        static void stop();
        static void write(const string &msg);
        static void writeD(const string &msg);
        static void print(const string &label, const Exception &ex);
        static void print(const string &label, const string &msg);
        static void e(const string &msg);
        static void e(const Exception &ex);
        static void w(const string &msg);
        static void w(const Exception &ex);
        static void i(const string &msg);
        static void i(const Exception &ex);
        static void d(const string &msg);
        static void d(const Exception &ex);
};
/* -----************************  end of class  ************************----- \\
       Log
// -----****************************************************************----- */


#endif
