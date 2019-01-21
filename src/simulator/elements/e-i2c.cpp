/***************************************************************************
 *   Copyright (C) 2017 by Santiago González                               *
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

#include "e-i2c.h"
//#include <cstdio>
//#include <QDebug>

eI2C::eI2C( std::string id )
    : eLogicDevice( id )
{
    m_address = 0x00;
}
eI2C::~eI2C() { }

void eI2C::initialize()                    // Called at Simulation Start
{
    eLogicDevice::initialize();   // Initialize Base Class ( Clock pin is managed in eLogicDevice )

    eNode* enode = m_input[0]->getEpin()->getEnode();
    if( enode ) enode->addToChangedFast( this );

    m_txReg = 0;
    m_rxReg = 0;
    m_bitPtr = 0;
    m_lastSDA = false;
    m_state = I2C_STOPPED;
    m_addressBits = 7;
    
    m_input[0]->setVoltHigh( 5 );
    m_input[0]->setOut( false );
    m_input[0]->setImp( high_imp );
}

void eI2C::setVChanged()            // Some Pin Changed State, Manage it
{
    int sclState = eLogicDevice::getClockState(); // Get Clk to don't miss any clock changes
    
    m_SDA = eLogicDevice::getInputState( 0 );        // State of SDA pin

    if(( sclState == CHigh )&&( m_state != I2C_ACK ))
    {
        if( !m_SDA  && m_lastSDA )       // We are in a Start Condition
        {
            m_bitPtr = 0;
            m_rxReg = 0;
            m_state = I2C_STARTED;
            //printf("I²C Start conditcion recieved\n");
            
        }
        else if( m_SDA && !m_lastSDA )     // We are in a Stop Condition
        {
            m_state = I2C_STOPPED;
            //printf("I²C Stop conditcion recieved\n");
        }
    }
    else if( sclState == Rising )        // We are in a SCL Rissing edge
    {
        if( m_state == I2C_STARTED )             // Get Transaction Info
        {                                // TODO add support for 10 bits
            readBit();
            if( m_bitPtr > m_addressBits )
            {
                bool rw = m_rxReg % 2;                 //Last bit is R/W
                m_rxReg >>= 1;

                if( m_rxReg == m_address )              // Address match
                {
                    if( rw ) 
                    {
                        m_state = I2C_READING;
                        writeByte();
                    }
                    else
                    {
                        m_state = I2C_WRITTING;
                        m_bitPtr = 0;
                    }
                    ACK();
                }
                else
                {
                    m_state = I2C_STOPPED;
                    m_rxReg = 0;
                }
            }
        }
        else if( m_state == I2C_WRITTING )
        {
            readBit();
            if( m_bitPtr == 8 ) readByte();
        }
        else if( m_state == I2C_WAITACK )      // We wait for Master ACK
        {
            if( !m_SDA )                        // ACK: Continue Sending
            {
                m_state = m_lastState;
                writeByte();
            }
            else m_state = I2C_IDLE;
        }
    }
    else if( sclState == Falling )
    {
        if( m_state == I2C_ACK )
        {
            m_input[0]->setOut( false );
            m_input[0]->setImp( 40 );
            m_state = I2C_ENDACK;
        }
        else if( m_state == I2C_ENDACK )     // We just sent ACK, release SDA
        {
            m_input[0]->setImp( high_imp );
            m_state = m_lastState;
            m_rxReg = 0;
        }
        if( m_state == I2C_READING ) writeBit();
    }
    m_lastSDA = m_SDA;
}

void eI2C::readBit()
{
    if( m_bitPtr > 0 ) m_rxReg <<= 1;
    m_rxReg += m_SDA;                            //Read one bit from sda
    m_bitPtr++;
}

void eI2C::writeBit()
{
    if( m_bitPtr < 0 ) 
    {
        waitACK();
        return;
    }
    bool bit = m_txReg>>m_bitPtr & 1;
    
    m_input[0]->setOut( bit );
    m_input[0]->setImp( 40 );
    //qDebug() << "eI2C::writeBit()"<<m_bitPtr<<bit;
    m_bitPtr--;
}

void eI2C::readByte()
{
    m_bitPtr = 0;
    ACK();
}

void eI2C::writeByte()
{
    m_bitPtr = 7;
}

void eI2C::ACK()
{
    m_lastState = m_state;
    m_state = I2C_ACK;
}

void eI2C::waitACK()
{
    m_input[0]->setOut( false );
    m_input[0]->setImp( high_imp );
    
    m_lastState = m_state;
    m_state = I2C_WAITACK;
}

void eI2C::setAddress( int address )
{
    m_address = address;
}

void eI2C::createPins()  // Usually Called by Subcircuit to create ePins
{
    createClockPin();            // Clock pin is managed in eLogicDevice

    eLogicDevice::createPins( 1, 0 );        // Create input Pin for SDA
}
