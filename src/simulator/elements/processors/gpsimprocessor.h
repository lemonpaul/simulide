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

#ifndef NO_PIC
  
#ifndef GPSIMPROCESSORXX_H
#define GPSIMPROCESSORXX_H

#include "baseprocessor.h"

class pic_processor;

class GpsimProcessor : public BaseProcessor
{
    Q_OBJECT
    public:
        GpsimProcessor();
        ~GpsimProcessor();
        
 static GpsimProcessor* self() { return m_pSelf; }

        bool loadFirmware( QString file );
        void terminate();

        void step();     // Run 1 step and returns actual source line number
        
        void reset();

        int  getRamValue( QString name );
        
        void readUsart();
        
        
        pic_processor* getCpu() { return m_pPicProcessor; }

    private:
 static GpsimProcessor *m_pSelf;
 
        QHash<QString, int> getRegsTable( QString lstFileName );
        
        pic_processor *m_pPicProcessor;

        bool m_DoneGpsimInit;
        bool m_lastTrmtBit;
};
#endif
#endif
