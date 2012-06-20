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

}
AVRBoard::~AVRBoard() { /*delete blResistor;*/ }

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
    eNode* groundEnode = new eNode(newid);
    ePin*  groundpin   = new ePin( id.append("-ePin").toStdString(), 0);
    id = newid;
    m_ground = new eSource( id.append("-eSource").toStdString(), groundpin );
    //groundEnode->setNodeNumber(0);          // eNode0 = ground
    groundpin->setEnode( groundEnode );

    // Create board led
    newid = m_id;
    newid.append(QString("boardled"));
    LedSmd* boardLed = new LedSmd( this, "LEDSMD", newid, QRectF(0, 0, 4, 3) );
    boardLed->createEpins(2);
    boardLed->setParentItem(this);
    boardLed->setPos( -45, 14/*22*/ );
    boardLed->setEnabled(false);
    boardLed->setMaxCurrent( 0.003 );
    boardLed->setRes( 1000 );
    eNode* boardLedEnode = new eNode(newid);
    ePin*  boardLedEpin0 = boardLed->getEpin(0);
    ePin*  boardLedEpin1 = boardLed->getEpin(1);

    // Connect board led to ground
    boardLedEpin0->setEnode( boardLedEnode );
    boardLedEpin0->setEnodeComp( groundEnode );
    boardLedEpin1->setEnode( groundEnode );
    boardLedEpin1->setEnodeComp( boardLedEnode );

    // Connect board led to pin 13
    ePin* pin13 = m_pinList.at(13)->pin();
    pin13->setEnode( boardLedEnode );
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

