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

#include "latchd.h"
#include "pin.h"


Component* LatchD::construct( QObject* parent, QString type, QString id )
{
        return new LatchD( parent, type, id );
}

LibraryItem* LatchD::libraryItem()
{
    return new LibraryItem(
        tr( "Latch" ),
        tr( "Logic" ),
        "subc.png",
        "LatchD",
        LatchD::construct );
}

LatchD::LatchD( QObject* parent, QString type, QString id )
        : LogicComponent( parent, type, id ), eLatchD( id.toStdString() )
{
    m_width  = 4;
    m_height = 10;

    QStringList pinList;

    pinList // Inputs:
            << "IL01 D0"
            << "IL02 D1"
            << "IL03 D2"
            << "IL04 D3"
            << "IL05 D4"
            << "IL06 D5"
            << "IL07 D6"
            << "IL08 D7"

            << "IL09 IE"
            << "IR09OE"

            // Outputs:
            << "OR01O0 "
            << "OR02O1 "
            << "OR03O2 "
            << "OR04O3 "
            << "OR05O4 "
            << "OR06O5 "
            << "OR07O6 "
            << "OR08O7 "
            ;
    init( pinList );

    eLogicDevice::createInEnablePin( m_inPin[8] );       // Input Enable

    eLogicDevice::createOutEnablePin( m_inPin[9] );    // IOutput Enable

    for( int i=0; i<8; i++ )
    {
        eLogicDevice::createInput( m_inPin[i] );
        eLogicDevice::createOutput( m_outPin[i] );
    }
}
LatchD::~LatchD(){}

#include "moc_latchd.cpp"
