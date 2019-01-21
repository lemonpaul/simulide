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

  
#ifndef PICPROCESSOR_H
#define PICPROCESSOR_H

#include "baseprocessor.h"

#include "registers.h"
#include "hexutils.h"

class pic_processor;
class _RCSTA;

class PicProcessor : public BaseProcessor
{
    Q_OBJECT
    public:
        PicProcessor( QObject* parent=0 );
        ~PicProcessor();
        
 //static PicProcessor* self() { return m_pSelf; }

        bool loadFirmware( QString file );
        void terminate();

        void reset();
        void step(); 
        void stepOne();
        void stepCpu();
        int pc();

        int getRamValue( int address );

        void uartIn( uint32_t value );
        
        pic_processor* getCpu() { return m_pPicProcessor; }

    private:
        virtual int  validate( int address );
        
        double m_ipc;
        double m_pendingIpc;
        
        pic_processor* m_pPicProcessor;
        
        IntelHexProgramFileType m_hexLoader;
        
        _RCSTA* m_rcsta;
};


#endif
