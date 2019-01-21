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

#include <math.h>
#include <QDebug>

#include "e-bus.h"

eBus::eBus( std::string id )
    : eLogicDevice( id )
{
    m_driving = false;
}
eBus::~eBus()
{ 
}

void eBus::initialize()
{
    setInputHighV( 2.5 );
    setInputLowV( 2.5 );
    for( int i=0; i<m_numInputs; i++ )
    {
        m_threshold = (m_inputHighV+m_inputLowV)/2;
        eSource* esource = m_input[i];
        esource->setImp( 1e7 );
        esource->setVoltHigh( m_threshold );
        esource->setVoltLow( m_threshold );
        esource->stampOutput();
        
        eNode* enode = esource->getEpin()->getEnode();
        if( enode ) enode->addToChangedFast(this);
    }
    m_output[0]->setImp( 1e7 );
    m_output[0]->setVoltHigh( -1 );
    m_output[0]->setVoltLow( -1 );
    m_output[0]->setOut( true );
    m_output[0]->stampOutput();
    
    eNode* enode = m_output[0]->getEpin()->getEnode();
    if( enode ) enode->addToChangedFast(this);
    
    m_driving = false;
}

void eBus::setVChanged()
{
    int address = 0;
    
    bool dataInBus = false;
    double volt = m_output[0]->getVolt();
        
    if( volt > -0.5 ) dataInBus = true;
    //qDebug()<<"\n"<< "eBus::setVChanged dataInBus"<<QString::fromStdString(m_elmId)<<dataInBus<<volt;
    
    if( !dataInBus ) m_driving = true;
    
    if( m_driving )
    {
        m_driving = false;
        
        if( m_input[0]->imp() == m_outImp ) // Was input, reset
        {
            for( int i=0; i<m_numInputs; i++ )
            {
                m_threshold = (m_inputHighV+m_inputLowV)/2;
                eSource* esource = m_input[i];
                esource->setImp( 1e7 );
                esource->setVoltHigh( m_threshold );
                esource->setVoltLow( m_threshold );
                esource->stampOutput();
            }
            return;
        }
        
        for( int i=0; i<m_numInputs; i++ )
        {
            
            double volt = m_input[i]->getVolt();
            
            if( fabs( volt-m_threshold ) > m_threshold/5 ) m_driving = true;
            //qDebug() << "eBus::setVChanged" <<volt<<m_threshold<<m_isInput;
            
            bool  state = m_inputState[i];

            if     ( volt > m_inputHighV ) state = true;
            else if( volt < m_inputLowV )  state = false;

            m_inputState[i] = state;
            
            if( state ) address += pow( 2, m_numInputs-1-i );
        }//qDebug() << "eBus::setVChanged is Output"<<QString::fromStdString(m_elmId) <<!m_isInput;
        double v = -1;
        double imp = 1e7;
        
        if( m_driving )
        {
            v = m_maxVolt*address/m_maxAddr;
            imp = m_outImp;
        }
        eSource* output = m_output[0];
        if( output->imp() != imp ) output->setImp( imp );
            
        output->setVoltHigh( v );
        output->stampOutput();
    }
    else
    {
        double volt = m_output[0]->getVolt();

        int address = (int)(volt*m_maxAddr/m_maxVolt+0.1);
        //qDebug()<<"\n" << "eInBus::setVChanged"<<volt << address<<(volt*m_maxAddr/m_maxVolt)<<(int)(volt*m_maxAddr/m_maxVolt);
        for( int i=0; i<m_numInputs; i++ )
        {
            //qDebug() << "eInBus::setVChanged" << i << (address & 1);

            eSource* esource = m_input[m_numInputs-1-i];
            esource->setImp( m_outImp );
            esource->setVoltHigh( m_outHighV );
            esource->setVoltLow( m_outLowV );
            esource->setOut( address & 1 );
            esource->stampOutput();

            address >>= 1;
        }
    }
    
    //qDebug() << "eBus::setVChanged" << v << address<<m_maxAddr<<m_maxVolt;
}
