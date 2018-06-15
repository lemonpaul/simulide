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
 
#ifndef NO_PIC
#include "piccomponentpin.h"
#include "piccomponent.h"
#include "simulator.h"

PICComponentPin::PICComponentPin( McuComponent* mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
    : McuComponentPin( mcu, id, type, label, pos, xpos, ypos, angle )
{
    m_pos = pos;
    //m_channel = -1;
    m_Vth = 0;
    m_PicProcessor  = 0l;
    m_pStimulusNode = 0l;
    m_pIOPIN        = 0l;
}
PICComponentPin::~PICComponentPin(){}

void PICComponentPin::attach( pic_processor *PicProcessor )
{
    if( m_PicProcessor ) return;
    
    m_PicProcessor = PicProcessor;

    if( m_id.startsWith("R") || m_id.startsWith("GP") )
    {
        m_port = m_id.at(1).toLatin1();
        m_pinN = m_id.mid(2,1).toInt();

        IOPIN * iopin = m_PicProcessor->get_pin( m_pos );

        if( !iopin )
        {
            qDebug() << "PICComponentPin::attach : iopin is NULL: "<< m_id << endl;
            return;
        }
        if( m_pStimulusNode )
        {
            qDebug() << "PICComponentPin::attach :Already have a node stimulus" << endl;
            return;
        }
        if( m_pIOPIN )
        {
            qDebug() << "PICComponentPin::attach :Already have an iopin" << endl;
            return;
        }
        m_pIOPIN = iopin;
        m_pStimulusNode = new Stimulus_Node(m_id.toLatin1());
        m_pStimulusNode->attach_stimulus(iopin);
        m_pStimulusNode->attach_stimulus(this);

        setImp( high_imp );
        setOut( 0 );
    }
    m_attached = true;
}

void PICComponentPin::setVChanged()
{
    if(!m_PicProcessor) return;

    if( m_imp!=high_imp ) return;      // Nothing to do if pin is output

    double volt = m_ePin[0]->getVolt();
    
    if( m_pIOPIN ) m_pIOPIN->set_nodeVoltage(volt);
}

void PICComponentPin::set_nodeVoltage( double v )     // Called by Gpsim
{
    //qDebug() << "PICComponentPin::set_nodeVoltage "<< m_id << v;
    if( !(m_ePin[0]->isConnected()) ) return;
    
    if( v == m_Vth ) return;
    m_Vth = v;
    
    //qDebug() << "PICComponentPin::set_nodeVoltage "<< m_id << v;
    
    if( m_pIOPIN->get_direction() == IOPIN::DIR_INPUT )
    {
        if( eSource::imp() != high_imp )
        {
            eSource::setImp( high_imp );
            if( m_ePin[0]->isConnected() && m_attached )
                m_ePin[0]->getEnode()->addToChangedFast(this);
        }
        return;      // Nothing to do if pin is input
    }
    else if( eSource::imp() != 40 )
    {
        eSource::setImp( 40 );
        if( m_ePin[0]->isConnected() && m_attached )
            m_ePin[0]->getEnode()->remFromChangedFast(this);
    }

    eSource::setOut( v > 2.5 );
    eSource::stampOutput();
    if( m_ePin[0]->getEnode()->needFastUpdate() ) 
    {
        Simulator::self()->runExtraStep();
    }
}

double PICComponentPin::get_Vth( )                    // Called by Gpsim
{
    //qDebug() << "PICComponentPin::get_Vth "<< m_id;
    return m_ePin[0]->getVolt();
}
#include "moc_piccomponentpin.cpp"
#endif
