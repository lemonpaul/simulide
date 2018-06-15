/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include <math.h>   // fabs(x,y)
#include <QDebug>

#include "e-mosfet.h"
#include "simulator.h"
#include "e-node.h"


eMosfet::eMosfet( std::string id )
    : eResistor( id )
{
    m_Pchannel = false;
    
    m_gateV     = 0;
    m_resist    = 1;
    m_RDSon     = 1;
    m_threshold = 3;

    m_ePin.resize(3);
}
eMosfet::~eMosfet(){}

void eMosfet::initialize()
{
    eResistor::setRes( 1 );
    
    m_accuracy = Simulator::self()->NLaccuracy();

    m_lastCurrent = 0;
    m_Vs = 0;
    m_Sfollow = false;
    
    m_kRDSon = m_RDSon*(10-m_threshold);
    m_Gth = m_threshold-m_threshold/4;
    
    if( (m_ePin[0]->isConnected()) 
      & (m_ePin[1]->isConnected())
      & (m_ePin[2]->isConnected()) ) 
    {
        m_ePin[0]->getEnode()->addToNoLinList(this);
        m_ePin[1]->getEnode()->addToNoLinList(this);
        m_ePin[2]->getEnode()->addToNoLinList(this);
    }
    eResistor::initialize();
}

void eMosfet::setVChanged()
{
    double Vgs;
    double Vds;
    double Vd = m_ePin[0]->getVolt();
    double Vs = m_ePin[1]->getVolt();
    double Vg = m_ePin[2]->getVolt();
    
    if(( m_Sfollow == false)&( fabs(Vs) > 1e-3 ))
    {
        if(( fabs(m_Vs) > 1e-3 )&( m_Vs != Vs )) m_Sfollow = true;
        m_Vs = Vs;
    }

    if( m_Pchannel )
    {
        Vgs = Vs-Vg;
        Vds = Vs-Vd;
    }
    else
    {
        Vgs = Vg-Vs;
        Vds = Vd-Vs;
    }
    m_gateV = Vgs - m_Gth;
    
    if( m_gateV < 0 ) m_gateV = 0;

    double satK = 1+Vds/100;
    double maxCurrDS = Vds/m_resist;
    
    if( Vds > m_gateV ) Vds = m_gateV;
    
    double DScurrent = (m_gateV*Vds-pow(Vds,2)/2)*satK/m_kRDSon;
    //if( m_Sfollow ) DScurrent /= 2;
    if( DScurrent > maxCurrDS ) DScurrent = maxCurrDS;
    
    double current = maxCurrDS-DScurrent;
    if( m_Pchannel ) current = -current;

    if( fabs(current-m_lastCurrent)<m_accuracy ) return;

    m_lastCurrent = current;
    m_ePin[0]->stampCurrent( current );
    m_ePin[1]->stampCurrent(-current );
}

void eMosfet::setRDSon( double rdson )
{
    if( rdson < cero_doub ) rdson = cero_doub;
    if( rdson > 10 ) rdson = 10;
    m_RDSon = rdson;
}
