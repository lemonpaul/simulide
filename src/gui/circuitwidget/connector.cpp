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

#include "connector.h"
#include "circuit.h"
#include "pin.h"
#include "e-node.h"
#include "utils.h"


Connector::Connector( QObject* parent, QString type, QString id, Pin* startpin, Pin* endpin )
                : Component( parent, type, id )
{
    //m_eNode = 0l;
    m_actLine   = 0;
    m_lastindex = 0;

    if( startpin )
    {
        m_startPin   =  startpin;
        m_startpinid = startpin->itemID();
        setPos( startpin->scenePos() );
    }

    if( endpin )
    {
        m_endPin   = endpin;
        m_endpinid = endpin->itemID();
        m_startPin->setConnector( this );
        m_endPin->setConnector( this );
        m_startPin->setConPin( m_endPin );
        m_endPin->setConPin( m_startPin );
    }
    else
    {
        m_endPin   = 0l;
        m_endpinid = "";
    }

    m_idLabel->setVisible( false );
}
Connector::~Connector() { }

void Connector::remNullLines()      // Remove lines with leght = 0 or aligned
{
    if( m_conLineList.length() < 2 ) return;

    foreach( ConnectorLine* line, m_conLineList )
    {
        int index = m_conLineList.indexOf( line );
        if( index < m_conLineList.length()-1 )      //
        {
            ConnectorLine* line2 = m_conLineList.at( index+1 );

            if( line->dx() == line2->dx() || line->dy() == line2->dy() ) // Lines aligned or null line
            {
                line2->sSetP1( line->p1() );
                remConLine( line  );
            }
        }
    }
    if( m_conLineList.length() < 2 )
    {
        m_lastindex = 0;
        m_actLine   = 0;
    }
    refreshPointList();
}

void Connector::remConLine( ConnectorLine* line  )
{
    int index = m_conLineList.indexOf( line );

    disconnectLines( index, index-1 );
    disconnectLines( index, index+1 );
    connectLines( index-1, index+1 );
    Circuit::self()->removeItem( line );
    m_conLineList.removeOne( line );
}

void Connector::refreshPointList()
{
    if ( m_conLineList.isEmpty() ) return;

    QStringList list;
    QString     data;

    data.setNum( m_conLineList.at(0)->p1().x() );
    list.append( data );
    data.setNum( m_conLineList.at(0)->p1().y() );
    list.append( data );
    int count = m_conLineList.size();
    for(int i=0; i<count; i++)
    {
        data.setNum( m_conLineList.at(i)->p2().x() );
        list.append( data );
        data.setNum( m_conLineList.at(i)->p2().y() );
        list.append( data );
    }
    if( !m_conLineList.isEmpty() ) setPos( m_conLineList.first()->scenePos() );

    setPointList( list );
    //qDebug() << "lines " << count << "connector poinlist" << property( "pointlist" ).toStringList();
}

void Connector::addConLine( ConnectorLine* line, int index )
{
    if( index > 0  && index < m_conLineList.size() ) disconnectLines( index-1, index );

    m_conLineList.insert( index, line );

    Circuit::self()->addItem(line);

    if( index > 0 )
    {
        connectLines( index-1, index );
        m_conLineList.at( index-1 )->sSetP2( line->p1() );
    }

    if( index < m_conLineList.size()-1 )
    {
        if( m_conLineList.size() < 2 ) return;

        connectLines( index, index+1 );
        m_conLineList.at( index+1 )->sSetP1( line->p2() );
    }
}

ConnectorLine* Connector::addConLine( int x1, int y1, int x2, int y2, int index )
{
    ConnectorLine* line = new ConnectorLine( x1, y1, x2, y2, this );

    addConLine( line, index );

    return line;
}

void Connector::connectLines( int index1, int index2 )
{
    if( index1 < 0 || index2 < 0 || index2 > m_conLineList.length()-1 )
        return;

    ConnectorLine* line1 = m_conLineList.at( index1 );
    ConnectorLine* line2 = m_conLineList.at( index2 );

    connect( line1, SIGNAL( yourP1changed(QPoint) ), line2, SLOT( sSetP1(QPoint) ) );
    connect( line2, SIGNAL( yourP2changed(QPoint) ), line1, SLOT( sSetP2(QPoint) ) );
}

