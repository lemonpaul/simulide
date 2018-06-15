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

#include "mainwindow.h"
#include "circuit.h"
#include "e-resistor.h"
#include "itemlibrary.h"
#include "arduino.h"

LibraryItem* Arduino::libraryItem()
{
    return new LibraryItem(
        tr("Arduino"),
        tr("Micro"),   
        "arduinoUnoIcon.png",
        tr("Arduino"),
        Arduino::construct );
}

Component* Arduino::construct( QObject* parent, QString type, QString id )
{ 
    if( m_canCreate ) 
    {
        Arduino* ard = new Arduino( parent, type,  id );
        if( m_error > 0 )
        {
            Circuit::self()->compList()->removeOne( ard );
            ard->deleteLater();
            ard = 0l;
            m_error = 0;
            m_canCreate = true;
        }
        return ard;
    }
    QMessageBox* msgBox = new QMessageBox( MainWindow::self() );
    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
    msgBox->setStandardButtons( QMessageBox::Ok );
    msgBox->setWindowTitle( tr("Error") );
    msgBox->setText( tr("Only 1 Mcu allowed\n to be in the Circuit.") );
    msgBox->setModal( false ); 
    msgBox->open();

    return 0l;
}

Arduino::Arduino( QObject* parent, QString type, QString id )
    : McuComponent( parent, type, id )
{
    m_pSelf = this;
    m_dataFile = "arduinos.xml";
    m_processor = AvrProcessor::self();
    
    setLabelPos( 100,-21, 0); // X, Y, Rot
    
    setTransformOriginPoint( boundingRect().center() );

    //if( m_id.split("-").first() == "Arduino" ) m_id.replace( "Arduino", "Arduino Uno" );
    

    initPackage();
    if( m_error == 0 )
    {
        initBoard();
        setFreq( 16 );
        //initBootloader();
        
        qDebug() <<"     ..."<<m_id<<"OK\n";
    }
    else
    {
        qDebug() <<"     ..."<<m_id<<"Error!!!\n";
    }
}
Arduino::~Arduino() 
{
}

void Arduino::remove()
{
    if( Simulator::self()->isRunning() ) CircuitWidget::self()->powerCircOff();
    m_pb5Pin->setEnode( 0l );
    
    Circuit::self()->compList()->removeOne( m_boardLed );
    Simulator::self()->remFromUpdateList( m_boardLed );
    
    delete m_ground;
    delete m_groundpin;
    delete m_boardLed;
    
    Simulator::self()->remFromEnodeList( m_groundEnode, true );
    Simulator::self()->remFromEnodeList( m_boardLedEnode, true );

    McuComponent::remove();
}


void Arduino::initBootloader()
{
    
    /*struct avr_flash flash_data;
    char boot_path[1024] = "data/arduino/ATmegaBOOT_168_atmega328.hex";
    uint32_t boot_base, boot_size;
    
    avr_t* avr = avr_make_mcu_by_name("atmega328p");
    AvrProcessor *ap = dynamic_cast<AvrProcessor*>( m_processor );
    ap->setCpu( avr );
    
    uint8_t* boot = read_ihex_file(boot_path, &boot_size, &boot_base);
    if (!boot)
        fprintf(stderr, "Unable to load %s\n", boot_path);
    
    flash_data.avr_flash_fd = 0;
    // register our own functions
    avr->custom.init = avr_special_init;
    avr->custom.deinit = avr_special_deinit;
    avr->custom.data = &flash_data;
    avr_init(avr);
    avr->frequency = 16000000;
    
    memcpy(avr->flash + boot_base, boot, boot_size);
    free(boot);
    avr->pc = boot_base;
    // end of flash, remember we are writing /code/ 
    avr->codeend = avr->flashend;
    avr->log = 1;
    
    ap->initialized();
    
    attachPins();
    //reset();
    
    uart_pty_init(avr, &m_uart_pty);
    uart_pty_connect(&m_uart_pty, '0');
    if (!m_uart_pty.run) //thread)
    {
        ///TODO: shade connection symbol to indicate it is not active
    }*/
}

void Arduino::initialize()
{
        eNode* enod = m_pb5Pin->getEnode();
        
        if( !enod )                        // Not connected: Create boardLed eNode
        {
            m_boardLedEnode = new eNode( m_id+"-boardLedeNode" );
            enod = m_boardLedEnode;
            m_pb5Pin->setEnode( m_boardLedEnode );
        }
        else if( enod != m_boardLedEnode ) // Connected to external eNode: Delete boardLed eNode
        {
            Simulator::self()->remFromEnodeList( m_boardLedEnode, true );
            m_boardLedEnode = 0l;
        }
        else return;                       // Already connected to boardLed eNode: Do nothing
        
        m_boardLed->getEpin(0)->setEnode(enod);
}

