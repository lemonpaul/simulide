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

#include <QtDebug>

//#include "e-node.h"
#include "simulator.h"
//#include "connector.h"
#include "e-element.h"
//#include "e-pin.h"


eNode::eNode( QString id )
{
    m_id = id;
    m_nodeNum = 0;
    m_numCons = 0;
    m_volt    = 0;
    initialize();

    Simulator::self()->addToEnodeList( this );
}
eNode::~eNode(){ /*Simulator::self()->remFromEnodeList( this );*/ }

void eNode::pinChanged( ePin* epin, int enodeNum )
{
    if( m_nodeList[epin] == m_nodeNum  ) return; // Be sure msg doesn't come from this node
    m_nodeList[epin] = enodeNum;
    //addEpin(epin);
}

void eNode::stampCurrent( ePin* epin, double data )
{
    if( m_nodeList[epin] == m_nodeNum  ) return; // Be sure msg doesn't come from this node
    m_currList[epin] = data;

    if( m_initialized ) stampMatrix();
}

void eNode::stampAdmitance( ePin* epin, double data )
{
    if( m_nodeList[epin] == m_nodeNum ) return; // Be sure msg doesn't come from this node
    m_admitList[epin] = data;

    if( m_initialized ) stampMatrix();
}

void eNode::setNodeNumber( int n ) { m_nodeNum = n; }

void eNode::initialize()
{
    m_initialized = false;
    m_changedList.clear();
    m_admitList.clear();
    m_currList.clear();
    m_nodeList.clear();
    m_volt = 0;
}

void eNode::stampMatrix()
{
    QList<ePin*> epins = m_nodeList.uniqueKeys();
    QHash<int, double> admit;
    double totalCurr  = 0;
    double totalAdmit = 0;

    foreach( ePin* epin, epins )
    {
        int    enode = m_nodeList[epin];
        double adm   = m_admitList[epin];
        admit[enode] += adm;
        totalAdmit   += adm;

        totalCurr  += m_currList[epin];
    }
    foreach( int enode, m_nodeList )
        if( enode > 0 )
            CircMatrix::self()->stampMatrix( m_nodeNum, enode, -admit[enode] );

    CircMatrix::self()->stampMatrix( m_nodeNum, m_nodeNum, totalAdmit );
    CircMatrix::self()->stampCoef( m_nodeNum, totalCurr );
    m_initialized = true;
}

void  eNode::setVolt( double v )
{
    //qDebug() << m_id << m_volt << v;
    if( m_volt != v )
    {
        //qDebug() << m_id << "setVChanged";
        m_volt = v;
        foreach( eElement* el, m_changedList ) el->setVChanged();
    }
}
double eNode::getVolt() { return m_volt; }

/*void eNode::connectorChanged( int c )
{
    m_numCons += c;
    if( m_numCons == 0 )        // No connectors using this eNode.
        Simulator::self()->remFromEnodeList( this, true );
}*/

QList<ePin*> eNode::getEpins()    { return m_ePinList; }
void eNode::addEpin( ePin* epin ) { if( !m_ePinList.contains(epin)) m_ePinList.append(epin); }
void eNode::remEpin( ePin* epin )
{
    m_ePinList.removeOne(epin);

    // If 0 epins then remove this enode
    if( m_ePinList.size()== 0 ) Simulator::self()->remFromEnodeList( this, true );
}

void eNode::addToChangedList( eElement* el )   { if( !m_changedList.contains(el) ) m_changedList.append(el); }
void eNode::remFromChangedList( eElement* el ) { m_changedList.removeOne(el); }

int eNode::getNodeNumber() { return m_nodeNum; }

QString eNode::itemId() { return m_id; }
