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

#include <qstring.h>
#include <stdint.h>

#include "piccomponentpin.h"
#include "piccomponent.h"
#include "baseprocessor.h"
#include "simulator.h"

#include "stimuli.h"
#include "ioports.h"
#include "pic-processor.h"

PICComponentPin::PICComponentPin( McuComponent* mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
               : McuComponentPin( mcu, id, type, label, pos, xpos, ypos, angle )
{
    m_pos = pos;
    m_PicProcessor  = 0l;
    m_pIOPIN        = 0l;
}
PICComponentPin::~PICComponentPin(){}

void PICComponentPin::attach( pic_processor *PicProcessor )
{
    if( m_PicProcessor ) return;
    
    m_PicProcessor = PicProcessor;
    
    IOPIN* iopin = m_PicProcessor->get_pin( m_pos );

    if( m_id.startsWith("R") || m_id.startsWith("GP") )
    {
        m_pinType = 1;
        
        m_port = m_id.at(1).toLatin1();
        m_pinN = m_id.mid(2,1).toInt();

        if( !iopin )
        {
            qDebug() << "PICComponentPin::attach : iopin is NULL: "<< m_id << endl;
            return;
        }
        if( m_pIOPIN )
        {
            qDebug() << "PICComponentPin::attach :Already have an iopin" << endl;
            return;
        }
        m_pIOPIN = iopin;
        m_pIOPIN->setPicPin( this );
        if( m_pIOPIN->getType() == OPEN_COLLECTOR )
        {
            m_openColl = true;
            eSource::setVoltHigh( 0 );
        }
    }
    else if( m_id.startsWith("MCLR") )
    {
        m_pinType = 21;
    }
    m_attached = true;
}

void PICComponentPin::setVChanged()
{
    if( !m_isInput ) return;      // Nothing to do if pin is output

    double volt = m_ePin[0]->getVolt();
    //qDebug() << "PICComponentPin::setVChanged "<< m_id <<volt;
    
    if( m_pinType == 1 )                                 // Is an IO Pin
    {
        m_pIOPIN->set_nodeVoltage(volt);
    }
    else if( m_pinType == 21 ) // reset
    {
        if( volt < 3 )  BaseProcessor::self()->hardReset( true );
        else            BaseProcessor::self()->hardReset( false );
    }
}

void PICComponentPin::update_direction( bool out )
{
    //qDebug() << "PICComponentPin::update_direction "<< m_id << out;
    m_isInput = !out;
    
    if( out )
    {
        eSource::setImp( 40 );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->remFromChangedFast(this);
    }
    else
    {
        eSource::setImp( high_imp );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->addToChangedFast(this);
    }
}

void PICComponentPin::update_pullup( bool pullup )
{
    if( !m_isInput ) return;
    
    //qDebug() << "PICComponentPin::update_pullup "<< m_id << pullup;

    if( pullup )                         // Activate pullup
    {
        eSource::setImp( 1e5 );
        m_voltOut = m_voltHigh;
    }
    else                                 // Deactivate pullup
    {
        eSource::setImp( high_imp );
        m_voltOut = m_voltLow;
    }
    if( !(m_ePin[0]->isConnected()) ) 
    {
        m_pIOPIN->set_nodeVoltage( pullup ? 5:0);
        return;
    }
    
    m_ePin[0]->stampCurrent( m_voltOut/m_imp );
    //if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

void PICComponentPin::update_state( bool state )
{
    //qDebug() << "PICComponentPin::update_state "<< m_id << state << m_openColl;
    if( !(m_ePin[0]->isConnected()) ) return;
    if( m_isInput )  return;

    if( state == m_state ) return;
    m_state = state;

    if( m_openColl )
    {
        if( state ) eSource::setImp( high_imp );
        else        eSource::setImp( 40 );

        stamp();
    }
    else
    {
        eSource::setOut( state );
        eSource::stampOutput();
    }

    //if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

#include "moc_piccomponentpin.cpp"
