/***************************************************************************
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

#include "connector.h"
#include "gate.h"


Gate::Gate( QObject* parent, QString type, QString id, int inputs )
       : Component( parent, type, id ), eGate( id.toStdString(), inputs )
{
    m_inputPin.resize( m_numInputs );
    
    for( int i=0; i<m_numInputs; i++ )
        m_inputPin[i] = new Pin( 180, QPoint(-16-8,-8*m_numInputs+i*16+8 )
                          , m_id+"-in"+QString::number(i), i, this );
    
    m_outputPin = new Pin( 0, QPoint( 16+8,-8*m_numOutputs+0*16+8 )
                          , m_id+"-out", 1, this );
}
Gate::~Gate(){
    for( int i=0; i<m_numInputs; i++ ) delete m_inputPin[i];
    delete m_outputPin;
}

void Gate::remove()
{
    for( int i=0; i<m_numInputs; i++ )
        if( m_inputPin[i]->isConnected() ) m_inputPin[i]->connector()->remove();

    if( m_outputPin->isConnected() ) m_outputPin->connector()->remove();
    
    Component::remove();
}

void Gate::initialize()
{
    // get eNode for each Gate pin and assing to eGate ePins
    eNode* enod;
    for( int i=0; i<m_numInputs; i++ )
    {
       enod = m_inputPin[i]->getEnode();
       m_input[i]->getEpin()->setEnode(enod);
    }
    
    enod = m_outputPin->getEnode();
    m_output[0]->getEpin()->setEnode(enod);
    
    eGate::initialize();
}

void Gate::setNumInps( int inputs )
{
    if( inputs < 1 ) return;
    for( int i=0; i<m_numInputs; i++ ) delete m_inputPin[i];
        
    eLogicDevice::setNumInps( inputs );
    
    m_inputPin.resize( m_numInputs );
    
    for( int i=0; i<m_numInputs; i++ )
        m_inputPin[i] = new Pin( 180, QPoint(-16-8,-8*m_numInputs+i*16+8 )
                          , m_id+"-in"+QString::number(i), i, this );
}

void Gate::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    if( m_inverted )
    {
        Component::paint( p, option, widget );
        QPen pen = p->pen();
        pen.setWidth(2);
        p->setPen(pen);
        QRect rect( 14,-3, 6, 6 );     // Paint and draw circle of inverted pin
        p->drawEllipse(rect);
    }
}

#include "moc_gate.cpp"