void Connector::disconnectLines( int index1, int index2 )
{
    if( index1 < 0 || index2 < 0 || index2 > m_conLineList.length()-1 )
        return;

    ConnectorLine* line1 = m_conLineList.at( index1 );
    ConnectorLine* line2 = m_conLineList.at( index2 );

    disconnect( line1, SIGNAL( yourP1changed(QPoint) ), line2, SLOT( sSetP1(QPoint) ) );
    disconnect( line2, SIGNAL( yourP2changed(QPoint) ), line1, SLOT( sSetP2(QPoint) ) );
}

void Connector::updateConRoute( Pin* nod, QPointF thisPoint )
{
    if( Circuit::self()->pasting() ) return;

    int length = m_conLineList.length();
    ConnectorLine* line;
    ConnectorLine* preline = 0l;

    if( nod == m_startPin )
    {
        line = m_conLineList.first();
        line->sSetP1( thisPoint.toPoint() );
        m_lastindex = 0;

        if( length > 1 )
        {
            preline = m_conLineList.at(1);
            m_actLine = 1;
        }
        else m_actLine = 0;
    }
    else
    {
        line = m_conLineList.last();
        line->sSetP2( togrid( thisPoint ).toPoint() );
        m_lastindex = length-1;

        if( length > 1 )
        {
            preline = m_conLineList.at( m_lastindex-1 );
            if( nod != 0l ) m_actLine = m_lastindex-1;
        }
    }

    if( line->dx() == 0 && line->dy() == 0 && length > 1 )
    {
        Circuit::self()->removeItem( line );
        m_conLineList.removeOne( line );

        if( m_actLine > 0 )  m_actLine -= 1;
    }
    else if( line->dx() != 0 && line->dy() != 0 )
    {
        QPoint point;

        if( m_lastindex == m_actLine )          // Add new corner
        {
            point = line->p2();

            if( abs(line->dx()) > abs(line->dy()) ) point.setY( line->p1().y() );
            else                                    point.setX( line->p1().x() );

            ConnectorLine* newLine = addConLine( point.x(), point.y(), line->p2().x(), line->p2().y(), m_lastindex + 1 );

            if( line->isSelected() ) newLine->setSelected( true );

            line->setP2( point );
            //remNullLines();

        }
        else if( m_lastindex < m_actLine )      // Update first corner
        {
            point = line->p2();

            if( preline->dx() == 0 ) point.setY( line->p1().y() );
            else                     point.setX( line->p1().x() );

            line->setP2( point );

            if( line->dx() == preline->dx() || line->dy() == preline->dy() ) // Lines aligned or null line
            {
                if( line->isSelected() || preline->isSelected())
                {
                    preline->sSetP1( line->p1() );
                    remConLine( line  );
                    if( m_actLine > 0 )  m_actLine -= 1;
                }
            }
        }
        else                                    // Update last corner
        {
            point = line->p1();

            if( preline->dx() == 0 ) point.setY( line->p2().y() );
            else                     point.setX( line->p2().x() );

            line->setP1( point );

            if( line->dx() == preline->dx() || line->dy() == preline->dy() ) // Lines aligned or null line
            {
                if( line->isSelected() || preline->isSelected())
                {
                    preline->sSetP2( line->p2() );
                    remConLine( line  );
                    if( m_actLine > 0 )  m_actLine -= 1;
                }
            }
        }
    }
    //refreshPointList();
}

void Connector::remLines()
{
    while( !m_conLineList.isEmpty() )
    {
        ConnectorLine* line = m_conLineList.takeLast();
        Circuit::self()->removeItem( line );
        delete line;
    }
}


void Connector::move( QPointF delta )
{
    //qDebug() << "Connector::move ..........................";
    if( Circuit::self()->pasting() )
    {
        foreach( ConnectorLine* line, m_conLineList )
            line->move( delta );

        //return;
    }
    //else
    //remNullLines();
    //Component::move( delta );
}

void Connector::setSelected(  bool selected )
{
    //qDebug() <<"\nConnector::setSelected"<<selected;
    foreach( ConnectorLine* line, m_conLineList )
    {
        line->setSelected( selected );
        //qDebug() << line->isSelected();
    }

    Component::setSelected( selected );
}

void Connector::remove()
{
    //qDebug() << "Connector::remove simulator running: " << Simulator::self()->isRunning();
    //qDebug()<<"Connector::remove" << this->objectName();
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();

    if( m_startPin ) m_startPin->reset();
    if( m_endPin )   m_endPin->reset();

    Circuit::self()->conList()->removeOne( this );
    Circuit::self()->removeItem( this );
    remLines();

    if( pauseSim ) Simulator::self()->runContinuous();
}

