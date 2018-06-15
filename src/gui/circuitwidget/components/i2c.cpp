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

#include "i2c.h"
#include "pin.h"

Component* I2C::construct( QObject* parent, QString type, QString id )
{
        return new I2C( parent, type, id );
}

LibraryItem* I2C::libraryItem()
{
    return new LibraryItem(
        tr( "I2C" ),
        tr( "Logic" ),
        "subc.png",
        "I2C",
        I2C::construct );
}

I2C::I2C( QObject* parent, QString type, QString id )
   : LogicComponent( parent, type, id ), eI2C( id.toStdString() )
{
    m_width  = 4;
    m_height = 4;
    
    QStringList pinList;                              // Create Pin List

    pinList // Inputs:
            << "IL01 SDA"//type: Input, side: Left, pos: 01, name: "SDA"
            << "IL03 SCL"
            
            // Outputs:
            ;
    init( pinList );                   // Create Pins Defined in pinList
    
    eLogicDevice::createInput( m_inPin[0] );                // Input SDA
    eLogicDevice::createClockPin( m_inPin[1] );             // Input SCL
}
I2C::~I2C(){}

void I2C::setVChanged()             // Some Pin Changed State, Manage it
{
    eI2C::setVChanged();                               // Run I2C Engine
    
    if( m_inputAvailable )                       // Handle Data Received
    {
        qDebug() << "I2C::setVChanged Received: " << (int)rxReg;
        
        char byte = rxReg;                         // Echo Received Byte
        eI2C::sendBite( byte );
        
        m_inputAvailable = false;
    }
}

#include "moc_i2c.cpp"
