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

#include "dectobcd.h"


Component* DecToBcd::construct( QObject* parent, QString type, QString id )
{
        return new DecToBcd( parent, type, id );
}

LibraryItem* DecToBcd::libraryItem()
{
    return new LibraryItem(
        tr( "Dec. To Bcd" ),
        tr( "Logic/Converters" ),
        "3to2g.png",
        "DecToBcd",
        DecToBcd::construct );
}

DecToBcd::DecToBcd( QObject* parent, QString type, QString id )
        : LogicComponent( parent, type, id ), eDecToBcd( id.toStdString() )
{
    m_width  = 4;
    m_height = 10;

    QStringList pinList;

    pinList // Inputs:
            << "IL01 D1"
            << "IL02 D2"
            << "IL03 D3"
            << "IL04 D4"
            << "IL05 D5"
            << "IL06 D6"
            << "IL07 D7"
            << "IL08 D8"
            << "IL09 D9"

            << "IU03OE "

            // Outputs:
            << "OR03A "
            << "OR04B "
            << "OR05C "
            << "OR06D "
            ;
    init( pinList );

    eLogicDevice::createOutEnablePin( m_inPin[9] );    // IOutput Enable

    for( int i=0; i<9; i++ )
    {
        eLogicDevice::createInput( m_inPin[i] );
    }
        
    for( int i=0; i<4; i++ )
    {
        eLogicDevice::createOutput( m_outPin[i] );
    }
}
DecToBcd::~DecToBcd(){}

#include "moc_dectobcd.cpp"
