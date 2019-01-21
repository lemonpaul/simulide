/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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
#include "inbus.h"

Component* InBus::construct( QObject* parent, QString type, QString id )
{
        return new InBus( parent, type, id );
}

LibraryItem* InBus::libraryItem()
{
    return new LibraryItem(
        tr( "In Bus" ),
        tr( "Logic/Other Logic" ),
        "inbus.png",
        "InBus",
        InBus::construct );
}

InBus::InBus( QObject* parent, QString type, QString id )
     : LogicComponent( parent, type, id )
     , eInBus( id.toStdString() )
{    
    m_width  = 1;
    m_height = 8;

    setNumOuts( 8 );                           // Create Output Pins

    LogicComponent::setNumInps( 1 );
    
    m_inPin[0] = new Pin( 0, QPoint( 8, 0 ), m_id+"-out", 1, this );
    
    m_inPin[0]->setIsBus( true );
                          
    eLogicDevice::createInput( m_inPin[0] );
}
InBus::~InBus(){
}

void InBus::setNumOuts( int outs )
{
    if( outs == m_numOutputs ) return;
    if( outs < 1 ) return;

    eLogicDevice::deleteOutputs( m_numOutputs );
    LogicComponent::setNumOuts( outs );
    
    for( int i=0; i<outs; i++ )
    {
        m_outPin[i] = new Pin( 180, QPoint(-8,-8*outs+i*8+8 )
                               , m_id+"-out"+QString::number(i), i, this );

        //m_outPin[i]->setLength( 2 );
        //m_outPin[i]->setFlag( QGraphicsItem::ItemStacksBehindParent, false );
        

        eLogicDevice::createOutput( m_outPin[i] );
    }
    m_maxAddr = pow( 2, m_numOutputs )-1;
    m_maxVolt = m_maxAddr;

    m_height = outs-1;
    m_area = QRect( -2, -m_height*8-4, 4, m_height*8+8 );
}

void InBus::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    p->drawLine( 2, 0, 6, 3 );
    p->drawLine( 2, 0, 6,-3 );

    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    p->drawRect( QRect( 0, -m_height*8, 0, m_height*8 ) );
}

#include "moc_inbus.cpp"
