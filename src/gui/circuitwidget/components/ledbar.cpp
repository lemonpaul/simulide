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

#include "ledbar.h"
#include "connector.h"
#include "pin.h"


Component* LedBar::construct( QObject* parent, QString type, QString id )
{ return new LedBar( parent, type, id ); }

LibraryItem* LedBar::libraryItem()
{
    return new LibraryItem(
            tr( "LedBar" ),
            tr( "Outputs" ),
            "ledbar.png",
            "LedBar",
            LedBar::construct);
}

LedBar::LedBar( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
{
    m_led.resize( 8 );
    //m_pin.resize( 16 );
    
    // Create Leds
    for( int i=0; i<8; i++ )
    {
        QString ledid = m_id;
        ledid.append(QString("-led"+QString::number(i)));
        m_led[i] = new LedSmd( this, "LEDSMD", ledid, QRectF(0, 0, 4, 4) );
        m_led[i]->setParentItem(this);
        m_led[i]->setPos( 0, -28+2+i*8 );
        m_led[i]->setEnabled( false );
        m_led[i]->setAcceptedMouseButtons(0);
        
        QPoint nodpos = QPoint(-16,-32+8+i*8 );
        Pin* pin = new Pin( 180, nodpos, ledid+"-pinP", 0, this);
        m_led[i]->setEpin( 0, pin );
        //m_pin[i] = pin;
        
        nodpos = QPoint( 16,-32+8+i*8 );
        pin = new Pin( 0, nodpos, ledid+"-pinN", 0, this);
        m_led[i]->setEpin( 1, pin );
        //m_pin[8+i] = pin;
    }
    setRes( 0.6 ); 
}
LedBar::~LedBar(){}

void LedBar::setColor( LedBase::LedColor color ) 
{ 
    foreach( LedSmd* led, m_led )
        led->setColor( color ); 
}

LedBase::LedColor LedBar::color() 
{ 
    return m_led[0]->color(); 
}

double LedBar::threshold()                     
{ 
    return m_led[0]->threshold(); 
}

void LedBar::setThreshold( double threshold ) 
{ 
    for( int i=0; i<8; i++ ) m_led[i]->setThreshold( threshold );
}

double LedBar::maxCurrent()                   
{ 
    return m_led[0]->maxCurrent(); 
}
void LedBar::setMaxCurrent( double current ) 
{ 
    for( int i=0; i<8; i++ ) m_led[i]->setMaxCurrent( current ); 
}

double LedBar::res() { return m_led[0]->res(); }

void LedBar::setRes( double resist )
{
    if( resist == 0 ) resist = 1e-14;

    for( int i=0; i<8; i++ ) m_led[i]->setRes( resist );
}

bool LedBar::grounded()
{
    return m_led[0]->grounded();
}

void LedBar::setGrounded( bool grounded )
{
    for( int i=0; i<8; i++ ) m_led[i]->setGrounded( grounded );
}

void LedBar::remove()
{
    for( int i=0; i<8; i++ ) m_led[i]->remove();
    
    Component::remove();
}
void LedBar::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    p->setBrush( QColor( 0, 0, 0) );

    p->drawRoundRect( boundingRect(), 4, 4 );
}

#include "moc_ledbar.cpp"
