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

#include "e-diode.h"


eDiode::eDiode( string id ) : eResistor(id )
{
    m_imped = 1;
    m_threshold = 0.6;
    m_resist = high_imp;

    //Simulator::self()->addToRunList( this );
}
eDiode::~eDiode(){ /*Simulator::self()->remFromRunList( this ); */}

void eDiode::initialize()
{
    if( m_ePin[0]->isConnected() ) m_ePin[0]->getEnode()->addToChangedList(this);
    if( m_ePin[1]->isConnected() ) m_ePin[1]->getEnode()->addToChangedList(this);
    eResistor::initialize();
}

void eDiode::setVChanged()
{
    m_pVolt  = m_ePin[0]->getVolt();
    m_nVolt  = m_ePin[1]->getVolt();
    //qDebug() << "eDiode::runStep " << m_pVolt << m_nVolt;

    double deltaR = m_imped;
    double deltaV = m_threshold;
    double voltPN = m_pVolt - m_nVolt;

    if( voltPN < m_threshold )   // No conduce
    {
        deltaV = m_pVolt;
        deltaR = high_imp;
    }
    //qDebug() <<"eDiode::runStep,  deltaR: "<< deltaR << "  deltaV" <<deltaV;

    if( deltaR != m_resist )
    {
        m_resist = deltaR;
        eResistor::stamp();
    }
    if( deltaV != m_deltaV )
    {
        m_deltaV = deltaV;

        double current = deltaV/m_resist;

        m_ePin[0]->stampCurrent( current );
        m_ePin[1]->stampCurrent(-current );
    }
}

void eDiode::setThreshold( double threshold )
{
    m_threshold = threshold;
    //update();
}

void eDiode::setRes( double resist )
{
    m_imped = resist;
}

void eDiode::updateVI()
{
    if( m_ePin[0]->isConnected() && m_ePin[1]->isConnected() )
    {
        double volt = m_pVolt - m_nVolt - m_deltaV;
        //if( volt < 0 ) volt = -volt;
        m_current = volt/m_resist;
        //qDebug() << " current " <<m_current<<volt<<m_deltaV;
    }
    else m_current = 0;
}
