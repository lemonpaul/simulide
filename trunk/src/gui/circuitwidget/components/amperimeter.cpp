/***************************************************************************
 *   Copyright (C) 2017 by santiago González                               *
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

#include "amperimeter.h"
#include "simulator.h"
#include "connector.h"


Component* Amperimeter::construct( QObject* parent, QString type, QString id )
{ return new Amperimeter( parent, type, id ); }

LibraryItem* Amperimeter::libraryItem()
{
    return new LibraryItem(
            tr( "Amperimeter" ),
            tr( "Meters" ),
            "amperimeter.png",
            "Amperimeter",
            Amperimeter::construct);
}

Amperimeter::Amperimeter( QObject* parent, QString type, QString id )
    : Meter( parent, type, id )
{
    m_unit = "A";
    setRes( 1e-6 );
    Meter::updateStep();
}
Amperimeter::~Amperimeter(){}

void Amperimeter::updateStep()
{
    double curr = current();
    
    if( curr != m_dispValue )
    { 
        setUnit("A");
        m_dispValue = curr;
        Meter::updateStep();
    }
}

#include "moc_amperimeter.cpp"
