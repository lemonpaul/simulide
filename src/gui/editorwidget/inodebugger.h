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
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef INODEBUGGER_H
#define INODEBUGGER_H

#include <QHash>

#include "basedebugger.h"

class InoDebugger : public BaseDebugger
{
    Q_OBJECT
    public:
        InoDebugger( QObject* parent, OutPanelText* outPane, QString filePath  );
        ~InoDebugger();

        //QString getFileName() { return m_symbolFile; }

        bool loadFirmware();

        int  step();     // Run 1 step and returns actual source line number
        int  stepOver();
        int getValidLine( int line );
        
        int compile();
        
    private:
        void mapInoToFlash();

        QHash<int, int> m_inoToFlash;               // Map .ino code line to flash adress
        QHash<int, int> m_flashToIno;               // Map flash adress to .ino code line
        
        int m_lastInoLine;
        int m_processorType;
        
        QString m_arduino;                      // path to arduino executable
};


#endif
