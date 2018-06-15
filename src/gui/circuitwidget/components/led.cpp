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

#include "led.h"
#include "pin.h"

Component* Led::construct( QObject* parent, QString type, QString id )
{ return new Led( parent, type, id ); }

LibraryItem* Led::libraryItem()
{
    return new LibraryItem(
            tr( "Led" ),
            tr( "Outputs" ),
            "led.png",
            "Led",
            Led::construct);
}

Led::Led( QObject* parent, QString type, QString id )
    : LedBase( parent, type, id )
{
    QString nodid = m_id;
    nodid.append(QString("-lPin"));
    QPoint nodpos = QPoint(-16, 0 );
    Pin* pin = new Pin( 180, nodpos, nodid, 0, this);
    pin->setPos(-8-8,0);
    m_ePin[0] = pin;

    nodid = m_id;
    nodid.append(QString("-rPin"));
    nodpos = QPoint( 16, 0 );
    pin = new Pin( 0, nodpos, nodid, 1, this);
    pin->setPos( 8+8,0);
    m_ePin[1] = pin;
}
Led::~Led(){}


void Led::drawBackground( QPainter *p )
{
    p->drawEllipse( -8, -8, 16, 16 );
}

void Led::drawForeground( QPainter *p )
{
    static const QPointF points[3] = {
        QPointF( 5, 0 ), QPointF(-5,-5 ),QPointF(-5, 5 ) };

    p->drawPolygon( points, 3 );
    p->drawLine( 5,-5, 5, 5 );
    p->drawLine(-8, 0, 8, 0 );
}

#include "moc_led.cpp"
