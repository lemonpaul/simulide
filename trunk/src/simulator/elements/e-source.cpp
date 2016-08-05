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

#include "e-source.h"
#include "simulator.h"

eSource::eSource( string id, ePin* epin ) : eElement( id )
{
    m_ePin.resize(1);
    m_ePin[0] = epin;
    m_out     = false;
    m_inverted = false;

    m_voltHigh = cero_doub;
    m_voltLow  = cero_doub;
    m_voltOut  = cero_doub;
    m_imp      = cero_doub;
    m_admit    = 1/m_imp;

    QString nodId = QString::fromStdString(id);

    m_scrEnode = new eNode( nodId+"scr" );
    m_scrEnode->setNodeNumber(0);
    Simulator::self()->remFromEnodeList( m_scrEnode, /*delete=*/ false );
}
eSource::~eSource(){ delete m_scrEnode; }

/*void eSource::setEpin( ePin* epin )
{
    m_ePin[0] = epin;
}*/

void eSource::initialize()
{
    m_ePin[0]->setEnodeComp( m_scrEnode );
}

void eSource::stamp()
{
    //qDebug() <<"eSource::stamp"<< m_out;
    m_ePin[0]->stampAdmitance( m_admit );
    stampOutput();
}

void eSource::setVoltHigh( double v )
{
    m_voltHigh = v;
    if( m_out ) m_voltOut = v;
}

void eSource::setVoltLow( double v )
{
    m_voltLow = v;
    if( !m_out ) m_voltOut = v;
}

void eSource::stampOutput()                               // Stamp Output
{
    m_scrEnode->setVolt(m_voltOut);

    m_ePin[0]->stampCurrent( m_voltOut/m_imp );
}

void eSource::setOut( bool out )           // Set Output to Hight or Low
{
    if( m_inverted ) m_out = !out;
    else             m_out =  out;

    double v = m_voltLow;
    if( m_out ) v = m_voltHigh;

    m_voltOut = v;
}

void eSource::setInverted( bool inverted )
{
    if( inverted == m_inverted ) return;

    m_inverted = inverted;
    setOut( m_out );
}

void eSource::setImp( double imp )
{
    m_imp = imp;
    m_admit = 1/m_imp;
    m_ePin[0]->stampAdmitance( m_admit );
}

ePin* eSource::getEpin()
{
    return m_ePin[0];
}

double eSource::getVolt()
{
    return m_ePin[0]->getVolt();
}
