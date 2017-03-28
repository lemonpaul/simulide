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
    //qDebug() << "eNode::pinChanged" << m_id << epin << enodeComp;
    //if( m_nodeList[epin] == m_nodeNum  ) return; // Be sure msg doesn't come from this node
    m_nodeList[epin] = enodeComp;
}

void eNode::initialize()
{
    m_single  = false;
    m_changed = false;
    m_currChanged  = false;
    m_admitChanged = false;
    m_needFastUpdate = false;
    m_ePinSubList.clear();
    m_changedFast.clear();
    m_reactiveList.clear();
    m_admitList.clear();
    m_currList.clear();
    m_nodeList.clear();

    //m_totalCurr = 0;
    m_volt = 0;
}

void eNode::stampCurrent( ePin* epin, double data )
{
    m_currList[epin] = data;
    
    //qDebug()<< m_nodeNum << epin << data << m_totalCurr;

    m_currChanged = true;
    
    if( !m_changed ) 
    {
        m_changed = true;
        Simulator::self()->addToChangedNodeList( this );
    }
}

void eNode::stampAdmitance( ePin* epin, double data )
{
    m_admitList[epin] = data;

    m_admitChanged = true;
    
    if( !m_changed ) 
    {
        m_changed = true;
        Simulator::self()->addToChangedNodeList( this );
    }
}

void eNode::setNodeNumber( int n ) { m_nodeNum = n; }

void eNode::stampMatrix()
{
    m_changed = false;
    
    if( m_admitChanged )
    {
        QHash<int, double> admit;
        double totalAdmit = 0;
        double adm;
        int    enode;
        
        QHashIterator<ePin*, double> i(m_admitList); // ePin-Admit
        while ( i.hasNext() ) 
        {
            i.next();

            adm   = i.value(); //m_admitList[epin];
            if( !m_single ) 
            {
                ePin* epin = i.key();
                enode = m_nodeList[epin];
                admit[enode] += adm;
            }
            totalAdmit   += adm;
        }
        
        if( m_single ) m_totalAdmit = totalAdmit;   // single element in matrix
        else 
        {
            QHashIterator<int, double> ai(admit); // iterate admitance hash: eNode-Admit
            while ( ai.hasNext() ) 
            {
                ai.next();
                enode = ai.key();
                if( enode > 0 )
                    CircMatrix::self()->stampMatrix( m_nodeNum, enode, -ai.value() );
            }
            
            CircMatrix::self()->stampMatrix( m_nodeNum, m_nodeNum, totalAdmit );
        }
        m_admitChanged = false;
    }
    
    if( m_currChanged )
    {
        double totalCurr  = 0;
        foreach( double current, m_currList ) totalCurr  += current;

        if( m_single ) setVolt( totalCurr/m_totalAdmit ); // single element in matrix
        else CircMatrix::self()->stampCoef( m_nodeNum, totalCurr );
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
            
        foreach( eElement* el, m_reactiveList )
            Simulator::self()->addToReactiveList( el );
            
        foreach( eElement* el, m_nonLinear ) 
            Simulator::self()->addToNoLinList( el );
    }
}
double eNode::getVolt() { return m_volt; }

QList<ePin*> eNode::getEpins()    { return m_ePinList; }

QList<ePin*> eNode::getSubEpins() { return m_ePinSubList; }

void eNode::addEpin( ePin* epin ) 
{ 
    //qDebug() << "eNode::addEpin" << m_id << QString::fromStdString(epin->getId());
    if( !m_ePinList.contains(epin)) m_ePinList.append(epin); 
}

void eNode::addSubEpin( ePin* epin ) 
{ 
    //qDebug() << "eNode::addSubEpin" << m_id << QString::fromStdString(epin->getId());
    if( !m_ePinSubList.contains(epin)) m_ePinSubList.append(epin); 
}

void eNode::remEpin( ePin* epin )
{
    //qDebug() << "eNode::remEpin" << m_id << QString::fromStdString(epin->getId());
    if( m_ePinList.contains(epin) )    m_ePinList.removeOne(epin);
    if( m_ePinSubList.contains(epin) ) m_ePinSubList.removeOne(epin);

    // If No epins then remove this enode
    if( m_ePinList.size()== 0 ) Simulator::self()->remFromEnodeList( this, true );
}

void eNode::addToChangedFast( eElement* el )   
{ 
    if( !m_changedFast.contains(el) ) m_changedFast.append(el); 
    m_needFastUpdate = true;
}

void eNode::remFromChangedFast( eElement* el ) 
{ 
    m_changedFast.removeOne(el); 
    if( m_changedFast.isEmpty() & m_nonLinear.isEmpty()) m_needFastUpdate = false;
}

void eNode::addToReactiveList( eElement* el )
{ 
    if( !m_reactiveList.contains(el) ) m_reactiveList.append(el);
}

void eNode::remFromReactiveList( eElement* el )
{ 
    m_reactiveList.removeOne(el);
}

void eNode::addToNoLinList( eElement* el )
{
    if( !m_nonLinear.contains(el) ) m_nonLinear.append(el);
    m_needFastUpdate = true;
}

void eNode::remFromNoLinList( eElement* el )
{
    m_nonLinear.removeOne(el);
    if( m_changedFast.isEmpty() & m_nonLinear.isEmpty()) m_needFastUpdate = false;
}

int eNode::getNodeNumber() { return m_nodeNum; }

QString eNode::itemId() { return m_id; }

