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

#include "buffer.h"
#include "itemlibrary.h"


Component* Buffer::construct( QObject* parent, QString type, QString id )
{
        return new Buffer( parent, type, id );
}

LibraryItem* Buffer::libraryItem()
{
    return new LibraryItem(
        tr( "Buffer" ),
        tr( "Logic" ),
        "buffer.png",
        "Buffer",
        Buffer::construct );
}

Buffer::Buffer( QObject* parent, QString type, QString id )
        : Gate( parent, type, id, 1 )
{
}
Buffer::~Buffer(){}

void Buffer::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    QPen pen = p->pen();
    pen.setWidth(2);
    p->setPen(pen);

    static const QPointF points[4] = {
        QPointF(-16,-16 ),
        QPointF(-16, 16 ),
        QPointF( 16,  1 ),
        QPointF( 16, -1 )             };

    p->drawPolygon(points, 4);

    Gate::paint( p, option, widget );            // draw inverted circle
}

#include "moc_buffer.cpp"

