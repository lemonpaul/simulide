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

#include "potentiometer.h"
#include "connector.h"
#include "circuit.h"
#include "itemlibrary.h"


Component* Potentiometer::construct( QObject* parent, QString type, QString id )
{
    return new Potentiometer( parent, type, id );
}

LibraryItem* Potentiometer::libraryItem()
{
    return new LibraryItem(
        tr( "Potentiometer" ),
        tr( "Passive" ),
        "potentiometer.png",
        "Potentiometer",
        Potentiometer::construct );
}

Potentiometer::Potentiometer( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
    , eElement( (id+"-eElement").toStdString() )
    , m_pinA( 180, QPoint(-16,0 ), id+"-PinA", 0, this )
    , m_pinM( 270, QPoint( 0,16), id+"-PinM", 0, this )
    , m_pinB(   0, QPoint( 16,0 ), id+"-PinB", 0, this )
    , m_ePinA( (id+"-ePinA").toStdString(), 0 )
    , m_ePinB( (id+"-ePinB").toStdString(), 1 )
    , m_resA(  (id+"-resA").toStdString() )
    , m_resB(  (id+"-resB").toStdString() )
{
    m_area = QRectF( -11, -4.5, 22, 9 );
    
    m_dialW.setupWidget();
    m_dialW.setFixedSize( 24, 24 );
    m_dialW.dial->setMinimum(0);
    m_dialW.dial->setMaximum(1000);
    m_dialW.dial->setValue(500);
    m_dialW.dial->setSingleStep(25);
    
    m_proxy = Circuit::self()->addWidget( &m_dialW );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint( -12, -24-5) );
    //m_proxy->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent, true );

    m_dial = m_dialW.dial;
    
    m_resA.setEpin( 0, &m_pinA );
    m_resA.setEpin( 1, &m_ePinA );
    
    m_resB.setEpin( 0, &m_pinB );
    m_resB.setEpin( 1, &m_ePinB );

    m_unit = "Ω";
    setRes(1000);
    setValLabelPos( 10,-20, 0);
    setShowVal( true );
    resChanged( 500 );
    
    Simulator::self()->addToUpdateList( this );

    connect( m_dial, SIGNAL(valueChanged(int)),
             this,   SLOT  (resChanged(int)) );
}

Potentiometer::~Potentiometer() 
{
}

void Potentiometer::initialize()
{
    eNode* enod = m_pinM.getEnode(); // Get eNode from middle Pin
    
    m_ePinA.setEnode(enod); // Set eNode to internal eResistors ePins
    m_ePinB.setEnode(enod);
    m_changed = true;
    updateStep();
}

void Potentiometer::updateStep()
{
    if( m_changed ) 
    {
        double res = m_dial->value();

        m_resA.setRes( res );
        m_resB.setRes( m_resist-res );
        
        m_changed = false;
    }
}

void Potentiometer::resChanged( int res ) // Called when dial is rotated
{
    //qDebug() << res << m_resist;
    m_changed = true;
}

void Potentiometer::setRes( double res ) // Called when property resistance is changed
{
    Component::setValue( res );       // Takes care about units multiplier
    m_resist = m_value*m_unitMult;
    m_dial->setMaximum( int(m_resist) );
    
    m_changed = true;
}

void Potentiometer::setUnit( QString un ) 
{
    Component::setUnit( un );
    m_resist = m_value*m_unitMult;
    m_dial->setMaximum( int(m_resist) );

    m_changed = true;
}

void Potentiometer::setVal( int val )
{
    m_dial->setValue( val );
    //resChanged( val );
}

int Potentiometer::val()
{
    return m_dial->value();
}

void Potentiometer::remove()
{
    if( m_pinA.isConnected() ) m_pinA.connector()->remove();
    if( m_pinM.isConnected() ) m_pinM.connector()->remove();
    if( m_pinB.isConnected() ) m_pinB.connector()->remove();
    
    Simulator::self()->remFromUpdateList( this );
    
    Component::remove();
}

void Potentiometer::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    //p->setBrush(Qt::white);
    //p->drawRoundedRect( QRect( 0, 0, 48, 48 ), 1, 1 );
    //p->setBrush(Qt::darkGray);
    //p->fillRect( QRect( 3, 3, 45, 45 ), Qt::darkGray );

    
    //p->drawRoundedRect( QRect( 8, -56, 8, 40 ), 1, 1 );

    Component::paint( p, option, widget );
    p->drawRect( -10.5, -4, 21, 8 );
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    p->drawLine( 0, 6, -4, 10 );
    p->drawLine( 0, 6,  4, 10 );
}

#include "moc_potentiometer.cpp"


