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

#include "circuit.h"
#include "itemlibrary.h"
#include "mainwindow.h"
#include "node.h"
#include "utils.h"

Circuit*  Circuit::m_pSelf = 0l;

Circuit::Circuit( qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent)
    : QGraphicsScene(x, y, width, height, parent)
{
    setObjectName( "Circuit" );
    setParent( parent );
    m_graphicView = parent;
    m_scenerect.setRect( x, y, width, height );
    setSceneRect( QRectF(x, y, width, height) );

    m_pSelf = this;

    m_drawGrid = true;
    m_pasting = false;
    m_con_started = false;
    new_connector = 0l;
    m_seqNumber   = 0;
}

Circuit::~Circuit()
{
    // Avoid PropertyEditor problem: comps not unregistered

    QPropertyEditorWidget::self()->removeObject( this );

    foreach( Component* comp, m_compList )
    {
        QPropertyEditorWidget::self()->removeObject( comp );
    }
}

QList<Component*>* Circuit::compList() { return &m_compList; }
QList<Component*>* Circuit::conList()  { return &m_conList; }

int Circuit::nlAcc()
{
    return Simulator::self()->nlAcc();
}

void  Circuit::setNlAcc( int ac )
{
    Simulator::self()->setNlAcc( ac );
}

int Circuit::reactStep()
{
    return Simulator::self()->reaClock();
}

void  Circuit::setReactStep( int steps )
{
    Simulator::self()->setReaClock( steps );
}

int Circuit::noLinStep()
{
    return Simulator::self()->noLinClock();
}

void Circuit::setNoLinStep( int steps )
{
    Simulator::self()->setNoLinClock( steps );
}

int  Circuit::circSpeed()
{
    return Simulator::self()->simuRate();
}
void Circuit::setCircSpeed( int rate )
{
    Simulator::self()->simuRateChanged( rate );
}

void Circuit::removeItems()                     // Remove Selected items
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();

    saveState();

    foreach( Component* comp, m_compList )
    {
        bool isNode = comp->objectName().contains( "Node" ); // Don't remove Graphical Nodes
        if( comp->isSelected() && !isNode )  removeComp( comp );
    }

    if( pauseSim ) Simulator::self()->runContinuous();
}

void Circuit::removeComp( Component* comp )
{
    comp->remove();
    QPropertyEditorWidget::self()->removeObject( comp );
    compList()->removeOne( comp );
    removeItem( comp );
    comp->deleteLater();
}

void Circuit::remove() // Remove everything
{
    //qDebug() << m_compList.size();
    foreach( Component* comp, m_compList )
    {
        //qDebug() << "Circuit::remove" << comp->itemID();

        // Don't remove internal items
        bool isNumber = false;
        comp->objectName().split("-").last().toInt( &isNumber ); // TODO: Find a proper way !!!!!!!!!!!
        // Don't remove Graphical Nodes
        bool isNode = comp->objectName().contains( "Node" );

        if( isNumber && !isNode )  removeComp( comp );
    }

}

void Circuit::saveState()
{
    //qDebug() << "saving state";
    foreach( QDomDocument* doc, m_redoStack ) delete doc;

    m_redoStack.clear();

    circuitToDom();
    m_undoStack.append( new QDomDocument() );
    m_undoStack.last()->setContent( m_domDoc.toString() );
    
    QString title = MainWindow::self()->windowTitle();
    if (!title.endsWith('*'))
        MainWindow::self()->setWindowTitle(title+'*');
}

/*void Circuit::setChanged()
{
    m_changed = true;
}*/

bool Circuit::drawGrid()
{
    return m_drawGrid;
}
void Circuit::setDrawGrid( bool draw )
{
    m_drawGrid = draw;
}

void Circuit::drawBackground ( QPainter*  painter, const QRectF & rect )
{
    Q_UNUSED( rect );
    /*painter->setBrush(QColor( 255, 255, 255 ) );
    painter->drawRect( m_scenerect );*/

    painter->setBrush(QColor( 240, 240, 210 ) );
    painter->drawRect( m_scenerect );
    painter->setPen( QColor( 210, 210, 210 ) );

    if( !m_drawGrid ) return;
    
    int startx = int(m_scenerect.x());///2;
    int endx   = int(m_scenerect.width())/2;
    int starty = int(m_scenerect.y());///2;
    int endy   = int(m_scenerect.height())/2;

    for (int i = 4; i<endx; i+=8)
    {
        painter->drawLine( i, starty, i, endy );
        painter->drawLine(-i, starty,-i, endy );
    }
    for (int i = 4; i<endy; i+=8)
    {
        painter->drawLine( startx, i, endx, i);
        painter->drawLine( startx,-i, endx,-i);
    }
    
}

