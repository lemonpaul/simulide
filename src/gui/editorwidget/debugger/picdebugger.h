/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/

#ifndef PICDEBUGGER_H
#define PICDEBUGGER_H

#include "basedebugger.h"

class pic_processor;

class PicDebugger: public BaseDebugger
{
    Q_OBJECT
    public:
        PicDebugger();
        ~PicDebugger();

        QString getFileName() { return m_symbolFile; }

        bool loadFirmware( QString file );

        int  step();     // Run 1 step and returns actual source line number
        int  stepOver();
        //void run();     // thread
        void reset();

        int getValidLine( int line );
        int getSourceLine();    // returns actual source line number
        int getRamValue( QString name );
        int getRegAddress( QString name );
        QStringList getDefsList();

    private:
        void setRegisters();
        
        pic_processor *m_pPicProcessor;

        bool m_DoneGpsimInit;
};
#endif

