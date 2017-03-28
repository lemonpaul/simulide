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

#include <QDebug>

#include "e-diode.h"
#include "e-node.h"

eDiode::eDiode( std::string id ) : eResistor(id )
{
    m_imped = 0.6;
    m_threshold = 0.7;
    m_zenerV = 0;
    m_resist = high_imp;
}
eDiode::~eDiode()
{ 
}

void eDiode::initialize()
{
    m_resist = high_imp;
    m_converged = true;
    m_voltPN  = 0;
    m_deltaV  = 0;
    m_current = 0;

    if( m_ePin[0]->isConnected() ) m_ePin[0]->getEnode()->addToNoLinList(this);
    if( m_ePin[1]->isConnected() ) m_ePin[1]->getEnode()->addToNoLinList(this);
    eResistor::initialize();
}

void eDiode::setVChanged()
{
    m_voltPN = m_ePin[0]->getVolt()-m_ePin[1]->getVolt();
    m_converged = true;
    
    //if( abs(m_voltPN) < 1e-9 ) m_voltPN = 0;

    double deltaR = m_imped;
    double deltaV = m_threshold;

    if( m_voltPN <= m_threshold )   // Not conducing
    {
        if( (m_zenerV > 0)&(m_voltPN <-m_zenerV) ) 
            deltaV =-m_zenerV;
        else                        
        {
            deltaV = 0;//cero_doub;//m_voltPN;
            deltaR = high_imp;
        }
    }
    //qDebug() <<"eDiode::setVChanged,  deltaR: "<< deltaR << "  deltaV" << deltaV << "m_voltPN" << m_voltPN ;

    if( deltaR != m_resist )
    {
        m_resist = deltaR;
        eResistor::stamp();
        m_converged = false;
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
}

double eDiode::res()
{
    return m_imped;
}

void eDiode::setRes( double resist )
{
    if( resist == 0 ) resist = 0.6;
    m_imped = resist;
}

void  eDiode::setZenerV( double zenerV ) 
{ 
    if( zenerV > 0 ) m_zenerV = zenerV; 
    else             m_zenerV = 0;
    setResSafe( m_resist );
}

void eDiode::updateVI()
{
    if( !m_converged ) return;

    if( m_resist == high_imp ) 
    {
        m_current = 0;
        return;
    }
    if( m_ePin[0]->isConnected() && m_ePin[1]->isConnected() )
    {
        double volt = m_voltPN - m_deltaV;
        if( volt>0 )
        {
            m_current = volt/m_resist;
            //qDebug() << " current " <<m_current<<volt<<m_deltaV;
        }
    }
    else m_current = 0;
}

