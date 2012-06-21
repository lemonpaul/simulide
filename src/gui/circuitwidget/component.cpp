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

#include "component.h"
#include "connector.h"
#include "utils.h"

Component::Component( QObject* parent , QString type, QString id )
    : QObject(parent), QGraphicsItem()
{
    m_showId = false;
    m_id = id;

    setObjectName( id );
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    m_type     = type;
    m_labelx   = 0;
    m_labely   = -24;
    m_labelrot = 0;
    m_color = QColor( Qt::white );

    label = new Label( this );
    setLabelPos();
    //label->setText( id );
    label->setBrush( Qt::darkBlue );

    setToolTip( QString("Left-Click and move this component") );
    setCursor(Qt::OpenHandCursor);
    this->setFlag( QGraphicsItem::ItemIsSelectable, true );

    if( type == "Connector" ) Circuit::self()->conList()->append( this ); // Connectors at end of list
    else                      Circuit::self()->compList()->prepend( this );
}
Component::~Component(){}

void Component::setLabel()
{
    if( m_showId ) label->setText( m_id );
    else           label->setText( " " );
}

void Component::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if( event->button() == Qt::LeftButton )
    {
        event->accept();

        if( !isSelected() )     // Deselecciona los demas
        {
            QList<QGraphicsItem*> itemlist = Circuit::self()->selectedItems();

            foreach( QGraphicsItem* item, itemlist ) item->setSelected( false );

            setSelected( true );
        }

        QPropertyEditorWidget::self()->setObject( this );
        setCursor( Qt::ClosedHandCursor );
        grabMouse();
    }
}

void Component::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();

    QList<QGraphicsItem*> itemlist = Circuit::self()->selectedItems();
    if ( !itemlist.isEmpty() )
    {
        QPointF delta = togrid(event->scenePos()) - togrid(event->lastScenePos());

        QGraphicsItem* item;
        foreach( item , itemlist )
        {
            ConnectorLine* line =  qgraphicsitem_cast<ConnectorLine* >( item );
            if( line->objectName() == "" )  line->move( delta ); //qDebug () <<line->objectName();

            Component* comp =  qgraphicsitem_cast<Component* >( item );
            if( comp ) comp->move( delta );
        }
    }
}

void Component::move( QPointF delta )
{
    setPos( scenePos() + delta );
    emit moved();
}

void Component::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    setCursor( Qt::OpenHandCursor );
    ungrabMouse();
}

void Component::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    event->accept();
    QMenu* menu = new QMenu();
    contextMenu( event, menu );
    menu->deleteLater();
}

void Component::contextMenu( QGraphicsSceneContextMenuEvent* event, QMenu* menu )
{
    QAction* removeAction = menu->addAction( QIcon( ":/remove.png"),"Remove" );
    connect( removeAction, SIGNAL( triggered()), this, SLOT(slotremove()) );

    QAction* rotateCWAction = menu->addAction( QIcon( ":/rotateCW.png"),"Rotate CW" );
    connect( rotateCWAction, SIGNAL( triggered()), this, SLOT(rotateCW()));

    QAction* rotateCCWAction = menu->addAction(QIcon( ":/rotateCCW.png"),"Rotate CCW" );
    connect( rotateCCWAction, SIGNAL( triggered()), this, SLOT(rotateCCW()) );

    QAction* rotateHalfAction = menu->addAction(QIcon(":/rotateCCW.png"),"Rotate 180");
    connect( rotateHalfAction, SIGNAL( triggered()), this, SLOT(rotateHalf()) );

    menu->exec(event->screenPos());
}

void Component::slotremove()
{
    setSelected( true );
    Circuit::self()->removeItems();
}

void Component::remove()
{
    QPropertyEditorWidget::self()->removeObject( this );
    Circuit::self()->compList()->removeOne( this );
    Circuit::self()->removeItem( this );
    this->deleteLater();
}

void Component::rotateCW()
{
    setRotation( rotation() + 90);
    label->rotateCCW();
    emit moved();
}

void Component::rotateCCW()
{
    setRotation( rotation() - 90);
    label->rotateCW();
    emit moved();
}

void Component::rotateHalf()
{
    setRotation( rotation() - 180);
    label->rotate180();
    emit moved();
}

void Component::setLabelPos()
{
    label->setX( m_labelx );
    label->setY( m_labely );
    label->setRotation( m_labelrot );
}

bool Component::Show_id()               {return m_showId;}
void Component::setShow_id( bool show ) {  m_showId = show; setLabel(); }

QString Component::itemID()         { return  m_id; }
void Component::setId( QString id ) {  m_id = id; setLabel(); }

int Component::labelx()            { return m_labelx; }
void Component::setLabelX( int x ) { m_labelx = x; }

int Component::labely()            { return m_labely; }
void Component::setLabelY( int y ) { m_labely = y; }

int Component::labelrot()              { return m_labelrot; }
void Component::setLabelRot( int rot ) { m_labelrot = rot; }

QString Component::itemtype()  { return m_type; }
QString Component::category()  { return m_category; }
QIcon   Component::icon()      { return m_icon; }

//bool Component::isChanged(){ return m_changed;}

void Component::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QPen pen(Qt::black, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    if ( isSelected() )
    {
        pen.setColor( Qt::darkGray);
        painter->setBrush( Qt::darkGray);
        //label->setBrush( Qt::darkGray );
    }
    else
    {
        painter->setBrush( m_color );
        //label->setBrush( Qt::darkBlue );
    }

    //painter->setBrush( Qt::yellow );
    //painter->drawRect( boundingRect() );

    painter->setPen( pen );
}


// CLASS Label  *****************************************************************************

Label::Label( Component* parent )
    : QObject(parent), QGraphicsSimpleTextItem( parent )
{
    m_parentComp = parent;
    setCursor( Qt::OpenHandCursor );
}
Label::~Label() { }

void Label::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if ( event->button() == Qt::LeftButton )
    {
        //QPropertyEditorWidget::self()->setObject( this );
        event->accept();
        setCursor( Qt::ClosedHandCursor );
        grabMouse();
    }
}

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    setPos(  pos() + mapToItem( m_parentComp, event->pos() ) - mapToItem( m_parentComp, event->lastPos() ) );
    m_parentComp->setLabelX( int(pos().x()) );
    m_parentComp->setLabelY( int(pos().y()) );
}

void Label::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    setCursor( Qt::OpenHandCursor );
    ungrabMouse();
}

void Label::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    event->accept();
    QMenu menu;

    QAction* rotateCWAction = menu.addAction(QIcon(":/rotateCW.png"),"Rotate CW");
    connect(rotateCWAction, SIGNAL(triggered()), this, SLOT(rotateCW()));

    QAction* rotateCCWAction = menu.addAction(QIcon(":/rotateCCW.png"),"Rotate CCW");
    connect(rotateCCWAction, SIGNAL(triggered()), this, SLOT(rotateCCW()));

    QAction* rotate180Action = menu.addAction(QIcon(":/rotate180.png"),"Rotate 180º");
    connect(rotate180Action, SIGNAL(triggered()), this, SLOT(rotate180()));

    /*QAction* selectedAction = */menu.exec(event->screenPos());
}

void Label::rotateCW()
{
    setRotation( rotation() + 90 );
    m_parentComp->setLabelRot( int(rotation()) );
}

void Label::rotateCCW()
{
    setRotation( rotation() - 90 );
    m_parentComp->setLabelRot( int(rotation()) );
}

void Label::rotate180()
{
    setRotation( rotation() - 180 );
    m_parentComp->setLabelRot( int(rotation()) );
}

#include "moc_component.cpp"
