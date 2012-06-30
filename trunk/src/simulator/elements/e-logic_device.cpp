/***************************************************************************
 *   Copyright (C) 2003-2006 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   Copyright (C) 2010 by santiago González                               *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <sstream>
#include "e-logic_device.h"

eLogicDevice::eLogicDevice( string id )
    : eElement( id )
{
    m_numInputs  = 0;
    m_numOutputs = 0;

    m_inputHighV = 2.5;
    m_inputLowV  = 2.5;
    m_outHighV   = 5;
    m_outLowV    = 0;

    m_inputImp = high_imp;
    m_outImp   = 50;
}
eLogicDevice::~eLogicDevice()
{
    m_output.clear();
    m_input.clear();
}

void eLogicDevice::createPins( int inputs, int outputs )
{
    setNumImps( inputs );
    setNumOuts( outputs );
}

void eLogicDevice::createInputs( int inputs )
{
    int totalInps  = m_numInputs + inputs;
    m_input.resize( totalInps );

    for( int i=m_numInputs; i<totalInps; i++ )
    {
        std::stringstream sspin;
        sspin << m_elmId << "ePinInput" << i;
        ePin* epin = new ePin( sspin.str(), i );

        std::stringstream ssesource;
        ssesource << m_elmId << "eSourceInput" << i;
        m_input[i] = new eSource( ssesource.str(), epin );
        m_input[i]->setImp( m_inputImp );
    }
    m_numInputs = totalInps;
}

void eLogicDevice::createOutputs( int outputs )
{
    int totalOuts = m_numOutputs + outputs;
    m_output.resize( totalOuts );

    for( int i=m_numOutputs; i<totalOuts; i++ )
    {
        std::stringstream sspin;
        sspin << m_elmId << "ePinOutput" << i;
        ePin* epin = new ePin( sspin.str(), i );

        std::stringstream ssesource;
        ssesource << m_elmId << "eSourceOutput" << i;
        m_output[i] = new eSource( ssesource.str(), epin );
        m_output[i]->setVoltHigh( m_outHighV );
        m_output[i]->setImp( m_outImp );
    }
    m_numOutputs = totalOuts;
}

void eLogicDevice::deleteInputs( int inputs )
{
    m_numInputs -= inputs;

    for( int i=0; i<inputs; i++ ) m_input.pop_back();
}

void eLogicDevice::deleteOutputs( int outputs )
{
    m_numOutputs -= outputs;

    for( int i=0; i<outputs; i++ ) m_output.pop_back();
}

void eLogicDevice::setNumImps( int inputs )
{
    if     ( inputs > m_numInputs ) createInputs( inputs - m_numInputs );
    else if( inputs < m_numInputs ) deleteInputs( m_numInputs - inputs );
    else return;
    m_inputState.resize( inputs );
}

void eLogicDevice::setNumOuts( int outputs )
{
    if     ( outputs > m_numOutputs ) createOutputs( outputs - m_numOutputs );
    else if( outputs < m_numOutputs ) deleteOutputs( m_numOutputs - outputs );
}

void eLogicDevice::setOutHighV( double volt )
{
    m_outHighV = volt;

    for( int i=0; i<m_numOutputs; i++ )
        m_output[i]->setVoltHigh( volt );
}

void eLogicDevice::setOutLowV( double volt )
{
    m_outLowV = volt;

    for( int i=0; i<m_numOutputs; i++ )
        m_output[i]->setVoltLow( volt );
}

void eLogicDevice::setInputImp( double imp )
{
    m_inputImp = imp;

    for( int i=0; i<m_numOutputs; i++ )
        m_input[i]->setImp( imp );
}

void eLogicDevice::setOutImp( double imp )
{
    m_outImp = imp;

    for( int i=0; i<m_numInputs; i++ )
        m_output[i]->setImp( imp );
}

ePin* eLogicDevice::getEpin( int pin )  // First InPuts, then OutPuts
{
    //qDebug() << "eLogicDevice::getEpin " << pin;
    if( pin<m_numInputs ) return m_input[pin]->getEpin();
    else                  return m_output[pin-m_numInputs]->getEpin();
}
