/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#include <math.h>   // fabs(x,y)
#include <QDebug>

#include "e-volt_reg.h"

eVoltReg::eVoltReg( std::string id )
    : eElement( id )
{
    m_ePin.resize(3);
}
eVoltReg::~eVoltReg()
{ 
}

void eVoltReg::initialize()
{
    m_lastOut = 0;
    if( m_ePin[0]->isConnected() ) m_ePin[0]->getEnode()->addToNoLinList(this);
    if( m_ePin[1]->isConnected() ) m_ePin[1]->getEnode()->addToNoLinList(this);
    if( m_ePin[2]->isConnected() ) m_ePin[2]->getEnode()->addToNoLinList(this);
}

void eVoltReg::setVChanged() 
{
    m_voltPos = m_ePin[0]->getVolt();
    if( m_voltPos > 0.7 ) m_voltPos -= 0.7;
    else                  m_voltPos = 0;
    
    double vRefPin = m_ePin[1]->getVolt();
    
    double out = vRefPin+m_vRef;
    
    if     ( out > m_voltPos ) out = m_voltPos;
    else if( out < m_voltNeg ) out = m_voltNeg;
    //qDebug()<< out <<m_lastOut;

    if( fabs(out-m_lastOut)<1e-5 ) return;
    
    m_lastOut = out;

    m_ePin[2]->stampCurrent( out/cero_doub );
}

