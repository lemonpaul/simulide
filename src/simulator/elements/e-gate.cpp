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

#include "e-gate.h"
#include <QDebug>
eGate::eGate( std::string id, int inputs )
     : eLogicDevice( id )
{
    m_tristate = false;
}
eGate::~eGate()
{
}

void eGate::initialize()
{
    eLogicDevice::initialize();
    
    for( int i=0; i<m_numInputs; i++ )
    {
        eNode* enode = m_input[i]->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast(this);
    }
}

void eGate::setVChanged()
{
    if( m_tristate ) eLogicDevice::updateOutEnabled();

    int  inputs = 0;

    for( int i=0; i<m_numInputs; i++ )
    {
        bool  state = eLogicDevice::getInputState( i );

        if( state ) inputs++;
    }
    //qDebug() << "eGate::setVChanged" << inputs <<m_output[0]->imp(); 
    
    eLogicDevice::setOut( 0, calcOutput( inputs ) );// In each gate type
}

bool eGate::calcOutput( int inputs ) 
{ 
    return (inputs==m_numInputs); // Default for: Buffer, Inverter, And, Nand
}

bool eGate::tristate()
{
    return m_tristate;
}
void eGate::setTristate( bool t )
{
    m_tristate = t;
}
