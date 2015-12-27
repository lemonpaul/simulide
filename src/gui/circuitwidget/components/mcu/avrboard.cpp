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

#include "avrboard.h"
#include "avrcomponentpin.h"
#include "avrprocessor.h"
#include "e-resistor.h"
#include "itemlibrary.h"

// simavr includes

#include "sim_arduino.h"
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_core.h"

Component* AVRBoard::construct( QObject* parent, QString type, QString id )
{ return new AVRBoard( parent, type,  id ); }

LibraryItem* AVRBoard::libraryItem()
{
    return new LibraryItem(
        tr("Arduino"),
        tr(""),         // Not dispalyed
        "",
        tr("Arduino"),
        AVRBoard::construct );
}

AVRBoard::AVRBoard( QObject* parent, QString type, QString id )
    : McuComponent( parent, type, id )
{
    m_dataFile = "data/arduinos.xml";
    m_processor = AvrProcessor::self();

    if( m_id.split("-").first() == "Arduino" ) m_id.replace( "Arduino", "Arduino Uno" );

    /*connect( m_processor, SIGNAL(chipReset()),
             this,        SLOT  (reset()) );*/

    initPackage();
    initBoard();
    //initBootloader();
}
AVRBoard::~AVRBoard() 
{
    uart_pty_stop(&m_uart_pty);
    delete m_ground;
    delete m_groundEnode;
    delete m_groundpin;
    delete m_boardLed;
    delete m_boardLedEnode; 
}

void AVRBoard::initBootloader()
{
    struct avr_flash flash_data;
    char boot_path[1024] = "data/arduino/ATmegaBOOT_168_atmega328.ihex";
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
    /* end of flash, remember we are writing /code/ */
    avr->codeend = avr->flashend;
    avr->log = 1;
    
    ap->initialized();
    
    uart_pty_init(avr, &m_uart_pty);
    uart_pty_connect(&m_uart_pty, '0');
}

void AVRBoard::initBoard()
{
    for( int i=0; i<m_numpins; i++ )
    {
        McuComponentPin* mcuPin = m_pinList.at(i);

        if( i<16 ) mcuPin->move( 16, 0 );
        else       mcuPin->move(-16, 0 );

        Pin* pin = mcuPin->pin();
        pin->setLength(0);
        pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false );
    }
    // Create ground
    QString newid = m_id;
    newid.append(QString("-Gnod"));
    QString id = newid;
    m_groundEnode = new eNode(newid);
    m_groundpin   = new ePin( id.append("-ePin").toStdString(), 0);
    id = newid;
    m_ground = new eSource( id.append("-eSource").toStdString(), m_groundpin );
    //m_groundEnode->setNodeNumber(0);          // eNode0 = ground
    m_groundpin->setEnode( m_groundEnode );

    // Create board led
    newid = m_id;
    newid.append(QString("boardled"));
    m_boardLed = new LedSmd( this, "LEDSMD", newid, QRectF(0, 0, 4, 3) );
    m_boardLed->setNumEpins(2);
    m_boardLed->setParentItem(this);
    m_boardLed->setPos( 35, 125 );
    m_boardLed->setEnabled(false);
    m_boardLed->setMaxCurrent( 0.003 );
    m_boardLed->setRes( 1000 );
    
    newid.append(QString("-eNode"));
    m_boardLedEnode = new eNode(newid);
    
    ePin*  boardLedEpin0 = m_boardLed->getEpin(0);
    ePin*  boardLedEpin1 = m_boardLed->getEpin(1);

    // Connect board led to ground
    boardLedEpin0->setEnode( m_boardLedEnode );
    boardLedEpin0->setEnodeComp( m_groundEnode );
    boardLedEpin1->setEnode( m_groundEnode );
    boardLedEpin1->setEnodeComp( m_boardLedEnode );

    // Connect board led to pin 13
    ePin* pin13 = m_pinList.at(13)->pin();
    pin13->setEnode( m_boardLedEnode );
}

void AVRBoard::attachPins()
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
}

void AVRBoard::addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
{
    //qDebug()<<pos<<id<<label;
    m_pinList.append( new AVRComponentPin( this, id, type, label, pos, xpos, ypos, angle ) );
}


void AVRBoard::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    int ox = m_area.x();
    int oy = m_area.y();

    p->drawPixmap( ox, oy, QPixmap(":/arduinoUno.png"));
}

#include "moc_avrboard.cpp"

