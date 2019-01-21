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

#ifndef ARDUINO_H
#define ARDUINO_H

#include "avrcomponentpin.h"
#include "mcucomponent.h"
#include "avrprocessor.h"
#include "ledsmd.h"
#include "e-source.h"
#include "e-node.h"
#include "itemlibrary.h"


class Arduino : public McuComponent
{
    Q_OBJECT

    public:

        Arduino( QObject* parent, QString type, QString id );
        ~Arduino();

 static Component* construct( QObject* parent, QString type, QString id );
 static LibraryItem * libraryItem();
 
        void initialize();

        int getRamValue( int address );
        
        void adcread( int channel );
        
        void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );
        
        static void adc_hook( struct avr_irq_t* irq, uint32_t value, void* param )
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to AVRComponentPin*
            Arduino* ptrArduino = reinterpret_cast<Arduino*> (param);

            int channel = int( value/524288 );
            ptrArduino->adcread( channel );
        }
        
    public slots:
        virtual void remove();

    private:
        void attachPins();
        void initBoard();
        void addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );

        eSource* m_ground;
        eNode*   m_groundEnode;
        ePin*    m_groundpin;
        LedSmd*  m_boardLed;
        eNode*   m_boardLedEnode;
        Pin*     m_pb5Pin;
        
        QHash<int, AVRComponentPin*> m_ADCpinList;
        
        AvrProcessor m_avr;
};

#endif
