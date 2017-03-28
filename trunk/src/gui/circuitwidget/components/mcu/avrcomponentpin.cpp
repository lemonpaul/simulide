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
#include "simulator.h"

AVRComponentPin::AVRComponentPin( McuComponent* mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
    : McuComponentPin( mcu, id, type, label, pos, xpos, ypos, angle )
{
    m_channel = -1;
}
AVRComponentPin::~AVRComponentPin(){}

void AVRComponentPin::attach( avr_t*  AvrProcessor )
{
    m_AvrProcessor = AvrProcessor;

    if( m_id.startsWith("P") )
    {
        m_pinType = 1;
        m_port = m_id.at(1).toLatin1();
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
        //qDebug()  << m_port << m_pinN;
        
        // Lo mismo para direccion de pin ( en realidad recibe direcciones de puerto completo )
        avr_irq_t* directionIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ( m_port ), IOPORT_IRQ_DIRECTION_ALL );
        avr_irq_register_notify( directionIrq, ddr_hook, this );


        // Registra IRQ para informar a simavr de estado de pin //avr_irq_pool_t* 	pool; // Esto que es??? en nuevo simavr
        const char* name = " ";
        m_Write_stat_irq = avr_alloc_irq( &AvrProcessor->irq_pool, 0, 1, &name );
        avr_irq_t* writeIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ(m_port), m_pinN );
        avr_connect_irq( m_Write_stat_irq, writeIrq );
        
        if( m_type.startsWith("adc") ) 
        {
            m_channel = m_type.right(1).toInt();
            m_Write_adc_irq = avr_io_getirq( m_AvrProcessor, AVR_IOCTL_ADC_GETIRQ, m_channel);
        }
    }
    else if( m_type == "reset" ) 
    {
        m_pinType = 21;
        eSource::setImp( high_imp );
    }
    else if( m_type == "vcc"  ) 
    {
        m_pinType = 22;
        eSource::setImp( high_imp );
    }
    else if( m_type == "avcc" ) 
    {
        m_pinType = 23;
        eSource::setImp( high_imp );
    }
    else if( m_type == "aref" )
    {
         m_pinType = 24;
         eSource::setImp( high_imp );
    }
    m_attached = true;
}

void AVRComponentPin::setVChanged()
{
    float volt = m_ePin[0]->getVolt();

    //qDebug() << m_id << m_type << volt;
    if( m_pinType == 1 )                           // Is an IO Pin
    {
        if( volt  > 2.5 ) avr_raise_irq(m_Write_stat_irq, 1);
        else              avr_raise_irq(m_Write_stat_irq, 0);
    }
    else if( m_pinType == 21 ) // reset
    {
        ;
        //if( volt > 3 )  avr_reset( m_AvrProcessor );
        //else            m_mcuComponent->setMclr( false );
    }
    else if( m_pinType == 22 ) { m_AvrProcessor->vcc  = volt*1000;}
    else if( m_pinType == 23 ) { m_AvrProcessor->avcc = volt*1000;}
    else if( m_pinType == 24 ) { m_AvrProcessor->aref = volt*1000;}
}

void AVRComponentPin::set_pinVoltage( uint32_t value )
{
    //qDebug() << "Port" << m_port << m_id << "   estado: " << value;
    if( !(m_ePin[0]->isConnected()) ) return;
    //eSource::setOut( value == 1 );
    //eSource::stampOutput();
    if( value == 1 ) m_voltOut = m_voltHigh;
    else             m_voltOut = m_voltLow;
    m_ePin[0]->stampCurrent( m_voltOut/m_imp );
    if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

void AVRComponentPin::set_pinImpedance( uint32_t value )
{
    //qDebug() << "Port" << m_port << m_id << "   salida: " << (value & ( 1<<m_pinN ));
    
    if( value & (1 << m_pinN) )                         // Pis is Output
    {
        eSource::setImp( 40 );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->remFromChangedFast(this);
    }
    else                                                 // Pin is Input
    {
        eSource::setImp( high_imp );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->addToChangedFast(this);
    }
    //qDebug()<< m_id << "Port" << m_port << m_pinN << "   salida: " << (value and ( 1<<m_pinN ));
}

void AVRComponentPin::adcread()
{
    //qDebug() << "ADC Read channel:    pin: " << m_id <<m_ePin[0]->getVolt()*1000 ;
    avr_raise_irq( m_Write_adc_irq, m_ePin[0]->getVolt()*1000 );
}

#include "moc_avrcomponentpin.cpp"
