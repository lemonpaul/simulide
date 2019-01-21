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

#include "resistordip.h"
#include "itemlibrary.h"
#include "connector.h"
#include "circuit.h"
#include "pin.h"


Component* ResistorDip::construct( QObject* parent, QString type, QString id )
{ return new ResistorDip( parent, type, id ); }

LibraryItem* ResistorDip::libraryItem()
{
    return new LibraryItem(
            tr( "ResistorDip" ),
            tr( "Passive" ),
            "resistordip.png",
            "ResistorDip",
            ResistorDip::construct);
}

ResistorDip::ResistorDip( QObject* parent, QString type, QString id )
           : Component( parent, type, id )
{
    m_size = 0;
    setSize( 8 );
    
    m_unit = "Ω";
    setResist( 100 );
    setValLabelX( 5 );
    setValLabelY(-28 );
    setValLabRot( 90 );
    setValLabelPos();
    //m_valLabel->setEnabled( false );
    m_valLabel->setAcceptedMouseButtons(0);
    setShowVal( true );
}
ResistorDip::~ResistorDip(){}

void ResistorDip::createResistors( int c )
{
    int start = m_size;
    m_size = m_size+c;
    m_resistor.resize( m_size );
    m_pin.resize( m_size*2 );

    for( int i=start; i<m_size; i++ )
    {
        int index = i*2;
        QString reid = m_id;
        reid.append(QString("-resistor"+QString::number(i)));
        m_resistor[i] = new eResistor( reid.toStdString() );
        
        QPoint pinpos = QPoint(-16,-32+8+i*8 );
        Pin* pin = new Pin( 180, pinpos, reid+"-pinP", 0, this);
        m_resistor[i]->setEpin( 0, pin );
        m_pin[index] = pin;
        
        pinpos = QPoint( 16,-32+8+i*8 );
        pin = new Pin( 0, pinpos, reid+"-pinN", 0, this);
        m_resistor[i]->setEpin( 1, pin );
        m_pin[index+1] = pin;
    }
    //update();
}

void ResistorDip::deleteResistors( int d )
{
    if( d > m_size ) d = m_size;
    int start = m_size-d;

    for( int i=start*2; i<m_size*2; i++ )
    {
        Pin* pin = m_pin[i];
        if( pin->isConnected() ) pin->connector()->remove();
        
        delete pin;
    }
    for( int i=start; i<m_size; i++ ) delete m_resistor[i];
    m_size = m_size-d;
    m_resistor.resize( m_size );
    m_pin.resize( m_size*2 );
    //Circuit::self()->update();
}

int ResistorDip::size()
{
    return m_size;
}

void ResistorDip::setSize( int size )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();
    
    if( size == 0 ) size = 8;
    
    if     ( size < m_size ) deleteResistors( m_size-size );
    else if( size > m_size ) createResistors( size-m_size );
    
    m_area = QRect( -8, -28, 16, m_size*8 ); 
    
    if( pauseSim ) Simulator::self()->runContinuous();
    Circuit::self()->update();
}

double ResistorDip::resist() { return m_value; }

void ResistorDip::setResist( double r )
{
    Component::setValue( r );       // Takes care about units multiplier
    
    foreach( eResistor* res, m_resistor ) res->setResSafe( m_value*m_unitMult );
}

void ResistorDip::setUnit( QString un ) 
{
    Component::setUnit( un );
    foreach( eResistor* res, m_resistor ) res->setResSafe( m_value*m_unitMult );
}

void ResistorDip::remove()
{
    deleteResistors( m_size );

    Component::remove();
}
void ResistorDip::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    //p->setBrush( QColor( 80, 80, 80) );

    p->drawRoundRect( boundingRect(), 4, 4 );
}

#include "moc_resistordip.cpp"