void Connector::closeCon( Pin* endpin )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();

    m_endPin   = endpin;
    m_endpinid = endpin->objectName();

    QString enodid = "enode";
    enodid.append(m_id);
    enodid.remove("Connector");
    eNode* newEnode = new eNode( enodid );

    eNode* startPinEnode = m_startPin->getEnode();
    eNode* endPinEnode   = m_endPin->getEnode();

    // We will get all ePins from stratPin and endPin nets an add to new eNode
    m_startPin->setConPin( 0l );
    m_endPin->setConPin( 0l );

    QList<ePin*> epins; 

    if( startPinEnode==0l ) { m_startPin->setEnode( newEnode ); }
    else
    {
        // Get connected pins of old eNode and assing to new one.
        // If old eNode ran out of pins will delete itself.
        
        // If it's a Node Pin,  get all connected Pins
        m_startPin->findNodePins(); 
        
        // All connected pins will register in eNode 
        epins = startPinEnode->getSubEpins();
        
        // Set new eNode, ePins will unregister from old eNode
        foreach( ePin* epin, epins ) epin->setEnode( newEnode );
    }

    if( endPinEnode==0l ) { m_endPin->setEnode( newEnode ); }
    else //if( endPinEnode!=startPinEnode )     // Same than startPinEnode
    {
        m_endPin->findNodePins();
        epins = endPinEnode->getSubEpins();
        foreach( ePin* epin, epins ) epin->setEnode( newEnode );
    }

    m_startPin->setConnector( this );
    m_endPin->setConnector( this );

    m_startPin->setConPin( m_endPin );
    m_endPin->setConPin( m_startPin );

    remNullLines();
    //refreshPointList();

    if( pauseSim ) Simulator::self()->runContinuous();
}

void Connector::splitCon( int index, Pin* pin1, Pin* pin2 )
{
    if( !m_endPin ) return;

    pin2->setEnode( enode() );
    pin1->setEnode( enode() );

    disconnectLines( index-1, index );

    QString type = QString("Connector");
    QString id = type;
    id.append( "-" );
    id.append( Circuit::self()->newSceneId() );

    Connector* new_connector = new Connector( Circuit::self(), type, id, pin2 );
    Circuit::self()->addItem(new_connector);

    int newindex = 0;
    int size = m_conLineList.size();
    for( int i = index; i < size; ++i)
    {
        ConnectorLine* lline = m_conLineList.takeAt( index );
        new_connector->lineList()->insert( newindex, lline );

        lline->setParent( new_connector );
        lline->setConnector( new_connector );

        if( newindex > 1 ) new_connector->incActLine();
        ++newindex;
    }

    if( index > 1 )  m_actLine = index-2;
    else             m_actLine = 0;
    
    new_connector->closeCon( m_endPin );    // Close new_connector first please
    closeCon( pin1 );                       // Close this
}

void Connector::updateLines()
{
    foreach( ConnectorLine*  line, m_conLineList ) line->update();
}

QStringList Connector::pointList()             { refreshPointList(); return m_pointList; }
void Connector::setPointList( QStringList pl ) { /*m_pointList.clear(); */m_pointList = pl;}

QString Connector::startPinId()               { return m_startpinid;}
void Connector::setStartPinId( QString pinid) { m_startpinid = pinid; }
QString Connector::endPinId()                 { return m_endpinid; }
void Connector::setEndPinId( QString pinid)   { m_endpinid = pinid; }

QString Connector::enodId() {
    eNode *node = m_startPin->getEnode();
    // node sometimes is NULL leading to crash
    if (node)
        return node->itemId();
    return "";
}
//void Connector::setEnodId( QString enodid ) { m_enodid = enodid; }

Pin* Connector::startPin()             { return m_startPin;}
void Connector::setStartPin( Pin* pin) { m_startPin = pin; }
Pin* Connector::endPin()             { return m_endPin; }
void Connector::setEndPin( Pin* pin) { m_endPin = pin; }

eNode* Connector::enode()            { return m_startPin->getEnode(); }
void   Connector::setEnode( eNode* enode )
{ 
    //m_eNode = enode;
    //m_enodid = m_eNode->itemId();
    if( m_startPin ) m_startPin->setEnode( enode );
    if( m_endPin )   m_endPin->setEnode( enode );
}

double Connector::getVolt()
{
    //if( !m_eNode ) return 0;
    return m_startPin->getVolt();
}

QList<ConnectorLine*>* Connector::lineList() { return &m_conLineList; }

void Connector::incActLine() { if( m_actLine < m_conLineList.size()-1 ) m_actLine += 1;}

#include "moc_connector.cpp"
