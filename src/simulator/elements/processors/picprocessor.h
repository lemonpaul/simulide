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

#ifndef NO_PIC
  
#ifndef PICPROCESSOR_H
#define PICPROCESSOR_H

#include "baseprocessor.h"
#include "gpsim/pic-processor.h"
#include "gpsim/registers.h"
#include "gpsim/uart.h"
#include "gpsim/pir.h"

class RegBitSink;

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
        void stepOne();
        void step(); 
        int pc();

        int getRamValue( int address );

        void uartIn( uint32_t value );
        
        void readUsart();
        
        void bitChange( QString regName, int bit, bool value );
        
        pic_processor* getCpu() { return m_pPicProcessor; }

    private:
 
        virtual int  validate( int address );
        
        int m_cpi;
        
        pic_processor* m_pPicProcessor;

        bool m_DoneGpsimInit;
        bool m_lastTrmtBit;
        
        _RCREG* m_rcReg;
        _TXSTA* m_txsta;
        PIR* m_pir;
        
        //RegBitSink* m_tmrtBitSink;
};

class RegBitSink : public BitSink  // BitSink inform us about bit changes in a register
{

    public:
        RegBitSink(PicProcessor* picProcessor, QString name, int bit );
        ~RegBitSink();

        void setSink(bool b);  // Called by gpsim when bit changes
      
    private:
        PicProcessor* m_picProcessor;
        
        QString m_regName;
        int     m_bit;
};
#endif
#endif
