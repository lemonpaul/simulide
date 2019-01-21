/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#include "connector.h"
#include "circuit.h"
#include "bus.h"

Component* Bus::construct( QObject* parent, QString type, QString id )
{
        return new Bus( parent, type, id );
}

LibraryItem* Bus::libraryItem()
{
    return new LibraryItem(
        tr( "Bus" ),
        tr( "Logic/Other Logic" ),
        "outbus.png",
        "Bus",
        Bus::construct );
}

Bus::Bus( QObject* parent, QString type, QString id )
   : LogicComponent( parent, type, id )
   , eBus( id.toStdString() )
{    
    m_width  = 1;
    m_height = 8;

    setNumInps( 8 );                           // Create Input Pins

    LogicComponent::setNumOuts( 1 );
    
    m_outPin[0] = new Pin( 0, QPoint( 8, 0 ), m_id+"-out", 1, this );
                          
    eLogicDevice::createOutput( m_outPin[0] );
    
    m_outPin[0]->setIsBus( true );
}
Bus::~Bus(){
}

void Bus::setNumInps( int inputs )
{
    if( inputs == m_numInputs ) return;
    if( inputs < 1 ) return;

    eLogicDevice::deleteInputs( m_numInputs );
    LogicComponent::setNumInps( inputs );
    
    for( int i=0; i<inputs; i++ )
    {
        m_inPin[i] = new Pin( 180, QPoint(-8,-8*inputs+i*8+8 )
                               , m_id+"-in"+QString::number(i), i, this );

        //m_inPin[i]->setLength( 2 );
        //m_inPin[i]->setFlag( QGraphicsItem::ItemStacksBehindParent, false );

        eLogicDevice::createInput( m_inPin[i] );
    }
    m_maxAddr = pow( 2, m_numInputs )-1;
    m_maxVolt = m_maxAddr;

    m_height = inputs-1;
    m_area = QRect( -2, -m_height*8-4, 4, m_height*8+8 );
}

void Bus::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    
    if( Circuit::self()->animate() )
    {
        QPen pen = p->pen();

        if( m_driving )
        {
            pen.setColor( QColor( 200, 50, 50 ) );
            p->setPen(pen);
            p->drawLine( 7, 0, 3, 3 );
            p->drawLine( 7, 0, 3,-3 );
        }
        else
        {
            pen.setColor( QColor( 50, 50, 200 ) );
            p->setPen(pen);
            p->drawLine( 1, 0, 5, 3 );
            p->drawLine( 1, 0, 5,-3 );
        }
        pen.setColor( Qt::black );
        p->setPen(pen);
    }

    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    p->drawRect( QRect( 0, -m_height*8, 0, m_height*8 ) );
}

#include "moc_bus.cpp"