void Circuit::importCirc(  QPointF eventpoint  )
{
    //saveState();

    m_pasting = true;

    m_deltaMove = QPointF( 160, 160 );//togrid(eventpoint);

    const QString dir = m_fileName;
    QString fileName = QFileDialog::getOpenFileName( 0l, tr("Load Circuit"), dir,
                                          tr("Circuits (*.simu);;All files (*.*)"));

    if( !fileName.isEmpty() && fileName.endsWith(".simu") )
        loadCircuit( fileName );
    m_pasting = false;

    //copy( eventpoint );
    //removeItems();
    //paste( eventpoint );
}

void Circuit::loadCircuit( QString &fileName )
{
    m_fileName = fileName;
    QFile file( fileName );

    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        QMessageBox::warning(0l, tr("MainWindow::loadCircuit"),
        tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    saveState();

    if (!m_domDoc.setContent(&file))
    {
        QMessageBox::warning(0l, tr("MainWindow::loadCircuit"),
        tr("Cannot set file %1\nto DomDocument").arg(fileName));
        file.close();
        return;
    }
    file.close();

    //m_seqNumber = 1;

    loadDomDoc( &m_domDoc );

    m_domDoc.clear();
}

QString Circuit::getCompId( QString name )
{
    QStringList nameSplit = name.split("-");
    if( nameSplit.isEmpty() ) return "";

    QString compId  = nameSplit.takeFirst();
    if( nameSplit.isEmpty() ) return "";

    QString compNum = nameSplit.takeFirst();

    int num = 0;
    for( int i=compNum.length(); i>0; i-- )
    {
        bool ok = false;
        num = compNum.left(i).toInt(&ok);
        if( ok ) break;
    }
    return compId+"-"+QString::number(num);
}

Pin* Circuit::findPin( int x, int y, QString id )
{
    QRectF itemRect = QRectF ( x-4, y-4, 8, 8 );

    QList<QGraphicsItem*> list = items( itemRect ); // List of items in (x, y)
    foreach( QGraphicsItem* it, list )
    {
        Pin* pin =  qgraphicsitem_cast<Pin*>( it );
//qDebug()<< it->scenePos() << pointList.first().toInt()<< pointList.at(1).toInt();
        if( pin )
        {
            if( pin->itemID().left(1) == id.left(1) ) // Test if names start by same letter
            {
                //qDebug() <<"FIND BY POS: startpin"<<startpinid<<pinId;
                return pin;
            }
        }
    }
    foreach( QGraphicsItem* it, list ) // Not found by first letter, take first Pin
    {
        Pin* pin =  qgraphicsitem_cast<Pin*>( it );
//qDebug()<< it->scenePos() << pointList.first().toInt()<< pointList.at(1).toInt();
        if( pin ) return pin;
    }

    return 0l;
}

void Circuit::loadDomDoc( QDomDocument* doc )
{
    int firstSeqNumber = m_seqNumber+1;

    QDomElement root = doc->documentElement();
    QString docType = root.attribute("type");
    QDomNode    node = root.firstChild();
    QList<Component*> compList;   // Component List
    QList<Component*> conList;    // Connector List
    QList<Node*>      jointList;  // Joint List
    QHash<QString, QString> idMap;
    QHash<QString, eNode*> nodMap;

    while( !node.isNull() )
    {
        QDomElement   element = node.toElement();
        const QString tagName = element.tagName();

        if( tagName == "item" )
        {
            QString objNam = element.attribute( "objectName"  ); // Data in simu file
            QString type   = element.attribute( "itemtype"  );
            QString itemId = element.attribute( "id"  );

            QString id = objNam.split("-").first()+"-"+newSceneId(); // Create new id
            idMap[objNam] = id;                              // Map simu id to new id

            element.setAttribute( "objectName", id  );

            if( itemId == objNam ) // id = objectName => no id changed, so apply new id
                element.setAttribute( "id", id );

            if( type == "Connector" )
            {
                Pin* startpin  = 0l;
                Pin* endpin    = 0l;
                QString startpinid = element.attribute( "startpinid" );
                QString endpinid   = element.attribute( "endpinid" );
                QString startCompName = getCompId( startpinid );
                QString endCompName   = getCompId( endpinid );

                QStringList keys = idMap.keys();
                foreach( QString oldname, keys ) // Find new pin names
                {
                    if( startCompName == oldname ) startpinid.replace( oldname, idMap[oldname] );
                    if( endCompName   == oldname ) endpinid.replace( oldname, idMap[oldname] );
                }
                QList<QGraphicsItem*> itemlist = items();

                foreach( QGraphicsItem* item, itemlist )  // Look for start and end pins of this connector
                {
                    Pin* thispin =  qgraphicsitem_cast<Pin* >( item );

                    if( thispin )
                    {
                        QString thispinid = thispin->objectName();

                        int thisSeqNumber = thispinid.split("-").at(1).toInt();
                        if( thisSeqNumber < firstSeqNumber ) continue; // Avoid connecting with "pre-paste" items

                        if     ( thispinid == startpinid ) { startpin = thispin; }
                        else if( thispinid == endpinid )   { endpin   = thispin; }
                    }
                }
                if( !m_pasting )// When pasting we cannot find pins by pos, they are in this moment in same pos that originals
                {
                    if( !startpin ) // Pin not found by name... find it by pos
                    {
                        QStringList pointList   = element.attribute( "pointList" ).split(",");
                        int itemX = pointList.first().toInt();
                        int itemY = pointList.at(1).toInt();

                        startpin = findPin( itemX, itemY, startpinid );
                    }
                    if( !endpin ) // Pin not found by name... find it by pos
                    {
                        QStringList pointList   = element.attribute( "pointList" ).split(",");
                        int itemX = pointList.at(pointList.size()-2).toInt();
                        int itemY = pointList.last().toInt();

                        endpin = findPin( itemX, itemY, endpinid );
                    }
                }
                if( startpin && endpin )    // Create Connector
                {
                    Connector* con  = new Connector( this, type, id, startpin, endpin );

                    element.setAttribute( "startpinid", startpin->itemID() );
                    element.setAttribute(   "endpinid", endpin->itemID() );

                    loadProperties( element, con );

                    QString enodeId = element.attribute( "enodeid" );
                    eNode*  enode   = nodMap[enodeId];
                    if( !enode )                    // Create eNode and add to enodList
                    {
                        enode = new eNode( "Circ_eNode-"+newSceneId() );
                        nodMap[enodeId] = enode;
                    }
                    con->setEnode( enode );

                    QStringList plist = con->pointList();   // add lines to connector
                    int p1x = plist.first().toInt();
                    int p1y = plist.at(1).toInt();
                    int p2x = plist.at(plist.size()-2).toInt();
                    int p2y = plist.last().toInt();

                    con->addConLine( con->x(),con->y(), p1x, p1y, 0 );

                    int count = plist.size();
                    for (int i=2; i<count; i+=2)
                    {
                        p2x = plist.at(i).toInt();
                        p2y = plist.at(i+1).toInt();
                        con->addConLine( p1x, p1y, p2x, p2y, i/2 );
                        p1x = p2x;
                        p1y = p2y;
                    }
                    con->remNullLines();
                    conList.append( con );
                }
                else // Start or End pin not found
                {
                    if( !startpin ) qDebug() << "\n   ERROR!!  Circuit::loadCircuit:  null startpin in " << id << startpinid;
                    if( !endpin )   qDebug() << "\n   ERROR!!  Circuit::loadCircuit:  null endpin in " << id << endpinid;
                }
            }
            else if( type == "Node")
            {
                Node* joint = new Node( this, type, id );
                loadProperties( element, joint );
                compList.append( joint );
                jointList.append( joint );
            }
            else if( type == "LEDSMD" ); // TODO: this type shouldnt be saved to circuit
                                         // bcos is created inside another component, for example boards
            else
            {
                Component* item = createItem( type, id );
                if( item )
                {
                    loadProperties( element, item );
                    if( docType == "simu" ) // version<0.1.5
                    {
                        QString itemType = item->itemType();

                        if( itemType == "FlipFlopD" )
                        {
                            item->setProperty( "S_R_Inverted", true );
                        }
                        else if( itemType == "FlipFlopJK" )
                        {
                            item->setProperty( "S_R_Inverted", true );
                        }
                        else if( (itemType == "Counter") || (itemType == "ShiftReg") )
                        {
                            item->setProperty( "Reset_Inverted", true );
                        }
                    }
                    compList.append( item );
                }
                else qDebug() << " ERROR Creating Component: "<< type << id;
            }
        }
        //else
        //    qDebug() << QString( "Circuit::loadCircuit: Unrecognised element tag name: %1")
        //               .arg( tagName ) ;

        node = node.nextSibling();
    }
    if( m_pasting )
    {
        foreach( Component *item, compList )
        {
            item->move( m_deltaMove );
            item->setSelected( true );
        }
        foreach( Component* item, conList )
        {
            Connector* con = static_cast<Connector*>( item );
            con->setSelected( true );
            con->move( m_deltaMove );
        }
    }
    else 
        m_graphicView->centerOn( QPointF( 1200+itemsBoundingRect().center().x(), 950+itemsBoundingRect().center().y() ) );
    
    // Take care about unconnected Joints
    foreach( Node* joint, jointList ) joint->remove(); // Only removed if some missing connector
}

bool Circuit::saveCircuit( QString &fileName )
{
    if( !fileName.endsWith(".simu") ) fileName.append(".simu");

    QFile file( fileName );

    if( !file.open(QFile::WriteOnly | QFile::Text) )
    {
          QMessageBox::warning(0l, tr("Application"),
          tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
          return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    circuitToDom();

    out << m_domDoc.toString();
    file.close();
    QApplication::restoreOverrideCursor();

    //m_changed = false;
    return true;
}

void Circuit::listToDom( QDomDocument* doc, QList<Component*>* complist )
{
    QDomElement root = doc->firstChild().toElement();

    int count = complist->count();
    for (int i=0; i<count; i++)
    {
        Component* item = complist->at(i);
        // Don't save internal items
        bool isNumber = false;
        item->objectName().split("-").last().toInt( &isNumber );

        if ( item && isNumber)
        {
            QDomElement pin = m_domDoc.createElement("item");
            const QMetaObject* metaobject = item->metaObject();

            int count = metaobject->propertyCount();
            for (int i=0; i<count; i++)
            {
                QMetaProperty metaproperty = metaobject->property(i);
                const char* name = metaproperty.name();

                QVariant value = item->property( name );
                if ( metaproperty.type() == QVariant::StringList )
                {
                    QStringList list= value.toStringList();
                    pin.setAttribute( name, list.join(",") );
                }
                else  pin.setAttribute( name, value.toString() );

                //qDebug() << "type" << value.type()<< "typename" << value.typeName()<< "name " << name
                //         << "   value " << value << "saved" << value.toString();

            }
            QDomText blank = m_domDoc.createTextNode("\n \n");
            QDomText objNme = m_domDoc.createTextNode(item->objectName());
            root.appendChild(blank);
            root.appendChild(objNme);
            blank = m_domDoc.createTextNode(": \n");
            root.appendChild(blank);
            root.appendChild(pin);
        }
    }
}

void Circuit::circuitToDom()
{
    m_domDoc.clear();
    QDomElement root = m_domDoc.createElement("circuit");
    root.setAttribute( "type", "simulide_0.1" );
    m_domDoc.appendChild(root);

    listToDom( &m_domDoc, &m_compList );

    foreach( Component* comp, m_conList )
    {
        Connector* con = static_cast<Connector*>( comp );
        con->remNullLines();
    }
    listToDom( &m_domDoc, &m_conList );

    QDomText blank = m_domDoc.createTextNode("\n \n");
    root.appendChild(blank);
}

void Circuit::undo()
{
    if( m_undoStack.isEmpty() ) return;

    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->stopSim();

    circuitToDom();
    m_redoStack.prepend( new QDomDocument() );
    m_redoStack.first()->setContent( m_domDoc.toString() );

    remove();
    QDomDocument* doc = m_undoStack.takeLast();
    m_domDoc.setContent( doc->toString());

    m_seqNumber = 0;
    loadDomDoc( &m_domDoc );

    if( pauseSim ) Simulator::self()->runContinuous();
}

void Circuit::redo()
{
    if( m_redoStack.isEmpty() ) return;

    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->stopSim();

    circuitToDom();
    m_undoStack.append( new QDomDocument() );
    m_undoStack.last()->setContent( m_domDoc.toString() );

    remove();
    QDomDocument* doc = m_redoStack.takeFirst();
    m_domDoc.setContent( doc->toString());

    m_seqNumber = 0;
    loadDomDoc( &m_domDoc );

    if( pauseSim ) Simulator::self()->runContinuous();
}

Component* Circuit::createItem( QString type, QString id )
{
    //qDebug() << type << id;
    foreach( LibraryItem* libItem, ItemLibrary::self()->items() )
        if( libItem->type()==type )
            return libItem->createItemFnPtr()( this, type, id );

    return 0l;
}

void Circuit::loadProperties( QDomElement element, Component* Item )
{
    const QMetaObject* metaobject = Item->metaObject();
    int count = metaobject->propertyCount();
    for (int i=0; i<count; ++i)
    {
        QMetaProperty metaproperty = metaobject->property(i);
        const char* name = metaproperty.name();
        QVariant value( element.attribute( name ) );

        if ( metaproperty.type() == QVariant::String )  Item->setProperty( name, value );

        else if ( metaproperty.type() == QVariant::StringList )
        {
            QStringList list= value.toString().split(",");
            Item->setProperty( name, list );
        }
        else if ( metaproperty.type() == QVariant::Int    )
                  Item->setProperty( name, value.toInt() );

        //else if ( metaproperty.type() == QMetaType::Float ) Item->setProperty( name, value.toFloat() );

        else if ( metaproperty.type() == QVariant::Double )
                  Item->setProperty( name, value.toDouble() );

        else if ( metaproperty.type() == QVariant::PointF )
                  Item->setProperty( name, value.toPointF() );

        else if ( metaproperty.type() == QVariant::Bool   )
                  Item->setProperty( name, value.toBool() );

        else qDebug() << "    ERROR!!! Circuit::LoadProperties\n  unknown type:  "<<"name "<<name<<"   value "<<value ;
    }
    Item->setLabelPos();
    Item->setValLabelPos();

    addItem(Item);

    int number = Item->objectName().split("-").last().toInt();

    if ( number > m_seqNumber ) m_seqNumber = number;               // Adjust item counter: m_seqNumber
}

void Circuit::copy( QPointF eventpoint )
{
    m_eventpoint = togrid(eventpoint);

    QList<Component*> complist;

    QList<QGraphicsItem*> itemlist = selectedItems();

    QGraphicsItem* item;
    foreach( item , itemlist )
    {
        Component* comp =  qgraphicsitem_cast<Component*>( item );
        if( comp )
        {
            if( comp->itemType() == "Connector" )
            {
                Connector* con = static_cast<Connector*>( comp );
                con->remNullLines();

                complist.append( con );
            }
            else
            {
                complist.prepend( comp );
            }
        }
    }

    m_copyDoc.clear();
    QDomElement root = m_copyDoc.createElement("circuit");
    root.setAttribute( "type", "simulide_0.1" );
    m_copyDoc.appendChild(root);

    listToDom( &m_copyDoc, &complist );
}

void Circuit::paste( QPointF eventpoint )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->stopSim();

    saveState();

    m_pasting = true;

    m_deltaMove = togrid(eventpoint) - m_eventpoint;

    loadDomDoc( &m_copyDoc );

    m_pasting = false;

    if( pauseSim ) Simulator::self()->runContinuous();
}

bool  Circuit::pasting() { return m_pasting; }
QPointF Circuit::deltaMove(){ return m_deltaMove; }

void Circuit::createSubcircuit()
{
    QHash<QString, QString> compList;        // Get Components properties

    //qDebug() << compIdTip<<"--------------------------";
    foreach( Component* component, m_compList)
    {
        QString compId     = "";
        QString propString = "";

        const QMetaObject* metaObject = component->metaObject();

        int count = metaObject->propertyCount();
        for (int i=0; i<count; ++i)
        {
            QMetaProperty property = metaObject->property(i);
            if( property.isUser() )
            {
                QString name = property.name();
                if( !name.contains( "Show" ) )
                {
                    const char* charname = property.name();

                    QVariant value = component->property(charname);
                    QString valString = value.toString();

                    if( name == "id") compId = valString;
                    else
                    {
                        name[0] = name[0].toLower();
                        name = name.replace( "_", "" );
                        propString += "        "+name+" = \""+valString+"\"\n";
                    }
                }
            }
        }
        compList[compId] = propString;
    }

    QList<eNode*> eNodeList = simulator.geteNodes();
    QList<QStringList> connectionList;

    int nodes = 0;
    foreach( eNode* node,  eNodeList  ) // Get all the connections in each eNode
    {
        QStringList pinConList;
        QList<ePin*> pinList =  node->getEpins();

        foreach( ePin* epin,  pinList  )
        {
            Pin* pin = (static_cast<Pin*>(epin));
            Component* component = pin->component();
            QString    compId    = component->itemID();

            QString test = compId.toLower();
            if( test.contains("packagepin") )
            {
                // Take care about "packagepin" bad spelling
                compId = compId.replace( test.indexOf("packagepin"), 10, "packagePin");
                pinConList.prepend( compId );
            }
            else if( compId.contains( "Node") ) ;
            else
            {
                pinConList.append( compId );
                pinConList.append( component->itemType() );
                pinConList.append( QString::fromStdString( epin->getId() ).split( "-" ).last() );
            }
        }
        QString conType = "Node";
        if( pinConList.length() == 4 ) conType = "Connection";

        if( conType == "Connection" )               // PackagePin to pin
        {
            QString pin1   = pinConList.takeLast();     // Component pin
            QString pin2   = pinConList.takeFirst();      // Package Pin
            QString compty = pinConList.takeFirst();   // Component type
            QString compId = pinConList.takeFirst();     // Component Id

            pinConList << compty << compId << pin1+"-"+pin2;
            connectionList.append( pinConList );
        }
        else                                      // Multiple connection
        {
            QString pin2   = "eNode"+QString::number(nodes);
            bool isNode = true;

            foreach (QString entry, pinConList)
            {
                if( entry.contains("packagePin") ) // No Node, connection to packagePin
                {
                    pin2 = entry;
                    pinConList.removeOne( entry );
                    isNode = false;
                    break;
                }
            }
            while( !pinConList.isEmpty() ) // Create connection entries
            {
                QStringList pinConList2;
                QString compId = pinConList.takeFirst();
                QString compty = pinConList.takeFirst();
                QString pin1   = pinConList.takeFirst();

                pinConList2 << compId << compty << pin1+"-"+pin2;
                connectionList.append( pinConList2 );
            }
            if( isNode ) nodes++;
        }
    }
    QString subcircuit = "<!DOCTYPE SimulIDE>\n\n";
    subcircuit += "<subcircuit enodes=\""+QString::number(nodes)+"\">\n\n";

    while( !connectionList.isEmpty() )
    {
        QStringList list = connectionList.takeFirst();
        if( list.isEmpty() ) continue;

        QString compId = list.takeFirst();
        QString compty = "e"+list.takeFirst().replace( " ", "");
        QString conect = list.takeFirst();

        subcircuit += "    <item itemtype=\""+compty+"\"\n";
        subcircuit += compList[compId];                        // Properties
        subcircuit += "        connections=\"";
        subcircuit += "\n        "+conect;

        int counter = -1;
        foreach( QStringList list2, connectionList )
        {
            counter++;
            if( list2.isEmpty() ) continue;
            QString compId2 = list2.at(0);

            if( compId == compId2)
            {
                list2.removeFirst();
                list2.removeFirst();
                QString conect2 = list2.takeFirst();
                subcircuit += "\n        "+conect2;
                connectionList.replace( counter, list2 );
            }
        }
        subcircuit +="\" >\n    </item>\n\n";
    }
    subcircuit +="</subcircuit>";
    QString fileName = "subcirc.xml";
    QFile file( fileName );

    if( !file.open(QFile::WriteOnly | QFile::Text) )
    {
          QMessageBox::warning(0l, tr("Application"),
          tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
    }
    QTextStream out(&file);
    out << subcircuit;
    file.close();
    //qDebug() << subcircuit;
}

QString Circuit::newSceneId()
{
    return QString("%1").arg(++m_seqNumber) ;
}

void Circuit::newconnector( Pin*  startpin )
{
    saveState();

    //if ( m_subcirmode ) return;
    m_con_started = true;

    QString type = QString("Connector");
    QString id = type;
    id.append( "-" );
    id.append( newSceneId() );

    new_connector = new Connector( this, type, id, startpin );

    QPoint p1 = startpin->scenePos().toPoint();
    QPoint p2 = startpin->scenePos().toPoint();

    new_connector->addConLine( p1.x(), p1.y(), p2.x(), p2.y(), 0 );

    addItem(new_connector);
}

void Circuit::closeconnector( Pin* endpin )
{
    m_con_started = false;
    new_connector->closeCon( endpin );
}

void Circuit::constarted( bool started) { m_con_started = started; }
bool Circuit::is_constarted() { return m_con_started ; }

void Circuit::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        QPropertyEditorWidget::self()->setObject( this );

        if( m_con_started )  event->accept();//new_connector->incActLine() ;
        QGraphicsScene::mousePressEvent( event );
    }
    else if ( event->button() == Qt::RightButton )
    {
        if( m_con_started )
        {
            event->accept();
        }
        else QGraphicsScene::mousePressEvent( event );
    }
}

void Circuit::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        //QPropertyEditorWidget::self()->setObject( this );

        if( m_con_started )  new_connector->incActLine() ;
        QGraphicsScene::mouseReleaseEvent( event );
    }
    else if ( event->button() == Qt::RightButton )
    {
        if( m_con_started )
        {
            event->accept();
            new_connector->remove();
            m_con_started = false;
        }
        else QGraphicsScene::mouseReleaseEvent( event );
    }
}

