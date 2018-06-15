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
//modified test
#include "e-i2c.h"
#include <cstdio>

eI2C::eI2C( std::string id )
    : eLogicDevice( id )
{

}
eI2C::~eI2C() { }

void eI2C::initialize()                    // Called at Simulation Start
{
       // Initialize Base Class ( Clock pin is managed in eLogicDevice )
    eLogicDevice::initialize();

        // Register for Pin Changes CallBack ( CallBack: setVChanged() )
    eNode* enode = m_input[0]->getEpin()->getEnode();
    if( enode ) enode->addToChangedFast(this);

                                                 // Initialize Variables
    txReg = 0;
    rxReg = 0;
    bitsRcv = 0;
    m_lastSDA = false;
    state = I2C_IDLE;
    addressBits = 7;
    address = 0x08;
    m_inputAvailable = false;
}

void eI2C::setVChanged()            // Some Pin Changed State, Manage it
{
    // Get Clk to don't miss any clock changes
    int sclState = eLogicDevice::getClockState();



    // e-logic_device.h:
    //
    // #define CLow    0
    // #define Rising  1
    // #define CHigh   2
    // #define Falling 3

    bool sda = eLogicDevice::getInputState( 0 );     // State of SDA pin

    if (sclState == CHigh){
      switch (state) {
        case I2C_IDLE:
           if (m_lastSDA && !sda)              // We are in a Start Condition
          {
            state = I2C_STARTED;
            printf("I²C Start conditcion recieved\n");
          }
        break;
        case I2C_STARTED:
          if ( bitsRcv <= addressBits + 1){         //TODO add support for 10 bits
            rxReg += sda;                       //Read one bit from sda
            rxReg = rxReg << 1;
            bitsRcv++;
          }
          else
          {
            m_rw = rxReg % 2; //Last bit is R/W
            if ((rxReg >> 1) == address){ //not considering R/W bit for address

              m_input[0]->setImp(40);
              m_out = 1;
              m_input[0]->setOut(true);


              //pull sda low for ACK
              if (m_rw)
                state = I2C_READING;
              else
                state = I2C_WRITTING;
            }
            else
              state = I2C_NOTME;
            bitsRcv = 0;
          }
        break;
        case I2C_NOTME:
          if (!m_lastSDA && sda)
            state = I2C_IDLE;
        break;
        case I2C_READING:
          if (bitsRcv < 8){
            rxReg += sda;                       //Read one bit from sda
            rxReg = rxReg << 1;
            bitsRcv++;
          }
          else{
            // pull sda low for ACK
            dataByteRecieved();
            state = I2C_IDLE;
          }
        break;
      }
    }
    if( sclState == Rising )             // We are in a SCL Rissing edge
    {
        ;
    }



    m_lastSDA = sda;
}

void eI2C::sendBite( char byte )
{
    txReg = byte;
    m_outputAvailable = true;
}
void eI2C::dataByteRecieved(){
  printf("Se recibió el caracter: %c\n",rxReg);
}
void eI2C::createPins()  // Usually Called by Subcircuit to create ePins
{
    createClockPin();            // Clock pin is managed in eLogicDevice

    eLogicDevice::createPins( 1, 0 );        // Create input Pin for SDA
}
