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

#ifndef EI2C_H
#define EI2C_H

#define I2C_IDLE      0
#define I2C_STARTED   1
#define I2C_READING   2
#define I2C_WRITTING  3
#define I2C_STOPPED   4
#define I2C_NOTME     5


#include "e-logic_device.h"


class MAINMODULE_EXPORT eI2C : public eLogicDevice
{
    public:

        eI2C( std::string id );
        ~eI2C();

        virtual void initialize();
        virtual void setVChanged();
        virtual void dataByteRecieved();

        void createPins();

        void sendBite( char byte );

    protected:
        char txReg;                                      // byte to Send
        char rxReg;                                      // byte Received
        char state;                                      // current state of i2c
        int address;                                // Addresr number
        char addressBits;                                // number of address bits
        char bitsRcv;                                    // number of bits recieved

        bool m_lastSDA;
        bool m_rw;                       //
        bool m_inputAvailable;           // A byte is available at rxReg
        bool m_outputAvailable;          // A byte is available at txReg

        int m_out;
};


#endif
