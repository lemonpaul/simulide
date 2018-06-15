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

#ifndef AVRCOMPONENT_H
#define AVRCOMPONENT_H


#include "mcucomponent.h"
#include "avrcomponentpin.h"
#include "avrprocessor.h"
#include "itemlibrary.h"

struct avr_t;

class AVRComponent : public McuComponent
{
    Q_OBJECT

    public:

        AVRComponent( QObject* parent, QString type, QString id );
        ~AVRComponent();
    
 static Component* construct( QObject* parent, QString type, QString id );
 static LibraryItem * libraryItem();

        int getRamValue( int address );
        
        void adcread( int channel );
        
 static void adc_hook( struct avr_irq_t* irq, uint32_t value, void* param )
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to AVRComponentPin*
            AVRComponent* ptrAVRComponent = reinterpret_cast<AVRComponent*> (param);

            int channel = int( value/524288 );
            ptrAVRComponent->adcread( channel );
        }

    private:
        void attachPins();
        void addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );
        
        QHash<int, AVRComponentPin*> m_ADCpinList;
        
        AvrProcessor m_avr;
};

#endif

