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

#include "e-bcdtodec.h"

eBcdToDec::eBcdToDec( std::string id )
    : eLogicDevice( id )
{
}
eBcdToDec::~eBcdToDec()
{ 
}

void eBcdToDec::initialize()
{
    for( int i=0; i<4; i++ )
    {
        eNode* enode = m_input[i]->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast(this);
    }
    for( int i=0; i<10; i++ )
    {
        eLogicDevice::setOut( i, true );
    }

    eLogicDevice::initialize();
}

void eBcdToDec::setVChanged()
{
    if( m_outEnablePin )
    {
        bool outEnPrev = m_outEnable;
        bool outEn = eLogicDevice::outputEnabled();

        if( outEnPrev != outEn ) eLogicDevice::setOutputEnabled( outEn );
    }
    int address = 0;
    
    for( int i=0; i<4; i++ )
    {
        if( getInputState( i ) ) address += pow( 2, i );
    }

    if( address < 10 ) eLogicDevice::setOut( address, true );
    
    for( int i=0; i<10; i++ )
    {
        if( i != address ) eLogicDevice::setOut( i, false );
    }
}

void eBcdToDec::createPins()
{
    eLogicDevice::createPins( 4, 10 );
}
