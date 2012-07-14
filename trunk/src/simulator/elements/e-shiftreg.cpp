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

#include "e-shiftreg.h"
//#include "simulator.h"

eShiftReg::eShiftReg( string id ) : eLogicDevice( id )
{
    // input0: DS    serial data input
    // input1: MR    master reset (active LOW)

    m_shiftReg.reset();
}
eShiftReg::~eShiftReg()
{
}

void eShiftReg::initEpins()
{
    createPins( 2, 8 );
    createClockPin();
    /*eElement::setNumEpins(11);
    // Create outputs
    for( int i=0; i<8; i++ )
    {
        m_output[i] = new eSource( m_elmId, m_ePin[i] );
        m_output[i]->setVoltHigh(5);
    }*/
}

void eShiftReg::initialize()
{
    // Register for callBack when eNode volt change on clock or reset pins
    /*eNode* enode = m_ePin[9]->getEnode();
    if( enode ) enode->addToChangedList(this);

    enode = m_ePin[10]->getEnode();
    if( enode ) enode->addToChangedList(this);*/

    eNode* enode = m_input[1]->getEpin()->getEnode(); // m_input[1] = Reset pin
    if( enode ) enode->addToChangedList(this);

    eLogicDevice::initialize();
}

void eShiftReg::setVChanged()
{
    if( !Simulator::self()->isRunning() )return;

    double volt = m_input[1]->getEpin()->getVolt(); // Reset pin volt.

    if     ( volt > m_inputHighV ) m_inputState[1] = true;
    else if( volt < m_inputLowV )  m_inputState[1] = false;

    if( !(m_inputState[1]) )    // Reset pin is active low
    {
        m_shiftReg.reset();                         // Reset shift register if reset pin = Low
        for( int i=0; i<8; i++ ) m_output[i]->setOut( false );// Set outputs
    }
    else
    {
        if( getClockState()==Rising )
        {
            for( int i=7; i>0; i-- )
            {
                m_shiftReg[i] = m_shiftReg[i-1];
                m_output[i]->setOut( m_shiftReg[i] );// Set outputs
            }
            // Read data input pin & put in reg bit0
            volt = m_input[0]->getEpin()->getVolt(); // Reset pin volt.

            if     ( volt > m_inputHighV ) m_inputState[0] = true;
            else if( volt < m_inputLowV )  m_inputState[0] = false;

            m_shiftReg[0] = m_inputState[0];

            m_output[0]->setOut( m_shiftReg[0] );
        }
    }
}

