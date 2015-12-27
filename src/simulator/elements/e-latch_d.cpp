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

#include "e-latch_d.h"

eLatchD::eLatchD( string id, int channels )
    : eLogicDevice( id )
{
    setNumChannels( channels );
}
eLatchD::~eLatchD(){}

void eLatchD::initialize()
{
    if( !m_clockPin )
    {
        qDebug() << "eLatchD::initialize !m_clockPin";
        for( uint i=0; i<m_input.size(); i++ )
        {
            eNode* enode = m_input[i]->getEpin()->getEnode();
            if( enode ) enode->addToChangedList(this);
        }
    }
    eLogicDevice::initialize();
}

/*void eLatchD::setInverted( bool inverted )
{
    for( int i=0; i>m_numOutputs; i++ )
        m_output[i]->setInverted( inverted );
}*/

void eLatchD::setVChanged()
{
    if( m_outEnablePin )
    {
        bool outEn = outputEnabled();
        for( int i=0; i<m_numOutputs; i++ )
        {
            if( outEn ) m_output[i]->setImp( m_outImp );
            else        m_output[i]->setImp( high_imp );
        }
    }
    if( !m_clockPin || (getClockState()==Rising) )
    {
        for( int i=0; i<m_numOutputs; i++ )
        {
            bool state = m_inputState[i];
            double volt = m_input[i]->getVolt();

            if     ( volt > m_inputHighV ) state = true;
            else if( volt < m_inputLowV )  state = false;

            m_inputState[i] = state;

            m_output[i]->setOut( state );
        }
    }
}

void eLatchD::setNumChannels( int channels ) { createPins( channels, channels); }

