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
        tr( "Logic/Gates" ),
        "buffer.png",
        "Buffer",
        Buffer::construct );
}

Buffer::Buffer( QObject* parent, QString type, QString id )
      : Gate( parent, type, id, 1 )
{
    m_area = QRect( -19, -17, 38, 34 ); 
    
    m_outEnPin = new Pin( 90, QPoint( 0,-16 ), m_id+"-ePin-outEnable", 0, this );
    eLogicDevice::createOutEnablePin( m_outEnPin );
    
    setTristate( false );
}
Buffer::~Buffer(){}

void Buffer::setTristate( bool t )
{
    if( !t ) 
    {
        if( m_outEnPin->isConnected() ) m_outEnPin->connector()->remove();
        m_outEnPin->reset();
    }
    m_outEnPin->setVisible( t );
    m_tristate = t;
    eLogicDevice::updateOutEnabled();
}

void Buffer::remove()
{
    if( m_tristate )
        if( m_outEnPin->isConnected() ) m_outEnPin->connector()->remove();
    
    Gate::remove();
}

QPainterPath Buffer::shape() const
{
    QPainterPath path;
    
    QVector<QPointF> points;
    
    points << QPointF(-17,-17 )
           << QPointF(-17, 17 )
           << QPointF( 17,  1 )
           << QPointF( 17, -1 );
        
    path.addPolygon( QPolygonF(points) );
    path.closeSubpath();
    return path;
}

void Buffer::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
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

    p->drawPolygon( points, 4 );
}

#include "moc_buffer.cpp"

