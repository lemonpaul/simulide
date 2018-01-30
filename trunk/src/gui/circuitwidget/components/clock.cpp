/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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

#include "clock.h"
#include "pin.h"
#include "simulator.h"

Component* Clock::construct( QObject* parent, QString type, QString id )
{
    return new Clock( parent, type, id );
}

LibraryItem* Clock::libraryItem()
{
    return new LibraryItem(
        tr( "Clock" ),
        tr( "Sources" ),
        "clock.png",
        "Clock",
        Clock::construct );
}

Clock::Clock( QObject* parent, QString type, QString id )
    : LogicInput( parent, type, id )
{
    m_isRunning = false;

    m_stepsPC = 0;
    m_step = 0;
    setFreq( 1000 );

    Simulator::self()->addToUpdateList( this );
}
Clock::~Clock(){}

void Clock::updateStep()
{
    if( m_changed )
    {
        if( m_isRunning )
            Simulator::self()->addToSimuClockList( this );
        else
        {
            m_out->setOut( false );
            m_out->stampOutput();
            Simulator::self()->remFromSimuClockList( this );
        }
        m_changed = false;
    }
}

void Clock::setFreq( int freq )
{
    //m_freq = freq;
    m_stepsPC = 1e6/freq/2;
    
    if (m_stepsPC < 1) m_stepsPC = 1;
    
    m_freq = 1e6/m_stepsPC/2;
}

void Clock::onbuttonclicked()
{
    m_isRunning = !m_isRunning;
    m_step = 0;

    m_changed = true;
    //qDebug() << m_stepsPC << m_isRunning ;
}

void Clock::simuClockStep()
{
    m_step++;

    if ( m_step >= m_stepsPC )
    {
        m_out->setOut( !m_out->out() );
        m_out->stampOutput();
        m_step = 0;
    }
}

void Clock::remove()
{

    Simulator::self()->remFromSimuClockList( this );

    LogicInput::remove();
}

void Clock::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    if (  m_isRunning )
        p->setBrush( QColor( 250, 200, 50 ) );
    else
        p->setBrush( QColor( 230, 230, 255 ) );

    p->drawRoundedRect( boundingRect(),2 ,2 );

    QPen pen;
    pen.setWidth(1);
    p->setPen( pen );
    
    p->drawLine(-11, 3,-11,-3 );
    p->drawLine(-11,-3,-5, -3 );
    p->drawLine( -5,-3,-5,  3 );
    p->drawLine( -5, 3, 1,  3 );
    p->drawLine(  1, 3, 1, -3 );
    p->drawLine(  1,-3, 4, -3 );
}

#include "moc_clock.cpp"

