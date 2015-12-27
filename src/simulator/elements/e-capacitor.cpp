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

// Capacitor model using backward euler  approximation
// consists of a current source in parallel with a resistor.

#include <cmath>

#include "e-capacitor.h"

eCapacitor::eCapacitor( string id ) : eResistor( id )
{
    m_cap = 1; // uF
    m_tStep = 1e-6*(double)Simulator::self()->reaClock(); //0.0001; //1/(double)Simulator::self()->reaClock(); qDebug() << m_tStep;
    m_resist = m_tStep/(m_cap*1e-6);// capacitance property(m_cap) is in uF, covert to F
    m_curSource = 0;
    m_volt = 0;

    Simulator::self()->addToReacList( this );
}
eCapacitor::~eCapacitor(){ Simulator::self()->remFromReacList( this ); }


void eCapacitor::initialize()
{
    eResistor::initialize();
    m_tStep = 1e-6*(double)Simulator::self()->reaClock();
    eResistor::setRes( m_tStep/(m_cap*1e-6) );
    //qDebug() << m_tStep;
}

void eCapacitor::setVChanged()
{
    double volt = m_ePin[0]->getVolt() - m_ePin[1]->getVolt();

    if( volt==0 ) return;
    m_volt = volt;

    m_curSource = -volt/m_resist;

    //qDebug() << "eCapacitor::setVChanged voltdiff " <<volt<<" m_resist "<<m_resist<< " m_curSource "<<m_curSource;

    m_ePin[0]->stampCurrent(-m_curSource );
    m_ePin[1]->stampCurrent( m_curSource );
}

double eCapacitor::uF()             { return m_cap; }
void  eCapacitor::setuF( double c ) { m_cap = c; eResistor::setRes( m_tStep/(m_cap*1e-6) ); 
    /*qDebug() << "eCapacitor::setVChanged  m_tStep" << m_tStep <<" m_cap "<<m_cap<<" m_resist "<<m_resist;*/}

