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

#include "avrcomponentpin.h"
#include "avrcomponent.h"

#include "sim_irq.h"
#include "sim_io.h"
#include "avr_adc.h"
#include "avr_timer.h"

AVRComponentPin::AVRComponentPin( McuComponent* mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
    : McuComponentPin( mcu, id, type, label, pos, xpos, ypos, angle )
{
    m_channel = -1;
    m_pwmFreq = 0;
    m_pwmDuty = 0;
}
AVRComponentPin::~AVRComponentPin(){}

void AVRComponentPin::attach( avr_t*  AvrProcessor )
{
    m_AvrProcessor = AvrProcessor;

    if( m_id.startsWith("P") )
    {
        m_port = m_id.at(1).toAscii();
        m_pinN = m_id.mid(2,1).toInt();

        avr_ioctl( AvrProcessor, AVR_IOCTL_IOPORT_GETSTATE(m_port), &m_avrPin );

        /*qDebug()  << "NAME" << QChar(int(m_avrPin.name))
                    << "PORT" <<  m_avrPin.port
                    << "DDR" << m_avrPin.ddr
                    << "PIN" << m_avrPin.pin;*/

        set_pinImpedance( m_avrPin.ddr );
        //set_pinVoltage( 0 );

        // Crea IRQ para recibir estado de pin: puerto "m_port", pin numero "m_pinN"
        avr_irq_t* stateIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ( m_port ),  m_pinN );
        // Registra IRQ callback a funcion "out_hook" en clase "this"
        avr_irq_register_notify( stateIrq, out_hook, this );

        // Lo mismo para direccion de pin ( en realidad recibe direcciones de puerto completo )
        avr_irq_t* directionIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ( m_port ), IOPORT_IRQ_DIRECTION_ALL );
        avr_irq_register_notify( directionIrq, ddr_hook, this );


        // Registra IRQ para informar a simavr de estado de pin //avr_irq_pool_t* 	pool; // Esto que es??? en nuevo simavr
        //const char** names;
        m_Write_stat_irq = avr_alloc_irq( 0l, 0, 1, 0l/*names*/ );
        avr_irq_t* writeIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ(m_port), m_pinN );
        avr_connect_irq( m_Write_stat_irq, writeIrq );

        if( m_type.startsWith("adc") ) // Registra IRQ para recibir petiones de voltaje de pin ( usado en ADC )
        {
            m_channel = m_type.right(1).toInt();
            avr_irq_t* adcIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_OUT_TRIGGER );
            avr_irq_register_notify( adcIrq, adc_hook, this );
        }

        if( m_id.startsWith("PD6")  /*&& m_pinLabel->text().contains("PWM")*/ )
        {
            avr_irq_t* i_pwm = avr_io_getirq( AvrProcessor, AVR_IOCTL_TIMER_GETIRQ('0'), TIMER_IRQ_OUT_PWM0 );
            avr_irq_register_notify( i_pwm, pwm_changed_hook, this);
        }
    }
    m_attached = true;
}

void AVRComponentPin::setVChanged()
{
    if(!m_AvrProcessor) return;

    float volt = m_ePin[0]->getVolt();

    //qDebug() << m_type << volt;
    if( m_type == "mclr" )
    {
        ;
        //if( volt > 3 )  m_mcuComponent->setMclr( true );
        //else            m_mcuComponent->setMclr( false );
    }
    else if( m_type == "vcc" )  { if(m_AvrProcessor) m_AvrProcessor->vcc  = volt*1000;}
    else if( m_type == "avcc" ) { if(m_AvrProcessor) m_AvrProcessor->avcc = volt*1000;}
    else if( m_type == "aref" ) { if(m_AvrProcessor) m_AvrProcessor->aref = volt*1000;}
    else
    {
        if( volt  > 2.5 ) avr_raise_irq(m_Write_stat_irq, 1);
        else              avr_raise_irq(m_Write_stat_irq, 0);
    }
}

void AVRComponentPin::set_pinVoltage( uint32_t value )
{
    //qDebug() << "Port" << m_port << m_id << "   estado: " << value;

    setOut( value == 1 );
}

void AVRComponentPin::set_pinImpedance( uint32_t value )
{
    //qDebug() << "Port" << m_port << m_id << "   salida: " << (value and ( 1<<m_pinN ));
    if( value & (1 << m_pinN) )
    {
        setImp( 40 );
        /*if( m_ePin[0]->isConnected())
            m_ePin[0]->getEnode()->remFromChangedList(this);*/
    }
    else
    {
        setImp( high_imp );
        /*if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->addToChangedList(this);*/
    }
    //qDebug()<< m_id << "Port" << m_port << m_pinN << "   salida: " << (value and ( 1<<m_pinN ));
}

void AVRComponentPin::set_pwm( uint32_t value )
{
    m_pwmDuty = value;
    qDebug()<<__FUNCTION__ << m_id << value;
}

void AVRComponentPin::adcread( int channel )
{
    if( m_channel == channel )
    {
        //qDebug() << "ADC Read channel:" << channel << "   pin: " << m_id <<m_ePin[0]->getVolt()*1000 ;
        avr_raise_irq( avr_io_getirq( m_AvrProcessor, AVR_IOCTL_ADC_GETIRQ, channel),
                                      m_ePin[0]->getVolt()*1000 );
    }
}

#include "moc_avrcomponentpin.cpp"
