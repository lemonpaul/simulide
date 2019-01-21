/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#include <math.h>
//#include <QDebug>

#include "e-outbus.h"

eOutBus::eOutBus( std::string id )
       : eLogicDevice( id )
{
}
eOutBus::~eOutBus()
{ 
}

void eOutBus::initialize()
{
    for( int i=0; i<m_numInputs; i++ )
    {
        m_threshold = (m_inputHighV+m_inputLowV)/2;
        eSource* esource = m_input[i];
        esource->setImp( 1e7 );
        esource->setVoltHigh( m_threshold );
        esource->setVoltLow( m_threshold );
        
        eNode* enode = esource->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast(this);
    }
    m_output[0]->setImp( cero_doub );
    m_output[0]->setVoltHigh( 0 );
    m_output[0]->setOut( true );
}

void eOutBus::setVChanged()
{
    int address = 0;
    
    bool haveInput = false;
    
    for( int i=0; i<m_numInputs; i++ )
    {
        double volt = m_input[i]->getVolt();
        
        if( fabs( volt-m_threshold ) > m_threshold/5 ) haveInput = true;
        
        //qDebug() << "eOutBus::setVChanged" <<volt<<m_threshold<<haveInput;
        
        bool  state = m_inputState[i];

        if     ( volt > m_inputHighV ) state = true;
        else if( volt < m_inputLowV )  state = false;

        if( m_input[i]->isInverted() ) state = !state;
        m_inputState[i] = state;
        
        if( state ) address += pow( 2, m_numInputs-1-i );
    }
    double v = m_maxVolt*address/m_maxAddr;
    double imp = m_outImp;
    
    if( !haveInput ) 
    {
        imp = m_inputImp;
        v = 0;
    }
    eSource* output = m_output[0];
    
    if( output->imp() != imp ) output->setImp( imp );

    //qDebug() << "eOutBus::setVChanged" << v << address<<m_maxAddr<<m_maxVolt;
    
    output->setVoltHigh( v );
    output->stampOutput();
}
