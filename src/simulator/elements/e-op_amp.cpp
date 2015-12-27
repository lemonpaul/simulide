/***************************************************************************
 *   Copyright (C) 2003-2006 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "e-op_amp.h"

eOpAmp::eOpAmp( string id )
    : eElement( id )
{
    m_ePin.resize(3);
    m_lastVolt = 0;
}
eOpAmp::~eOpAmp(){}

void eOpAmp::initialize()
{
    m_output->setVoltHigh(0);
    
    for( int i=0; i<3; i++ ) // Inputs ePin 0 1, output 2, power 3 4
    {
        eNode* enode = m_ePin[i]->getEnode();
        if( enode ) enode->addToChangedList(this);
    }
}

void eOpAmp::setVChanged() // Called when input pins nodes change volt
{
    double volt = ( m_ePin[0]->getVolt()-m_ePin[1]->getVolt() );
    volt = volt*m_gain;
    
    
    if     ( volt > m_voltPos ) volt = m_voltPos;
    else if( volt < m_voltNeg ) volt = m_voltNeg;
    
    qDebug() << volt ;
    qDebug() << m_lastVolt;
    qDebug() << "--";
    double deltaVolt = (volt-m_lastVolt)/2;
    volt -= deltaVolt;
    qDebug() << deltaVolt;
    qDebug() << volt ;
    qDebug() << "-----";
    m_lastVolt = volt;

    m_output->setVoltHigh(volt);
}

