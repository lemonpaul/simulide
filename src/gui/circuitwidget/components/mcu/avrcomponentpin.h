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

#ifndef AVRCOMPONENTPIN_H
#define AVRCOMPONENTPIN_H

#include <qstring.h>
#include <stdint.h>

#include "mcucomponentpin.h"

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_irq.h"
//#include "avr_adc.h"

//class AVRComponent;

class AVRComponentPin : public McuComponentPin
{
    Q_OBJECT
	public:
        AVRComponentPin( McuComponent *mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );
		~AVRComponentPin();

        void attach( avr_t * AvrProcessor );

        void set_pinVoltage( uint32_t value );
        void set_pinImpedance( uint32_t value );
        void set_pwm( uint32_t value );

        static void out_hook( struct avr_irq_t* irq, uint32_t value, void* param )
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to a new pointer variable
            AVRComponentPin* ptrAVRComponentPin = reinterpret_cast<AVRComponentPin*> (param);

            ptrAVRComponentPin->set_pinVoltage(value);
        }

        static void ddr_hook( struct avr_irq_t* irq, uint32_t value, void* param )
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to a new pointer variable
            AVRComponentPin * ptrAVRComponentPin = reinterpret_cast<AVRComponentPin *> (param);

            ptrAVRComponentPin->set_pinImpedance(value);
        }

        static void adc_hook( struct avr_irq_t* irq, uint32_t value, void* param )
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to a new pointer variable
            AVRComponentPin* ptrAVRComponentPin = reinterpret_cast<AVRComponentPin*> (param);

            int channel = int( value/524288 );
            ptrAVRComponentPin->adcread( channel );
        }

        static void pwm_changed_hook(struct avr_irq_t * irq, uint32_t value, void * param)
        {
            Q_UNUSED(irq);
            // get the pointer out of param and asign it to a new pointer variable
            AVRComponentPin* ptrAVRComponentPin = reinterpret_cast<AVRComponentPin*> (param);

            ptrAVRComponentPin->set_pwm(value);
        }

        void adcread( int channel );
        void resetOutput();

        void setVChanged();
		
    protected:
        char m_port;
        int  m_pinN;
        int  m_channel;
        int  m_pwmFreq;
        int  m_pwmDuty;

        const QString m_esp;


        //from simavr
        //AVRComponent       *m_pAVRComponent;
        avr_ioport_state_t  m_avrPin;
        avr_t              *m_AvrProcessor;
        avr_irq_t          *m_Write_stat_irq;
        //avr_irq_t          *m_Write_adc_irq;
};

#endif
