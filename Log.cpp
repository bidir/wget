/*
 * ================================== CPP ======================================
 * Filename: Log.cpp
 *
 * Description: none 
 *
 * Version: 1.0
 * Created: 03/04/2016 16:58:03
 * Revision: none
 *
 * Commants: none
 *
 * Compiler: gcc
 *
 * Author: Idir BOUCHENEB (Etudiant L3 Info), idir.boucheneb@etu.univ-lyon1.fr
 * =============================================================================
 */

#include <sstream>
#include <fstream>

#include "tools.hpp"
#include "Log.hpp"

/* ////////////////////////////////////|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
// |....oooooooOOOO000000000000000000000000000000000000000000OOOOooooooo....| \\
// |....oooooooOOOO00000********°°°°°^^^^^°°°°°********000000OOOOooooooo....| \\
// |....---------------|             class             |----------------....| \\
Class: Log
// |....----------------------------------------------------------------....| \\
// |....°°°°°°°OOOOOOOOO00000000000000000000000000000000OOOOOOOOO°°°°°°°....| \\
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|///////////////////////////////////// */

/* ====================  Data members  ==================== */
string Log::_e = "E";
string Log::_w = "W";
string Log::_i = "I";
string Log::_d = "D";
mutex Log::_m_write;
bool Log::_log = false;
vector<ostream *> Log::_out(0);


/* ====================  Methods       ==================== */
void Log::init()
{
    _m_write.lock();
    _log = true;
    for(unsigned int i = 0; i < _out.size(); i++)
    {
        *_out[i] << "================================================" << endl
            << "000000000000000000000000000000000000000000000000" << endl
            << "------------- " << tools::getCurrentTime() << " --------------" << endl
            << "000000000000000000000000000000000000000000000000" << endl
            << "================================================" << endl;
    }
    _m_write.unlock();
}

void Log::add(ostream &out)
{
    _out.push_back(&out);
}

void Log::stop()
{
    _log = false;
}

void Log::write(const string &msg)
{
    _m_write.lock();
    for(unsigned int i = 0; i < _out.size(); i++)
    {
        *_out[i] << endl << msg << endl;
    }
    _m_write.unlock();
}

void Log::print(const string &label, const Exception &ex)
{
    if(_log)
    {
        ostringstream oss;
        oss << "[" << label
            << "-" << ex.getCode() << "]"
            << "[" << ex.getFile() << ":" << ex.getLine() << "]"
            << "["  << ex.getFunction() << "]"
            << "[" << tools::getCurrentTime() << "]" << endl
            << ex.getMessage();
        write(oss.str());
        //thread th(Log::write, oss.str());
    }
}

void Log::print(const string &label, const string &msg)
{
    if(_log)
    {
        ostringstream oss;
        oss << "[" << label << "]"
            << "[" << tools::getCurrentTime() << "]" << endl
            << msg;
        write(oss.str());
        //thread th(Log::write, oss.str());
    }
}

void Log::e(const string &msg)
{
    print(_e, msg);
}

void Log::e(const Exception &ex)
{
    print(_e, ex);
}

void Log::w(const string &msg)
{
    print(_w, msg);
}

void Log::w(const Exception &ex)
{
    print(_w, ex);
}

void Log::i(const string &msg)
{
    print(_i, msg);
}

void Log::i(const Exception &ex)
{
    print(_i, ex);
}

void Log::d(const string &msg)
{
    print(_d, msg);
}

void Log::d(const Exception &ex)
{
    print(_d, ex);
}
