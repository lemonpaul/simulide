/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "toggleswitch.h"
#include "circuit.h"
#include "simulator.h"
#include "itemlibrary.h"

Component* ToggleSwitch::construct( QObject* parent, QString type, QString id )
{ return new ToggleSwitch( parent, type, id ); }

LibraryItem* ToggleSwitch::libraryItem()
{
    return new LibraryItem(
            tr( "Toggle Switch" ),
            tr( "Switches" ),
            "toggleswitch.png",
            "ToggleSwitch",
            ToggleSwitch::construct);
}

ToggleSwitch::ToggleSwitch( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
    , eElement( id.toStdString() )
{
    m_area =  QRectF( -11, -9, 22, 11 );
    m_idLabel->setPos(-12,-24);

    m_pin.resize(3);
    m_ePin.resize(4);

    m_changed = true;
    m_closed = false;

    QString pinid = m_id;
    pinid.append(QString("-lpin0"));
    QPoint pinpos = QPoint( -8-8, 0 );
    m_pin[0] = new Pin( 180, pinpos, pinid, 0, this);
    
    pinid = m_id;
    pinid.append(QString("-lepin0"));
    m_ePin[ 0 ] = new ePin( pinid.toStdString(), 0 );

    pinid = m_id;
    pinid.append(QString("-rpin0"));
    pinpos = QPoint( 8+8, 0 );
    m_pin[1] = new Pin( 0, pinpos, pinid, 1, this);
    m_ePin[1] = m_pin[1];

    pinid = m_id;
    pinid.append(QString("-rpin1"));
    pinpos = QPoint( 8+8, -8 );
    m_pin[2] = new Pin( 0, pinpos, pinid, 2, this);
    m_ePin[2] = m_pin[2];

    pinid = m_id;
    pinid.append(QString("-lepin1"));
    m_ePin[ 3 ] = new ePin( pinid.toStdString(), 3 );

    m_button = new QPushButton( );
    m_button->setMaximumSize( 16,16 );
    m_button->setGeometry(-20,-16,16,16);
    m_button->setCheckable( true );

    m_proxy = Circuit::self()->addWidget( m_button );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint(-8, 4) );
    
    connect( m_button, SIGNAL( clicked() ),
             this,     SLOT  ( onbuttonclicked() ));

    Simulator::self()->addToUpdateList( this );
}
ToggleSwitch::~ToggleSwitch()
{
}

void ToggleSwitch::initialize()
{
    eNode* node = m_pin[0]->getEnode();

    m_ePin[ 0 ]->setEnode( node );
    m_ePin[ 3 ]->setEnode( node );
    
    eNode* node0 = m_ePin[0]->getEnode();
    eNode* node1 = m_ePin[1]->getEnode();
    eNode* node2 = m_ePin[2]->getEnode();
    eNode* node3 = m_ePin[3]->getEnode();
    
    if( node0 ) node0->setSwitched( true );
    if( node1 ) node1->setSwitched( true );
    if( node2 ) node2->setSwitched( true );
    if( node3 ) node3->setSwitched( true );
        
    m_ePin[0]->setEnodeComp( node1 );
    m_ePin[1]->setEnodeComp( node0 );
    m_ePin[2]->setEnodeComp( node3 );
    m_ePin[3]->setEnodeComp( node2 );
    
    //for( int i=0; i<4; i++ ) m_ePin[i]->stampAdmitance( 1 ); // Restart circuit afther switch closed issue

    m_changed = true;
    updateStep();
}

void ToggleSwitch::updateStep()
{
    if( m_changed )
    {
        double admit0 = 0;
        double admit1 = 1e3;

        if( m_closed ) 
        {
            admit0 = 1e3;
            admit1 = 0;
        }

        m_ePin[0]->stampAdmitance( admit0 );
        m_ePin[1]->stampAdmitance( admit0 );
        m_ePin[2]->stampAdmitance( admit1 );
        m_ePin[3]->stampAdmitance( admit1 );

        m_changed = false;
    }
}

void ToggleSwitch::remove()
{
    Simulator::self()->remFromUpdateList( this );

    Component::remove();
}

void ToggleSwitch::onbuttonclicked()
{
    m_closed = !m_closed;
    m_changed = true;
}

void ToggleSwitch::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    if( m_closed ) p->drawLine(-10, 0, 10, -2 );
    else           p->drawLine(-10.5, 0, 8, -8 );
}
#include "moc_toggleswitch.cpp"
