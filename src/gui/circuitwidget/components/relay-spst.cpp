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

#include "relay-spst.h"
#include "connector.h"


Component* RelaySPST::construct( QObject* parent, QString type, QString id )
{ return new RelaySPST( parent, type, id ); }

LibraryItem* RelaySPST::libraryItem()
{
    return new LibraryItem(
            tr( "Relay SPST" ),
            tr( "Switches" ),
            "relay-spst.png",
            "RelaySPST",
            RelaySPST::construct);
}

RelaySPST::RelaySPST( QObject* parent, QString type, QString id )
         : RelayBase( parent, type, id )
{
    m_area = QRectF( -10, -26, 20, 36 );

    m_switches.resize( 1 );
    m_pin.resize( 2 );

    // Create Resistors
    //for( int i=0; i<1; i++ )
    {
        QString reid = m_id;
        reid.append(QString("-switch"+QString::number(0)));
        m_switches[0] = new eResistor( reid.toStdString() );

        QPoint pinpos = QPoint(-16,-16 );
        Pin* pin = new Pin( 180, pinpos, reid+"-pinP", 0, this);
        m_switches[0]->setEpin( 0, pin );
        m_pin[0] = pin;

        pinpos = QPoint( 16,-16 );
        pin = new Pin( 0, pinpos, reid+"-pinN", 0, this);
        m_switches[0]->setEpin( 1, pin );
        m_pin[1] = pin;
    }
}
RelaySPST::~RelaySPST(){}

void RelaySPST::setSwitch( bool on )
{
    if( on ) m_switches[0]->setRes( 1e-6 );
    else     m_switches[0]->setRes( 1e18 );
    update();
}

void RelaySPST::remove()
{
    if( m_pin[0]->isConnected() ) m_pin[0]->connector()->remove();
    if( m_pin[1]->isConnected() ) m_pin[1]->connector()->remove();

    foreach( eResistor* res, m_switches ) delete res;

    RelayBase::remove();
}



void RelaySPST::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    RelayBase::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    if( m_switches[0]->res() < 1 )                   // switch is closed
        p->drawLine(-10, -16, 10, -18 );
    else                                            // Switch is oppened
        p->drawLine(-10.5, -16, 8, -24 );
}

#include "moc_relay-spst.cpp"
