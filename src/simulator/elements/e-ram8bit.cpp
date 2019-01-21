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

#include "e-ram8bit.h"
#include "e-node.h"
#include <QDebug>

eRam8bit::eRam8bit( std::string id )
        : eLogicDevice( id )
{
}
eRam8bit::~eRam8bit(){}

void eRam8bit::initialize()                 // Called at Simulation Start
{
    for( int i=8; i<10; i++ )                 // Initialize control pins
    {
        eNode* enode =  m_input[i]->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast( this );
    }
    for( int i=0; i<8; i++ ) m_dataPinState[i] = false;
    
    m_cs = false;
    m_oe = false;
    
    eLogicDevice::initialize();
    
    double imp = 1e28;
    for( int i=0; i<m_numOutputs; i++ ) m_output[i]->setImp( imp );
}

void eRam8bit::setVChanged()        // Some Pin Changed State, Manage it
{
    bool CS = eLogicDevice::getInputState(9);
    if( CS != m_cs )
    {
        m_cs = CS;
        
        if( !CS )
        {
            m_oe = false;
            double imp = 1e28;
            for( int i=0; i<m_numOutputs; i++ ) m_output[i]->setImp( imp );
        }
    }
    if( !CS ) return;
    
    bool WE = eLogicDevice::getInputState(8);
    bool oe = eLogicDevice::outputEnabled() && !WE;
    //qDebug() << WE << oe;
    
    if( oe != m_oe )
    {
        m_oe = oe;
        eLogicDevice::setOutputEnabled( oe );
    }    
    int address = 0;
    
    for( int i=0; i<8; i++ )                              // Get Address
    {
        bool  state = eLogicDevice::getInputState(i);
        
        if( state ) address += pow( 2, 7-i );
    }
    if( WE )                                                    // Write
    {
        int value = 0;
        
        for( int i=0; i<8; i++ )
        {
            int volt = m_output[i]->getEpin()->getVolt();
            
            bool  state = m_dataPinState[i];
            
            if     ( volt > m_inputHighV ) state = true;
            else if( volt < m_inputLowV )  state = false;
            
            m_dataPinState[i] = state;
            //qDebug() << "Bit " << i << state;
            if( state ) value += pow( 2, i );
        }
        m_ram[address] = value;
        //qDebug() << "Writting " << address << value;
    }
    else                                                         // Read
    {
        int value = m_ram[address];
        //qDebug() << "Reading " << address << value;
        for( int i=0; i<8; i++ )
        {
            bool pinState =  value & 1;
            m_output[i]->setOut( pinState );
            m_output[i]->stampOutput();
            //qDebug() << "Bit " << i << pinState;
            value >>= 1;
        }
        
    }
}