void Arduino::initBoard()
{
    // Create Led ground
    m_groundEnode = new eNode(    m_id+"-Gnod");
    m_groundpin   = new ePin(    (m_id+"-Gnod-ePin_ground").toStdString(), 0);
    m_ground      = new eSource( (m_id+"-Gnod-eSource_ground").toStdString(), m_groundpin );
    m_groundpin->setEnode( m_groundEnode );

    // Create board led
    m_boardLed = new LedSmd( this, "LEDSMD", m_id+"boardled", QRectF(0, 0, 4, 3) );
    m_boardLed->setNumEpins(2);
    m_boardLed->setParentItem(this);
    m_boardLed->setPos( 35, 125 );
    m_boardLed->setEnabled(false);
    m_boardLed->setMaxCurrent( 0.003 );
    m_boardLed->setRes( 1000 );
    
    //m_boardLedEnode = new eNode( m_id+"-boardLedeNode" );
    m_boardLedEnode = 0l;
    
    //ePin* boardLedEpin0 = m_boardLed->getEpin(0);
    ePin* boardLedEpin1 = m_boardLed->getEpin(1);

    // Connect board led to ground
    //boardLedEpin0->setEnode( m_boardLedEnode );
    boardLedEpin1->setEnode( m_groundEnode );

    for( int i=0; i<m_numpins; i++ )                      // Create Pins
    {
        McuComponentPin* mcuPin = m_pinList.at(i);

        if( mcuPin->angle() == 0 ) mcuPin->move(-16, 0 );
        else                       mcuPin->move( 16, 0 );

        Pin* pin = mcuPin->pin();
        pin->setLength(0);
        pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false );
        
        QString pinId = pin->itemID();
        QString type  = mcuPin->ptype();
        if     ( pinId.contains( "GND" ) )                   // Gnd Pins
        {    
            mcuPin->setImp( 0.01 ); 
        }
        else if( pinId.contains( "V3V" ) )                  // 3.3V Pins
        {
            mcuPin->setImp( 0.1 );
            mcuPin->setVoltHigh( 3.3 );
            mcuPin->setVoltLow( 3.3 );
        }
        else if( pinId.contains( "V5V" ) )                    // 5V Pins
        {
            mcuPin->setImp( 0.1 );
            mcuPin->setVoltHigh( 5 );
            mcuPin->setVoltLow( 5 );
        }
        else if( pinId.contains( "Vin" ) )          // Vin Pins ( 12 V )
        {
            mcuPin->setImp( 0.1 );
            mcuPin->setVoltHigh( 12 );
            mcuPin->setVoltLow( 12 );
        }
        else if( type.contains( "led" ) )                      // Pin 13 
        {
            //pin->setEnode( m_boardLedEnode );
            m_pb5Pin = pin;
        }
    }
}

void Arduino::attachPins()
{
    AvrProcessor *ap = dynamic_cast<AvrProcessor*>( m_processor );
    avr_t *cpu = ap->getCpu();

    for( int i = 0; i < m_numpins; i++ )
    {
        AVRComponentPin *pin = dynamic_cast<AVRComponentPin*>( m_pinList[i] );
        pin->attach( cpu );
    }
    cpu->vcc  = 5000;
    cpu->avcc = 5000;
    
    // Registra IRQ para recibir petiones de voltaje de pin ( usado en ADC )
    avr_irq_t* adcIrq = avr_io_getirq( cpu, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_OUT_TRIGGER );
    avr_irq_register_notify( adcIrq, adc_hook, this );

    m_attached = true;
    
    //uart_pty_t m_uart_pty;
    /*uart_pty_init(cpu, &m_uart_pty);
    uart_pty_connect(&m_uart_pty, 0, '0');
    
    if (!m_uart_pty.run) //thread)
    {
        qDebug() << "Arduino::attachPins, Failed to run uart_pty";
    }*/
}

void Arduino::addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
{
    //qDebug()<<pos<<id<<label;
    AVRComponentPin*  newPin = new AVRComponentPin( this, id, type, label, pos, xpos, ypos, angle );
    m_pinList.append( newPin );
    
    if( type.startsWith("adc") )m_ADCpinList[type.right(1).toInt()] = newPin;
}

void Arduino::adcread( int channel )
{
    //qDebug() << "ADC Read channel:" << channel;
    AVRComponentPin* pin = m_ADCpinList.value(channel);
    if( pin ) pin->adcread();
}

void Arduino::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    int ox = m_area.x();
    int oy = m_area.y();

    p->drawPixmap( ox, oy, QPixmap( m_BackGround ));
}

#include "moc_arduino.cpp"
