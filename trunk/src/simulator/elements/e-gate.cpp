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

#include "e-gate.h"

eGate::eGate( string id, int inputs )
    : eLogicDevice( id )
{
    createPins( inputs, 1 );         // From eLogicDevice
    setInverted( false );
    //m_prevInputs = 0;
}
eGate::~eGate(){}

void eGate::initialize()
{
    for( int i=0; i<m_numInputs; i++ )
    {
        eNode* enode = m_input[i]->getEpin()->getEnode();
        if( enode ) enode->addToChangedList(this);
    }
}

void eGate::setInverted( bool inverted )
{
    m_inverted = inverted;
    m_output[0]->setInverted( inverted );
}


void eGate::setVChanged()
{
    int  inputs = 0;

    for( int i=0; i<m_numInputs; i++ )
    {
        bool  state = false; // = m_inputState[i];
        double volt = m_input[i]->getVolt();

        if     ( volt > m_inputHighV ) state = true;
        else if( volt < m_inputLowV )  state = false;

        if( state ) inputs++;
        m_inputState[i] = state;
    }
    //qDebug() << "eGate::setVChanged" << m_prevInputs << inputs; 
    //if( m_prevInputs != inputs )
    //{
        m_output[0]->setOut( calcOutput( inputs ) );   // In each gate type
        //m_prevInputs = inputs; 
    //}
}

bool eGate::calcOutput( int inputs ) { return (inputs==m_numInputs); } // default for: Buffer, Inverter, And, Nand

