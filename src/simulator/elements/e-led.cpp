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

#include "e-led.h"
#include "simulator.h"

eLed::eLed( std::string id ) : eDiode( id )
{
    m_threshold = 2.4;

    m_prevStep   = Simulator::self()->step();
    m_maxCurrent = 0.03;
    m_lastCurrent = 0.0;

    disp_brightness  = 0;
    avg_brightness   = 0;
    lastUpdatePeriod = 0;
}
eLed::~eLed() {}

void eLed::initialize()
{
    //qDebug() << "eLed::initialize";
    m_prevStep   = Simulator::self()->step();
    m_lastCurrent = 0.0;
    m_bright = 0;

    disp_brightness  = 0;
    avg_brightness   = 0;
    lastUpdatePeriod = 0;

    eDiode::initialize();
}

void eLed::setVChanged()
{
    eDiode::setVChanged();
    updateVI();
}
void eLed::updateVI()
{
    eDiode::updateVI();
    
    const unsigned long long step = Simulator::self()->step();
    int period = step - m_prevStep;    

    m_prevStep = step;
    lastUpdatePeriod += period;

    if( m_lastCurrent > 0) avg_brightness += m_lastCurrent * period / m_maxCurrent;
    m_lastCurrent = m_current;
    //qDebug()<<"current"<< m_current<<m_lastCurrent<<period<< lastUpdatePeriod <<avg_brightness;
    //label->setText( QString("%1 A"). arg(double(int(m_current*1000))/1000) );
}

void eLed::updateBright()
{
    if( !Simulator::self()->isRunning() )
    {
        avg_brightness = 0;
        lastUpdatePeriod = 0;
        m_bright = 40;
        return;
    }
    updateVI();
    
    if( lastUpdatePeriod != 0. )
    {
        disp_brightness = avg_brightness/lastUpdatePeriod;
        //qDebug() << disp_brightness << avg_brightness << lastUpdatePeriod;
        avg_brightness   = 0;
        lastUpdatePeriod = 0;
    }
    m_bright = uint(disp_brightness*255)+40;
    //qDebug() << m_bright;
}
