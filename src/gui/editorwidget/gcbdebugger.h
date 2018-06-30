/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef GCBDEBUGGER_H
#define GCBDEBUGGER_H

#include <QtGui>
#include <QHash>

#include "basedebugger.h"

class GcbDebugger : public BaseDebugger
{
    Q_OBJECT
    public:
        GcbDebugger( QObject* parent, OutPanelText* outPane, QString filePath  );
        ~GcbDebugger();

        bool loadFirmware();

        int  step();     // Run 1 step and returns actual source line number
        int  stepOver();
        int  getValidLine( int line );
        int  compile();

    private:
        void mapGcbToAsm();
        void mapLstToAsm();
        void getProcType();

        QHash<int, int> m_asmToGcb;
        QHash<int, int> m_gcbToAsm;
        QHash<int, int> m_asmToFlash;               // Map .asm code line to flash adress
        QHash<int, int> m_flashToAsm;               // Map flash adress to .asm code line
        
        int m_lastGcbLine;
        int m_processorType;
        
        //QString m_gcBasic;                      // path to gcBasic executable
};


#endif
