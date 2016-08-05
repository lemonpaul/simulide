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

#include "e-node.h"
#include "simulator.h"
#include "e-element.h"


eNode::eNode( QString id )
{
    m_id = id;
    m_nodeNum = 0;
    m_numCons = 0;
    m_volt    = 0;
    initialize();
    //qDebug() << "+eNode" << m_id;

    Simulator::self()->addToEnodeList( this );
}
eNode::~eNode(){ /*qDebug() << "~eNode" << m_id;Simulator::self()->remFromEnodeList( this );*/ }

void eNode::pinChanged( ePin* epin, int enodeComp ) // Add node at other side of pin
{
    if( m_nodeList[epin] == m_nodeNum  ) return; // Be sure msg doesn't come from this node
    m_nodeList[epin] = enodeComp;
}

void eNode::initialize()
{
    m_currChanged  = false;
    m_admitChanged = false;
    m_ePinSubList.clear();
    m_changedFast.clear();
    m_changedSlow.clear();
    m_admitList.clear();
    m_currList.clear();
    m_nodeList.clear();
    
    //m_totalCurr = 0;
    m_volt = 0;
}

void eNode::stampCurrent( ePin* epin, double data )
{
    if( m_nodeList[epin] == m_nodeNum  ) return; // Be sure msg doesn't come from this node
    m_currList[epin] = data;
    
    //qDebug()<< m_nodeNum << epin << data << m_totalCurr;

    m_currChanged = true;
}

void eNode::stampAdmitance( ePin* epin, double data )
{
    if( m_nodeList[epin] == m_nodeNum ) return; // Be sure msg doesn't come from this node
    m_admitList[epin] = data;

    m_admitChanged = true;
}

void eNode::setNodeNumber( int n ) { m_nodeNum = n; }

void eNode::stampMatrix()
{
    if( m_admitChanged )
    {
        QHash<int, double> admit;
        double totalAdmit = 0;
        double adm;
        int    enode;
        
        QHashIterator<ePin*, double> i(m_admitList);
        while ( i.hasNext() ) 
        {
            i.next();
            ePin* epin = i.key();

            enode = m_nodeList[epin];
            adm   = i.value(); //m_admitList[epin];
            admit[enode] += adm;
            totalAdmit   += adm;
        }

        QHashIterator<int, double> ai(admit); // iterate admitance hash: node-admit
        while ( ai.hasNext() ) 
        {
            ai.next();
            enode = ai.key();
            if( enode > 0 )
                CircMatrix::self()->stampMatrix( m_nodeNum, enode, -ai.value() );
        }
        CircMatrix::self()->stampMatrix( m_nodeNum, m_nodeNum, totalAdmit );

        m_admitChanged = false;
    }
    
    if( m_currChanged )
    {
        double totalCurr  = 0;
        foreach( double current, m_currList ) totalCurr  += current;

        CircMatrix::self()->stampCoef( m_nodeNum, totalCurr );
        //m_currList.clear();
        m_currChanged  = false;
    }
}

void  eNode::setVolt( double v )
{
    //qDebug() << m_id << m_volt << v;
    if( m_volt != v )
    {
        //qDebug() << m_id << "setVChanged";
        m_volt = v;
        
        foreach( eElement* el, m_changedFast ) 
            Simulator::self()->addToChangedFast( el ); // el->setVChanged();
            
        foreach( eElement* el, m_changedSlow ) 
            Simulator::self()->addToChangedSlow( el );
            
        foreach( eElement* el, m_nonLinear ) 
            Simulator::self()->addToNoLinList( el );
    }
}
double eNode::getVolt() { return m_volt; }

QList<ePin*> eNode::getEpins()    { return m_ePinList; }

QList<ePin*> eNode::getSubEpins() { return m_ePinSubList; }

void eNode::addEpin( ePin* epin ) 
{ 
    if( !m_ePinList.contains(epin)) m_ePinList.append(epin); 
}

void eNode::addSubEpin( ePin* epin ) 
{ 
    //qDebug() << "eNode::addSubEpin" << m_id << QString::fromStdString(epin->getId());
    if( !m_ePinSubList.contains(epin)) m_ePinSubList.append(epin); 
}

void eNode::remEpin( ePin* epin )
{
    if( m_ePinList.contains(epin) )    m_ePinList.removeOne(epin);
    if( m_ePinSubList.contains(epin) ) m_ePinSubList.removeOne(epin);

    // If No epins then remove this enode
    if( m_ePinList.size()== 0 ) Simulator::self()->remFromEnodeList( this, true );
}

void eNode::addToChangedFast( eElement* el )   
{ 
    if( !m_changedFast.contains(el) ) m_changedFast.append(el); 
}

void eNode::remFromChangedFast( eElement* el ) 
{ 
    m_changedFast.removeOne(el); 
}

void eNode::addToChangedSlow( eElement* el )   
{ 
    if( !m_changedSlow.contains(el) ) m_changedSlow.append(el); 
}

void eNode::remFromChangedSlow( eElement* el ) 
{ 
    m_changedSlow.removeOne(el); 
}

void eNode::addToNoLinList( eElement* el )
{
    if( !m_nonLinear.contains(el) ) m_nonLinear.append(el);
}

void eNode::remFromNoLinList( eElement* el )
{
    m_nonLinear.removeOne(el);
}

int eNode::getNodeNumber() { return m_nodeNum; }

QString eNode::itemId() { return m_id; }

