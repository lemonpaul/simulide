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

#include "avrcomponentpin.h"
#include "simulator.h"

AVRComponentPin::AVRComponentPin( McuComponent* mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
    : McuComponentPin( mcu, id, type, label, pos, xpos, ypos, angle )
{
    m_channel = -1;
    m_isInput = true;
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
        
        // PORTX Register change irq
        QString portName = "PORT";
        portName.append( m_id.at(1) );
        int portAddr = BaseProcessor::self()->getRegAddress( portName );
        if( !portAddr )
        {
            qDebug()  << tr("Register descriptor file for this AVR processor %1 is corrupted - cannot attach pin").arg(AvrProcessor->mmcu)
                      << portName << m_pinN
                         ;
            return;
        }
        m_PortRegChangeIrq = avr_iomem_getirq( AvrProcessor,
                                                portAddr,
                                                &m_port,
                                                m_pinN );
                                                        
        avr_irq_register_notify( m_PortRegChangeIrq, port_reg_hook, this );
        
        // By now need this for pwm to work
        m_PortChangeIrq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ( m_port ),  m_pinN );
        avr_irq_register_notify( m_PortChangeIrq, port_hook, this );
        
        // DDRX Register change irq
        QString ddrName = "DDR";
        ddrName.append( m_id.at(1) );
        int ddrAddr = BaseProcessor::self()->getRegAddress( ddrName );
        if( !ddrAddr )
        {
            qDebug()  << tr("Register descriptor file for this AVR processor %1 is corrupted - cannot attach pin \n").arg(AvrProcessor->mmcu)
                      << ddrName << m_pinN
                         ;
            return;
        }
        m_DdrRegChangeIrq = avr_iomem_getirq( AvrProcessor,
                                                        ddrAddr,
                                                        " ",
                                                        m_pinN );
                                                        
        avr_irq_register_notify( m_DdrRegChangeIrq, ddr_hook, this );
        
        //qDebug()  << m_port << m_pinN;
        
        m_Write_stat_irq = avr_io_getirq( AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ(m_port), m_pinN );
        /*m_Write_stat_irq = avr_alloc_irq(&AvrProcessor->irq_pool, 0, 1, NULL);
        avr_connect_irq(
                m_Write_stat_irq,
                avr_io_getirq(AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ(m_port), m_pinN));*/
        
        if( m_type.startsWith("adc") ) 
        {
            m_channel = m_type.right(1).toInt();
            m_Write_adc_irq = avr_io_getirq( m_AvrProcessor, AVR_IOCTL_ADC_GETIRQ, m_channel);
        }

#ifdef AVR_IOPORT_INTRN_PULLUP_IMP
        // If we wish to take full control over pull-ups,
        // we can turn off the simavr internall pull-ups support.
        avr_irq_t *src_imp_irq = avr_io_getirq( m_AvrProcessor, AVR_IOCTL_IOPORT_GETIRQ(m_port), m_pinN + IOPORT_IRQ_PIN0_SRC_IMP);
        avr_raise_irq_float(src_imp_irq, 0, true);
        // Otherwise it is active and can "override" our handling in some situations.
        // Verify tests/pullup_disconnect/pullup_disconnect.ino with digitalWrite(1, x) lines uncommented
#endif //AVR_IOPORT_INTRN_PULLUP_IMP
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

void AVRComponentPin::initialize() 
{
    if( m_pinType == 1 )                         // Initialize irq flags
    {
        if( m_PortRegChangeIrq && m_DdrRegChangeIrq ) 
        {
            m_PortChangeIrq->flags |= IRQ_FLAG_INIT;
            m_PortRegChangeIrq->flags |= IRQ_FLAG_INIT;
            m_DdrRegChangeIrq->flags |= IRQ_FLAG_INIT;
        } 
        else 
        {
            qDebug()  << tr("Pin is not initialized properly:")
                      << m_port << m_pinN;
        }
    }
    McuComponentPin::initialize();
}

void AVRComponentPin::setVChanged()
{
    float volt = m_ePin[0]->getVolt();

    //qDebug() << m_id << m_type << volt;
    if( m_pinType == 1 )                                 // Is an IO Pin
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

void AVRComponentPin::setPullup( uint32_t value )
{
    if( !m_isInput ) return;
    
    //qDebug() << "Port" << m_port << m_id << "   pullup: " << (value>0);

    if( value>0 )                         // Activate pullup
    {
        eSource::setImp( 1e5 );
        m_voltOut = m_voltHigh;
    }
    else                                 // Deactivate pullup
    {
        eSource::setImp( high_imp );
        m_voltOut = m_voltLow;
    }
    if( !(m_ePin[0]->isConnected()) ) {
        avr_raise_irq( m_Write_stat_irq, (value>0)? 1:0 );
        return;
    }
    
    m_ePin[0]->stampCurrent( m_voltOut/m_imp );
    if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

void AVRComponentPin::set_pinVoltage( uint32_t value )
{
    if( m_isInput ) return;
    //qDebug() << "Port" << m_port << m_id << "   estado: " << value;

    //if( m_isInput ) setPullup( value>0 ); // Activate pullup when port is written while input

    if( value > 0 ) m_voltOut = m_voltHigh;
    else            m_voltOut = m_voltLow;
    
    if( !(m_ePin[0]->isConnected()) ) return;
    
    //eSource::setOut( value >0 );
    //eSource::stampOutput();
    m_ePin[0]->stampCurrent( m_voltOut/m_imp ); // Save some calls
    if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

void AVRComponentPin::set_pinImpedance( uint32_t value )
{
    //qDebug() << "Port" << m_port << m_id << "   salida: " << (value > 0 );
    
    if( value > 0 )                         // Pis is Output
    {
        m_isInput = false;
        eSource::setImp( 40 );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->remFromChangedFast(this);
    }
    else                                                 // Pin is Input
    {
        m_isInput = true;
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
