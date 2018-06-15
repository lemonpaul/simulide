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
    m_resistor.resize( 8 );
    m_pin.resize( 16 );
    
    // Create Resistors
    for( int i=0; i<8; i++ )
    {
        QString reid = m_id;
        reid.append(QString("-resistor"+QString::number(i)));
        m_resistor[i] = new eResistor( reid.toStdString() );
        
        QPoint pinpos = QPoint(-16,-32+8+i*8 );
        Pin* pin = new Pin( 180, pinpos, reid+"-pinP", 0, this);
        m_resistor[i]->setEpin( 0, pin );
        m_pin[i] = pin;
        
        pinpos = QPoint( 16,-32+8+i*8 );
        pin = new Pin( 0, pinpos, reid+"-pinN", 0, this);
        m_resistor[i]->setEpin( 1, pin );
        m_pin[8+i] = pin;
    }
    
    m_unit = "Ω";
    setResist( 100 );
    setValLabelX( 7 );
    setValLabelY(-20 );
    setValLabRot( 90 );
    setValLabelPos();
    m_valLabel->setEnabled( false );
    m_valLabel->setAcceptedMouseButtons(0);
    setShowVal( true );
}
ResistorDip::~ResistorDip(){}

double ResistorDip::resist() { return m_value; }

void ResistorDip::setResist( double r )
{
    Component::setValue( r );       // Takes care about units multiplier
    
    for( int i=0; i<8; i++ ) m_resistor[i]->setResSafe( m_value*m_unitMult );
}

void ResistorDip::setUnit( QString un ) 
{
    Component::setUnit( un );
    for( int i=0; i<8; i++ ) m_resistor[i]->setResSafe( m_value*m_unitMult );
}

void ResistorDip::remove()
{
    for( int i=0; i<16; i++ )
        {if( m_pin[i]->isConnected() ) 
            {(static_cast<Pin*>(m_pin[i]))->connector()->remove();}}

    foreach (eResistor* res, m_resistor ) delete res;

    Component::remove();
}
void ResistorDip::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    //p->setBrush( QColor( 80, 80, 80) );

    p->drawRoundRect( boundingRect(), 4, 4 );
}

#include "moc_resistordip.cpp"