void Circuit::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
    if( m_con_started )
    {
        event->accept();
        new_connector->updateConRoute( 0l, event->scenePos() );
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Circuit::keyPressEvent( QKeyEvent* event )
{
    if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
    {
        QPoint p = CircuitWidget::self()->mapFromGlobal(QCursor::pos());

        copy( m_graphicView->mapToScene( p ) );
        clearSelection();
    }
    else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier))
    {
        QPoint p = CircuitWidget::self()->mapFromGlobal(QCursor::pos());

        paste( m_graphicView->mapToScene( p ) );
    }
    else if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier))
    {
        undo();
    }
    else if (event->key() == Qt::Key_Y && (event->modifiers() & Qt::ControlModifier))
    {
        redo();
    }
    else if (event->key() == Qt::Key_Delete)
    {
        removeItems();
    }
    if (event->key() == Qt::Key_N && (event->modifiers() & Qt::ControlModifier))
    {
        CircuitWidget::self()->newCircuit();
    }
    else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier))
    {
        if (event->modifiers() & Qt::ShiftModifier)
            CircuitWidget::self()->saveCircAs();
        else
            CircuitWidget::self()->saveCirc();
    }
    else if (event->key() == Qt::Key_O && (event->modifiers() & Qt::ControlModifier))
    {
        CircuitWidget::self()->openCirc();
    }
    else QGraphicsScene::keyPressEvent(event);
}

void Circuit::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QGraphicsScene::contextMenuEvent( event );

    /*if( !event->isAccepted() )
    {
        QMenu menu;

        QAction* openCircAct = menu.addAction(QIcon(":/opencirc.png"), tr("Open Circuit") );
        connect(openCircAct, SIGNAL(triggered()), MainWindow::self(), SLOT(openCirc()));

        QAction* newCircAct = menu.addAction( QIcon(":/newcirc.png"), tr("New Circuit") );
        connect( newCircAct, SIGNAL(triggered()), MainWindow::self(), SLOT(newCircuit()));

        QAction* saveCircAct = menu.addAction(QIcon(":/savecirc.png"), tr("Save Circuit") );
        connect(saveCircAct, SIGNAL(triggered()), MainWindow::self(), SLOT(saveCirc()));

        QAction* saveCircAsAct = menu.addAction(QIcon(":/savecircas.png"),tr("Save Circuit As...") );
        connect(saveCircAsAct, SIGNAL(triggered()), MainWindow::self(), SLOT(saveCircAs()));

        menu.exec( event->screenPos() );
    }*/
}

#include "moc_circuit.cpp"

