/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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

#include "sevensegment_bcd.h"
#include "simulator.h"
#include "connector.h"


Component* SevenSegmentBCD::construct( QObject* parent, QString type, QString id )
{
    return new SevenSegmentBCD( parent, type, id );
}

LibraryItem* SevenSegmentBCD::libraryItem()
{
    return new LibraryItem(
        tr( "7 Seg BCD" ),
        tr( "Logic" ),
        "seven_segment.png",
        "7-Seg BCD",
        SevenSegmentBCD::construct );
}

SevenSegmentBCD::SevenSegmentBCD( QObject* parent, QString type, QString id )
        : LogicComponent( parent, type, id ), eLogicDevice( id.toStdString() )
{
    m_width  = 4;
    m_height = 6;

    QStringList pinList;

    pinList // Inputs:
            << "IL06  "
            << "IL04  "
            << "IL02  "
            << "IL00  "
            ;
    init( pinList );

    for( int i=0; i<m_numInPins; i++ )
        eLogicDevice::createInput( m_inPin[i] );
        
    m_outValue.resize( 7 );
    
    Simulator::self()->addToUpdateList( this );
}
SevenSegmentBCD::~SevenSegmentBCD(){}

void SevenSegmentBCD::initialize()
{
    for( int i=0; i<m_numInPins; i++ )
    {
        eNode* enode = m_input[i]->getEpin()->getEnode(); 
        if( enode ) enode->addToChangedFast(this);
    }
    
    for( int i=0; i<7; i++ ) m_outValue[i] = false;
}

void SevenSegmentBCD::setVChanged()
{
    bool a = eLogicDevice::getInputState( 0 );
    bool b = eLogicDevice::getInputState( 1 );
    bool c = eLogicDevice::getInputState( 2 );
    bool d = eLogicDevice::getInputState( 3 );

    int digit = a*1+b*2+c*4+d*8;

    switch( digit)
    {
    case 0:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = false;
        break;
    case 1:
        m_outValue[0] = false;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = false;
        m_outValue[4] = false;
        m_outValue[5] = false;
        m_outValue[6] = false;
        break;
    case 2:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = false;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = false;
        m_outValue[6] = true;
        break;
    case 3:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = false;
        m_outValue[5] = false;
        m_outValue[6] = true;
        break;
    case 4:
        m_outValue[0] = false;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = false;
        m_outValue[4] = false;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 5:
        m_outValue[0] = true;
        m_outValue[1] = false;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = false;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 6:
        m_outValue[0] = true;
        m_outValue[1] = false;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 7:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = false;
        m_outValue[4] = false;
        m_outValue[5] = false;
        m_outValue[6] = false;
        break;
    case 8:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 9:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = false;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 10:
        m_outValue[0] = true;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = false;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 11:
        m_outValue[0] = false;
        m_outValue[1] = false;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 12:
        m_outValue[0] = true;
        m_outValue[1] = false;
        m_outValue[2] = false;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = false;
        break;
    case 13:
        m_outValue[0] = false;
        m_outValue[1] = true;
        m_outValue[2] = true;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = false;
        m_outValue[6] = true;
        break;
    case 14:
        m_outValue[0] = true;
        m_outValue[1] = false;
        m_outValue[2] = false;
        m_outValue[3] = true;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    case 15:
        m_outValue[0] = true;
        m_outValue[1] = false;
        m_outValue[2] = false;
        m_outValue[3] = false;
        m_outValue[4] = true;
        m_outValue[5] = true;
        m_outValue[6] = true;
        break;
    }
}

void SevenSegmentBCD::updateStep()
{
    update();
}

void SevenSegmentBCD::remove()
{
    Simulator::self()->remFromUpdateList( this );
    LogicComponent::remove();
}

void SevenSegmentBCD::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QPen pen;
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);

    p->setPen(pen);
    p->setBrush( QColor( 30, 30, 30 ) );
    p->drawRect( m_area );

    const int mg =  6;// Margin around number
    const int ds =  1; // "Slope"
    const int tk =  4; // Line thick
    const int x1 =  m_area.x()+mg;
    const int x2 = -m_area.x()-mg;
    const int y1 =  m_area.y()+mg;
    const int y2 = -m_area.y()-mg;


     pen.setWidth(tk);
     pen.setColor( QColor( 250, 250, 100));
     p->setPen(pen);

     if( m_outValue[0]) p->drawLine( x1+tk+ds, y1,    x2-tk+ds, y1    );
     if( m_outValue[1]) p->drawLine( x2+ds,    y1+tk, x2,      -tk    );
     if( m_outValue[2]) p->drawLine( x2,       tk,    x2-ds,    y2-tk );
     if( m_outValue[3]) p->drawLine( x2-tk-ds, y2,    x1+tk-ds, y2    );
     if( m_outValue[4]) p->drawLine( x1-ds,    y2-tk, x1,       tk    );
     if( m_outValue[5]) p->drawLine( x1,      -tk,    x1+ds,    y1+tk );
     if( m_outValue[6]) p->drawLine( x1+tk,    0,     x2-tk,    0     );

     /*if( m_point )
     {
         p->setPen( Qt::NoPen );
         p->setBrush( QColor( 250, 250, 100) );
         p->drawPie( x2+ds, y2-ds, tk, tk, 0, 16*360 );
         // Decimal pointn  p->drawPie( x2+ds, y3-ds, 6, 6, 0, 16*360 );
     }*/
}

#include "moc_sevensegment_bcd.cpp"
