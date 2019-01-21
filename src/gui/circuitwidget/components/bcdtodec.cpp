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

#include "bcdtodec.h"
#include "pin.h"


Component* BcdToDec::construct( QObject* parent, QString type, QString id )
{
        return new BcdToDec( parent, type, id );
}

LibraryItem* BcdToDec::libraryItem()
{
    return new LibraryItem(
        tr( "Bcd To Dec." ),
        tr( "Logic/Converters" ),
        "2to3g.png",
        "BcdToDec",
        BcdToDec::construct );
}

BcdToDec::BcdToDec( QObject* parent, QString type, QString id )
        : LogicComponent( parent, type, id )
        , eBcdToDec( id.toStdString() )
{
    m_width  = 4;
    m_height = 11;

    QStringList pinList;

    pinList // Inputs:
            
            << "IL04 S0"
            << "IL05 S1"
            << "IL06 S2"
            << "IL07 S3"

            << "IU01OE "
            
            // Outputs:
            << "OR01O0 "
            << "OR02O1 "
            << "OR03O2 "
            << "OR04O3 "
            << "OR05O4 "
            << "OR06O5 "
            << "OR07O6 "
            << "OR08O7 "
            << "OR09O8 "
            << "OR10O9 "
            ;
    init( pinList );

    eLogicDevice::createOutEnablePin( m_inPin[4] );    // IOutput Enable

    for( int i=0; i<4; i++ )
        eLogicDevice::createInput( m_inPin[i] );
        
    for( int i=0; i<10; i++ )
    {
        eLogicDevice::createOutput( m_outPin[i] );
    }
}
BcdToDec::~BcdToDec(){}

#include "moc_bcdtodec.cpp"
