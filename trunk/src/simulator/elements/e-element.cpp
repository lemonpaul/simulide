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

#include "e-element.h"
//#include <iostream>
#include <sstream>

eElement::eElement( string id )
    : cero_doub( 1e-12 )
    , high_imp( 1e12 )
    , digital_high( 5.0 )
    , digital_low( 0.0 )
    , digital_threshold( 2.5 )
{
    m_elmId = id;

    Simulator::self()->addToElementList( this );
}
eElement::~eElement()
{
    Simulator::self()->remFromElementList( this );
    m_ePin.clear();
}

void eElement::initEpins()
{
    setNumEpins(2); // by default create 2 ePins
}

void eElement::setNumEpins( int n )
{
    m_ePin.clear();

    m_ePin.resize(n);

    for( int i=0; i<n; i++ )
    {
        std::stringstream ss;
        ss << m_elmId << "ePin" << i;
        m_ePin[i] = new ePin( ss.str(), i );
    }
}

ePin* eElement::getEpin( int pin )
{
    return m_ePin[pin];
}

