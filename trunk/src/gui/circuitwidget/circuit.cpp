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

#include "mainwindow.h"
#include "circuit.h"
#include "connector.h"
#include "component.h"
#include "itemlibrary.h"
#include "node.h"
#include "pin.h"
#include "utils.h"

#include "avrprocessor.h"

Circuit*  Circuit::m_pSelf = 0l;

Circuit::Circuit( qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent)
    : QGraphicsScene(x, y, width, height, parent)
{
    setParent( parent );
    m_widget = parent;
    m_scenerect.setRect( x, y, width, height );
    setSceneRect( QRectF(x, y, width, height) );

    m_pSelf = this;

    m_con_started = false;
    m_changed = false;
    new_connector = 0l;
    m_seqNumber   = 1;
}

Circuit::~Circuit()
{
    // Avoid PropertyEditor problem: comps no unregistered
    foreach( Component* comp, m_compList )
        QPropertyEditorWidget::self()->removeObject( comp );
}
/*void Circuit::setChanged()
{
    m_changed = true;
}*/

void Circuit::drawBackground ( QPainter*  painter, const QRectF & rect )
{
    Q_UNUSED( rect );
    painter->setBrush(QColor( 250, 250, 230 ) );
    painter->drawRect( m_scenerect );
    painter->setPen( QColor( 230, 230, 230 ) );

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

void Circuit::loadCircuit( QString &fileName )
{
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

    QDomElement root = m_domDoc.documentElement();
    QDomNode    node = root.firstChild();
    QList<eNode*> enodList;

    while( !node.isNull() )
    {
        QDomElement   element = node.toElement();
        const QString tagName = element.tagName();

        if( tagName == "item" )
        {
            QString type = element.attribute( "itemtype"  );
            QString id   = element.attribute( "objectName"  );

            if( type == "Connector" )
            {
                Connector* con = 0l;
                eNode* enode  = 0l;
                Pin* thispin  = 0l;
                Pin* startpin = 0l;
                Pin* endpin   = 0l;

                QString startpinid = element.attribute( "startpinid" );
                QString endpinid   = element.attribute( "endpinid" );
                QString enodeId    = element.attribute( "enodeid" );

                QList<QGraphicsItem*> itemlist = items();
                QGraphicsItem* item;

                foreach( item, itemlist )  // Look for start and end pins of this connector
                {
                    thispin =  qgraphicsitem_cast<Pin* >( item );

                    if( thispin )
                    {
                        QString thispinid = thispin->objectName();

                        if     ( thispinid == startpinid ) { startpin = thispin; }
                        else if( thispinid == endpinid )   { endpin   = thispin; }
                    }
                }
                foreach( eNode* en, enodList )  // Find if eNode was already created
                {
                    if( en->itemId() == enodeId ) { enode = en; break;  }
                }
                if( !enode )                    // Create eNode and add to enodList
                {
                    enode = new eNode( enodeId );
                    enodList.append( enode );
                }

                if( startpin == 0l || endpin == 0l) qDebug() << "\n   ERROR!!    Circuit::loadCircuit:    null Pin in " << id;
                else    // Create Connector
                {
                    con = new Connector( this, type, id, startpin, endpin );

                    //con->closeCon( endpin );
                    con->setEnode( enode );

                    LoadProperties( element, con );

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
                }
            }
            else if( type == "Node")
            {
                Node* fork = new Node( this, type, id );
                LoadProperties( element, fork );
            }
            else if( type == "LEDSMD" ); // TODO: this type shouldnt be saved to circuit
                                         // bcos is created inside another component, for example boards
            else
            {
                //qDebug() << type << id;
                Component* Item = createItem( type, id );
                LoadProperties( element, Item );
            }
        }
        //else
        //    qDebug() << QString( "Circuit::loadCircuit: Unrecognised element tag name: %1")
        //               .arg( tagName ) ;

        node = node.nextSibling();
    }
    m_widget->ensureVisible( itemsBoundingRect() );
    m_changed = false;
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

    listToDom( compList() );
    listToDom( conList());

    QDomText blank = m_domDoc.createTextNode("\n \n");
    root.appendChild(blank);

    out << m_domDoc.toString();
    QApplication::restoreOverrideCursor();

    m_changed = false;
    return true;
}

void Circuit::listToDom( QList<Component*>* complist )
{
    QDomElement root = m_domDoc.firstChild().toElement();

    int count = complist->count();
    for (int i=0; i<count; i++)
    {
        Component* item = complist->at(i);
        if ( item )
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

void Circuit::LoadProperties( QDomElement element, Component* Item )
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
            //qDebug() << "pointlist: " << list;
        }
        else if ( metaproperty.type() == QVariant::Int    ) Item->setProperty( name, value.toInt() );
        //else if ( metaproperty.type() == QMetaType::Float ) Item->setProperty( name, value.toFloat() );
        else if ( metaproperty.type() == QVariant::Double ) Item->setProperty( name, value.toDouble() );
        else if ( metaproperty.type() == QVariant::PointF ) Item->setProperty( name, value.toPointF() );
        else if ( metaproperty.type() == QVariant::Bool   ) Item->setProperty( name, value.toBool() );
        else qDebug() << "    ERROR!!! Circuit::LoadProperties\n  unknown type:  "<<"name "<<name<<"   value "<<value ;
    }
    Item->setLabelPos();
    addItem(Item);

    int number = Item->objectName().split("-").last().toInt();

    if ( number > m_seqNumber ) m_seqNumber = number;               // Adjust item counter: m_seqNumber
}

QString Circuit::newSceneId()
{
    return QString("%1").arg(m_seqNumber++) ;
}

void Circuit::newconnector( Pin*  startpin )
{
    //if ( m_subcirmode ) return;
    m_con_started = true;

    QString type = QString("Connector");
    QString id = type;
    id.append( "-" );
    id.append( newSceneId() );

    new_connector = new Connector( 0, type, id, startpin );

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
        if( m_con_started )  new_connector->incActLine() ;
    }
    if ( event->button() == Qt::RightButton )
    {
        if( m_con_started )
        {
            event->accept();
            new_connector->remove();
            m_con_started = false;
        }
    }
    QGraphicsScene::mousePressEvent( event );
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
    QList<QGraphicsItem*> itemlist = selectedItems();
    if( !itemlist.isEmpty() )
    {
        QGraphicsItem* item;
        foreach( item , itemlist )
        {
            Component* comp =  qgraphicsitem_cast<Component*>( item );
            if ( comp ) comp->remove();
        }
    }
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

