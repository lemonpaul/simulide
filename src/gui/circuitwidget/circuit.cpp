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

#include "circuit.h"
#include "itemlibrary.h"
#include "mainwindow.h"
#include "node.h"
#include "utils.h"

Circuit*  Circuit::m_pSelf = 0l;

Circuit::Circuit( qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent)
    : QGraphicsScene(x, y, width, height, parent)
{
    setParent( parent );
    m_widget = parent;
    m_scenerect.setRect( x, y, width, height );
    setSceneRect( QRectF(x, y, width, height) );

    m_pSelf = this;

    m_pasting = false;
    m_con_started = false;
    new_connector = 0l;
    m_seqNumber   = 1;
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

void Circuit::remove()
{
    //qDebug() << m_compList.size();
    foreach( Component* comp, m_compList )
    {
        //qDebug() << "Circuit::remove" << comp->itemID();
        
        // Don't remove internal items
        bool isNumber = false;
        comp->objectName().split("-").last().toInt( &isNumber ); // TODO: Find a proper way !!!!!!!!!!!
        // Don't remove Graphical Nodes
        bool isNode = comp->objectName ().contains( "Node" );
        
        if( isNumber && !isNode )  comp->remove();
    }
        
}
/*void Circuit::setChanged()
{
    m_changed = true;
}*/

void Circuit::drawBackground ( QPainter*  painter, const QRectF & rect )
{
    Q_UNUSED( rect );
    /*painter->setBrush(QColor( 255, 255, 255 ) );
    painter->drawRect( m_scenerect );*/
    
    painter->setBrush(QColor( 240, 240, 210 ) );
    painter->drawRect( m_scenerect );
    painter->setPen( QColor( 210, 210, 210 ) );

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

    if (!m_domDoc.setContent(&file))
    {
        QMessageBox::warning(0l, tr("MainWindow::loadCircuit"),
        tr("Cannot set file %1\nto DomDocument").arg(fileName));
        file.close();
        return;
    }
    file.close();

    //m_seqNumber = 1;
    loadDomDoc();
    m_domDoc.clear();
}

QString Circuit::getCompId( QString name )
{
    QStringList nameSplit = name.split("-");
    QString compId  = nameSplit.takeFirst();
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

void Circuit::loadDomDoc()
{
    QDomElement root = m_domDoc.documentElement();
    QDomNode    node = root.firstChild();
    QList<Component*> compList;   // Component list
    QList<Component*> conList;    // Connector list
    QHash<QString, QString> idMap;
    QHash<QString, eNode*> nodMap;

    while( !node.isNull() )
    {
        QDomElement   element = node.toElement();
        const QString tagName = element.tagName();

        if( tagName == "item" )
        {
            QString objNam = element.attribute( "objectName"  );
            QString type   = element.attribute( "itemtype"  );
            QString itemId = element.attribute( "id"  );

            QString id = objNam.split("-").first()+"-"+newSceneId();
            idMap[objNam] = id;

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
//qDebug() << thispinid << endpinid;
                        if     ( thispinid == startpinid ) { startpin = thispin; }
                        else if( thispinid == endpinid )   { endpin   = thispin; }
                    }
                }

                if( !startpin ) // Pin not found by name... find it by pos
                {
                    QStringList pointList   = element.attribute( "pointList" ).split(",");
                    int itemX = pointList.first().toInt();
                    int itemY = pointList.at(1).toInt();

                    startpin = findPin( itemX, itemY, startpinid );

                    if( !startpin ) qDebug() << "\n   ERROR!!    Circuit::loadCircuit:  null startpin in " << id << startpinid;
                }
                if( !endpin ) // Pin not found by name... find it by pos
                {
                    QStringList pointList   = element.attribute( "pointList" ).split(",");
                    int itemX = pointList.at(pointList.size()-2).toInt();
                    int itemY = pointList.last().toInt();

                    endpin = findPin( itemX, itemY, endpinid );

                    if( !endpin )qDebug() << "\n   ERROR!!    Circuit::loadCircuit:  null endpin in " << id << endpinid;
                }
                if( startpin && endpin )    // Create Connector
                {

                    Connector* con  = new Connector( this, type, id, startpin, endpin );

                    element.setAttribute( "startpinid", startpinid  );
                    element.setAttribute(   "endpinid", endpinid );

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
            }
            else if( type == "Node")
            {
                Node* fork = new Node( this, type, id );
                loadProperties( element, fork );
                compList.append( fork );
            }
            else if( type == "LEDSMD" ); // TODO: this type shouldnt be saved to circuit
                                         // bcos is created inside another component, for example boards
            else
            {
                //qDebug() << type << id;
                Component* item = createItem( type, id );
                if( item )
                {
                    loadProperties( element, item );
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
        /*foreach( Component* item, compList ) // For connectors well-positionig
        {
            item->move( QPointF( 0, 8 ) );
            item->move( QPointF( 8, 0 ) );
        }*/
    }
    m_widget->ensureVisible( itemsBoundingRect() );
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

    m_domDoc.clear();
    QDomElement root = m_domDoc.createElement("circuit");
    root.setAttribute( "type", "simu" );
    m_domDoc.appendChild(root);

    listToDom( &m_compList );

    foreach( Component* comp, m_conList )
    {
        Connector* con = static_cast<Connector*>( comp );
        con->remNullLines();
    }
    listToDom( &m_conList );

    QDomText blank = m_domDoc.createTextNode("\n \n");
    root.appendChild(blank);

    out << m_domDoc.toString();
    file.close();
    QApplication::restoreOverrideCursor();

    //m_changed = false;
    return true;
}

void Circuit::listToDom( QList<Component*>* complist )
{
    QDomElement root = m_domDoc.firstChild().toElement();

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

Component* Circuit::createItem( QString type, QString id )
{
    //qDebug() << type << id;
    foreach( LibraryItem* libItem, itemLibrary()->items() )
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

    m_domDoc.clear();
    QDomElement root = m_domDoc.createElement("circuit");
    root.setAttribute( "type", "simu" );
    m_domDoc.appendChild(root);

    listToDom( &complist );
}

void Circuit::paste( QPointF eventpoint )
{
    m_pasting = true;

    m_deltaMove = togrid(eventpoint) - m_eventpoint;

    loadDomDoc();

    m_pasting = false;
}

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
    new_connector->setPos( startpin->scenePos() );
    addItem(new_connector);
}

void Circuit::closeconnector( Pin* endpin )
{
    m_con_started = false;
    new_connector->closeCon( endpin );
}

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

void Circuit::removeItems()
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) MainWindow::self()->powerCircOff();

    foreach( Component* comp, m_compList )
        if( comp->isSelected() ) comp->remove();

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

