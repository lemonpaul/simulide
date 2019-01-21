/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#include "ram8bit.h"

Component* Ram8bit::construct( QObject* parent, QString type, QString id )
{
    return new Ram8bit( parent, type, id );
}

LibraryItem* Ram8bit::libraryItem()
{
    return new LibraryItem(
        tr( "8bit Ram" ),
        tr( "Logic/Memory" ),
        "subc.png",
        "Ram8bit",
        Ram8bit::construct );
}

Ram8bit::Ram8bit( QObject* parent, QString type, QString id )
       : LogicComponent( parent, type, id )
       , eRam8bit( id.toStdString() )
{
    m_width  = 4;
    m_height = 11;
    
    QStringList pinList;                              // Create Pin List

    pinList // Inputs:
            << "IL01 A0"
            << "IL02 A1"
            << "IL03 A2"
            << "IL04 A3"
            << "IL05 A4"
            << "IL06 A5"
            << "IL07 A6"
            << "IL08 A7"
            
            << "IL09 WE"
            << "IL10 OE"
            << "IR10 CS"
            
            // Outputs:
            << "OR01 D0"
            << "OR02 D1"
            << "OR03 D2"
            << "OR04 D3"
            << "OR05 D4"
            << "OR06 D5"
            << "OR07 D6"
            << "OR08 D7"
            ;
    init( pinList );                   // Create Pins Defined in pinList
     
    for( int i=0; i<8; i++ ) 
    {
        eLogicDevice::createInput( m_inPin[i] );
        eLogicDevice::createOutput( m_outPin[i] );
    }
    eLogicDevice::createInput( m_inPin[8] );                       // RW
    eLogicDevice::createOutEnablePin( m_inPin[9] );                // OE
    eLogicDevice::createInput( m_inPin[10] );                      // CS
    
    for( int i=8; i<10; i++ ) m_input[i]->setInverted( true ); // Invert control pins
}
Ram8bit::~Ram8bit(){}

#include "moc_ram8bit.cpp"
