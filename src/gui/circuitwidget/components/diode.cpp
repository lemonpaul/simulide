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

#include "diode.h"
#include "connector.h"
#include "itemlibrary.h"


Component* Diode::construct( QObject* parent, QString type, QString id )
{ return new Diode( parent, type, id ); }

LibraryItem* Diode::libraryItem()
{
    return new LibraryItem(
            tr( "Diode" ),
            tr( "Active" ),
            "diode.png",
            "Diode",
            Diode::construct);
}

Diode::Diode( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eDiode( id.toStdString() )
{
    m_ePin.resize(2);
    QString nodid = m_id;
    nodid.append(QString("-lPin"));
    QPoint nodpos = QPoint(-16, 0 );
    m_ePin[0] = new Pin( 180, nodpos, nodid, 0, this ); // pPin

    nodid = m_id;
    nodid.append(QString("-rPin"));
    nodpos = QPoint( 16, 0 );
    m_ePin[1] = new Pin( 0, nodpos, nodid, 1, this ); // nPin
}
Diode::~Diode(){}

void Diode::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    Component::remove();
}


void Diode::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    p->setBrush( Qt::black );

    static const QPointF points[3] = {
        QPointF( 7, 0 ),
        QPointF(-8,-7 ),
        QPointF(-8, 7 )              };

   p->drawPolygon(points, 3);

   QPen pen = p->pen();
   pen.setWidth(3);
   p->setPen(pen);

   p->drawLine( 7, -6, 7, 6 );
   
   if( m_zenerV>0 ) 
   {
       p->drawLine( 7,-6, 4,-6 );
       p->drawLine( 7, 6, 10, 6 );
   }
}

#include "moc_diode.cpp"
