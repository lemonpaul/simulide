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

#include "switch.h"
#include "itemlibrary.h"
#include "pin.h"


Component* Switch::construct( QObject* parent, QString type, QString id )
{ return new Switch( parent, type, id ); }

LibraryItem* Switch::libraryItem()
{
    return new LibraryItem(
            tr( "Switch" ),
            tr( "Switches" ),
            "switch.png",
            "Switch",
            Switch::construct);
}

Switch::Switch( QObject* parent, QString type, QString id )
    : SwitchBase( parent, type, id )
{
    m_area =  QRectF( -11, -9, 22, 11 );
    
    m_pin.resize(2);

    QString pinid = m_id;
    pinid.append(QString("-lnod"));
    QPoint pinpos = QPoint(-8-8,0);
    m_pin[0] = new Pin( 180, pinpos, pinid, 0, this);
    m_ePin[0] = m_pin[0];

    pinid = m_id;
    pinid.append(QString("-rnod"));
    pinpos = QPoint(8+8,0);
    m_pin[1] = new Pin( 0, pinpos, pinid, 1, this);
    m_ePin[1] = m_pin[1];
    
    connect( m_button, SIGNAL( clicked() ),
             this,     SLOT  ( onbuttonclicked() ));
}
Switch::~Switch(){}

void Switch::onbuttonclicked()
{
    m_closed = !m_closed;
    m_changed = true;
}

void Switch::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    if( m_closed ) p->drawLine(-10, 0, 10, -2 );
    else           p->drawLine(-10.5, 0, 8, -8 );
}

#include "moc_switch.cpp"
