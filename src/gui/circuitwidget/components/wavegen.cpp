/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#include "wavegen.h"
#include "pin.h"
#include "simulator.h"

Component* WaveGen::construct( QObject* parent, QString type, QString id )
{
    return new WaveGen( parent, type, id );
}

LibraryItem* WaveGen::libraryItem()
{
    return new LibraryItem(
        tr( "Wave Gen." ),
        tr( "Sources" ),
        "wavegen.png",
        "WaveGen",
        WaveGen::construct );
}

WaveGen::WaveGen( QObject* parent, QString type, QString id )
    : ClockBase( parent, type, id )
{
    m_voltBase = 0;
    m_type = Sine;
}
WaveGen::~WaveGen(){}

void WaveGen::simuClockStep()
{
    m_step++;
    if( m_type == Sine )     genSine();
    if( m_type == Saw )      genSaw();
    if( m_type == Triangle ) genTriangle();
    if( m_type == Square )   genSquare();

    m_out->setVoltHigh( m_voltHight*m_vOut+m_voltBase );
    m_out->stampOutput();
}

void WaveGen::genSine()
{
    double time = Simulator::self()->step();
    time = remainder( time, m_stepsPC );
    time = qDegreesToRadians( time*360/m_stepsPC );

    m_vOut = sin( time )/2+0.5;
}
void WaveGen::genSaw()
{
    if( m_step >= m_stepsPC )
    {
        m_vOut = 0;
        m_step = 0;
        return;
    }
    m_vOut = (double)m_step/m_stepsPC;
    
}
void WaveGen::genTriangle()
{
    int halfW = m_stepsPC/2;
    if( m_step >= halfW )
    {
        m_vOut = 1-(double)(m_step-halfW)/halfW;
        if( m_step >= m_stepsPC ) m_step = 0;
    }
    else m_vOut = (double)m_step/halfW;
}
void WaveGen::genSquare()
{
    int halfW = m_stepsPC/2;
    if( m_step >= halfW )
    {
        m_vOut = 0;
        if( m_step >= m_stepsPC ) m_step = 0;
    }
    else m_vOut = 1;
}
void WaveGen::updateStep()
{
    if(( !m_out-> out() )&( m_isRunning ))
    {
        m_out->setOut( true );
    }
    ClockBase::updateStep();
}

void WaveGen::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
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
    
    p->drawLine(-11, 3,-5, -3 );
    p->drawLine( -5,-3,-5,  3 );
    p->drawLine( -5, 3, 1, -3 );
    p->drawLine(  1,-3, 1,  3 );
    p->drawLine(  1, 3, 4,  0 );
}

#include "moc_wavegen.cpp"

