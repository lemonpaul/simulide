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

#include "clock-base.h"
#include "pin.h"
#include "simulator.h"


ClockBase::ClockBase( QObject* parent, QString type, QString id )
    : LogicInput( parent, type, id )
{
    m_isRunning = false;

    m_stepsPC = 0;
    m_step = 0;
    setFreq( 1000 );

    Simulator::self()->addToUpdateList( this );
}
ClockBase::~ClockBase(){}

void ClockBase::updateStep()
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

void ClockBase::setFreq( int freq )
{
    //m_freq = freq;
    m_stepsPC = 1e6/(double)freq;
    
    if (m_stepsPC < 1) m_stepsPC = 1;
    
    m_freq = 1e6/m_stepsPC;
}

void ClockBase::onbuttonclicked()
{
    m_isRunning = !m_isRunning;
    m_step = 0;

    m_changed = true;
    //qDebug() << m_stepsPC << m_isRunning ;
}

void ClockBase::remove()
{

    Simulator::self()->remFromSimuClockList( this );

    LogicInput::remove();
}

#include "moc_clock-base.cpp"

