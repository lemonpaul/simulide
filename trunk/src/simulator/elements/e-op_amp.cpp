/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "e-op_amp.h"

eOpAmp::eOpAmp( string id )
    : eElement( id )
{
    m_ePin.resize(3);
    m_gain = 1000;
    
    m_connected = false;
}
eOpAmp::~eOpAmp()
{ 
}

void eOpAmp::initialize()
{
    m_lastOut = 0;
    m_lastIn  = 0;
    m_k = 1e-6/m_gain;
    m_converged = true;
    
    if( m_ePin[0]->isConnected() )
    {
        m_ePin[0]->getEnode()->addToNoLinList(this);
    }
    if( m_ePin[1]->isConnected() )
    {
        m_ePin[1]->getEnode()->addToNoLinList(this);
    }
}

void eOpAmp::setVChanged() // Called when input pins nodes change volt
{
    double vP = m_ePin[0]->getVolt();
    double vN = m_ePin[1]->getVolt();
    double vd = vP-vN;

    //qDebug() << "lastIn " << m_lastIn << "vd " << vd ;
    
    double out = vd * m_gain;
    if     ( out >= m_voltPos ) out = m_voltPos;
    else if( out <= m_voltNeg ) out = m_voltNeg;
    
    //qDebug() << "lastOut " << m_lastOut << "out " << out << abs(out-m_lastOut)<< "<1e-5 ??";

    if( abs(out-m_lastOut)<1e-5 )
    {
        m_converged = true;
        return;
    }

    if( m_converged )                  // First step after a convergence
    {
        double dOut = -1e-6;
        if( vd>0 ) dOut =  1e-6;
        
        out = m_lastOut + dOut;
        m_converged = false;
    }
    else
    {
        if( m_lastIn != vd )
        {
            // We problably are in a close loop configuration
            
            // Input diff with last dOut
            double dIn  = abs(m_lastIn-vd); 
            
            // Guess next converging output:
            //dOut = (vd-out/m_gain)/dIn*1e-4;
            out = (m_lastOut*dIn + vd*1e-6)/(dIn + m_k);
        }
        m_converged = true;
    }
    
    if     ( out >= m_voltPos ) out = m_voltPos;
    else if( out <= m_voltNeg ) out = m_voltNeg;
    
    //qDebug()<< "lastOut " << m_lastOut << "out " << out << "dOut" << dOut  << "converged" << m_converged;
    
    m_lastIn = vd;
    m_lastOut = out;
    
    m_output->setVoltHigh(out);
    m_output->stampOutput();

}

