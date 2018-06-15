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

#include <math.h>
#include <QDebug>

#include "e-outbus.h"

eOutBus::eOutBus( std::string id )
    : eLogicDevice( id )
{
}
eOutBus::~eOutBus()
{ 
}

void eOutBus::initialize()
{
    for( int i=0; i<m_numInputs; i++ )
    {
        eNode* enode = m_input[i]->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast(this);
    }
    m_output[0]->setImp( cero_doub );
    m_output[0]->setVoltHigh( 0 );
    m_output[0]->setOut( true );
}

void eOutBus::setVChanged()
{
    int address = 0;
    
    for( int i=0; i<m_numInputs; i++ )
    {
        if( getInputState( i ) ) address += pow( 2, i );
    }
    double volt = m_maxVolt*address/m_maxAddr;
    //qDebug() << "eOutBus::setVChanged" << address<<m_numInputs;
    
    m_output[0]->setVoltHigh( volt );
    m_output[0]->stampOutput();
}
