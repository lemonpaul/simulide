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

#include "ledbase.h"
#include "connector.h"

LedBase::LedBase( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eLed( id.toStdString() )
{
    m_ePin.resize(2);

    m_color = QColor( Qt::black );

    Simulator::self()->addToUpdateList( this );
}
LedBase::~LedBase(){ Simulator::self()->remFromUpdateList( this ); }

void LedBase::updateStep()
{
    //if( m_prevpVolt == m_pVolt && m_prevnVolt == m_nVolt ) return;
    update();
}

void LedBase::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    Component::remove();
}

void LedBase::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    eLed::updateBright();

    QPen pen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    if( m_bright > 255+75 )  // Corriente sobrepasada
    {
        m_bright = 0;
        p->setBrush( Qt::white );
        pen.setColor( QColor( Qt::white ));
    }

    if( m_bright > 20 )
    {
        m_bright += 30;    // Set a minimun bright
        if( m_bright > 255 ) m_bright = 255;
    }

    p->setPen(pen);

    drawBackground( p );

    pen.setColor( QColor( m_bright, m_bright, 50 ));
    pen.setWidth(2.5);
    p->setPen(pen);
    p->setBrush( QColor( m_bright, m_bright, 50) );

    drawForeground( p );
}

#include "moc_ledbase.cpp"
