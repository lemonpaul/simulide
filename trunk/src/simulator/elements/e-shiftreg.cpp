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

eShiftReg::eShiftReg( string id ) : eElement( id )
{
    // ePins 0-7: Q0-Q7
    // ePin    8: DS    serial data input
    // ePin    9: MR    master reset (active LOW)
    // ePin   10: SH    shift register clock input (L to H  Edge-triggered)

    //m_ePin.resize(11);

    m_shiftReg.reset();
}
eShiftReg::~eShiftReg()
{
    for( int i=0; i<8; i++ )
    {
        delete m_output[i];
        delete m_ePin[i];
    }
}

void eShiftReg::initEpins()
{
    eElement::setNumEpins(11);
    // Create outputs
    for( int i=0; i<8; i++ )
    {
        m_output[i] = new eSource( m_elmId, m_ePin[i] );
        m_output[i]->setVoltHigh(5);
    }
}

void eShiftReg::initialize()
{
    // Register for callBack when eNode volt change on clock or reset pins
    eNode* enode = m_ePin[9]->getEnode();
    if( enode ) enode->addToChangedList(this);

    enode = m_ePin[10]->getEnode();
    if( enode ) enode->addToChangedList(this);

}

void eShiftReg::setVChanged()
{
    if( !Simulator::self()->isRunning() )return;

    if( m_ePin[9]->getVolt()<digital_threshold )    // Read reset pin
    {
        m_shiftReg.reset();                         // Reset shift register if reset pin = Low
        for( int i=0; i<8; i++ ) m_output[i]->setOut( m_shiftReg[i] );// Set outputs
    }
    else
    {                                               // Read clock pin
        bool clock = m_ePin[10]->getVolt()>digital_threshold;
        //qDebug() << "eShiftReg::setVChanged" << m_clockPrev << clock << Simulator::self()->isRunning();

        if(  clock && !m_clockPrev)                 // If L to H edge
        {
            for( int i=7; i>0; i-- )
            {
                m_shiftReg[i] = m_shiftReg[i-1];
                m_output[i]->setOut( m_shiftReg[i] );// Set outputs
            }
            // Read data input pin & put in reg bit0
            m_shiftReg[0] = m_ePin[8]->getVolt()>digital_threshold;
            m_output[0]->setOut( m_shiftReg[0] );
        }
        m_clockPrev = clock;
    }
}

