/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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
 ***************************************************************************/

#ifndef AVRDEBUGGER_H
#define AVRDEBUGGER_H

#include <QtGui>
#include <QHash>

#include "basedebugger.h"

// simavr includes
#include "sim_avr.h"

struct avr_t;

class AvrDebugger : public BaseProcessor
{
    Q_OBJECT
    public:
        AvrDebugger();
        ~AvrDebugger();

        QString getFileName() { return m_symbolFile; }

        bool loadFirmware( QString file );

        int  step();     // Run 1 step and returns actual source line number
        int  stepOver();
        void reset();

        int getValidLine( int line );
        int getSourceLine();
        int getRamValue( QString name );
        int getRegAddress( QString name );
        void terminate();
        QStringList getDefsList();

    private:
        void mapLstToAsm();
        void setRegisters();
        void getProcName();

        QString m_device;

        //From simavr
        avr_t *m_avrProcessor;
};


#endif
