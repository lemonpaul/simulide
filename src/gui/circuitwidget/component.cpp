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
    ,multUnits( "TGMk munp" )
{
    setObjectName( id );
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    
    m_unitMult = 1;
    m_mult     = " ";
    m_unit     = " ";
    m_type     = type;
    m_color    = QColor( Qt::white );
    m_showId   = false;
    m_id       = id;
    
    m_idLabel = new Label( this );
    m_idLabel->setDefaultTextColor( Qt::darkBlue );
    m_idLabel->m_labelx   = -16;
    m_idLabel->m_labely   = -24;
    m_idLabel->m_labelrot = 0;
    m_idLabel->setLabelPos();
    setShowId( false );
    
    m_valLabel = new Label( this );
    m_valLabel->setDefaultTextColor( Qt::black );
    m_valLabel->m_labelx   = 0;
    m_valLabel->m_labely   = 0;
    m_valLabel->m_labelrot = 0;
    m_valLabel->setLabelPos();
    QFont f;
    f.setPointSize(9);
    m_valLabel->setFont(f);
    setShowVal( false );

    setToolTip( QString("Left-Click and Move this Component \nRight-Click for Context Menu \nSelect and change properties in Right Panel") );
    setCursor(Qt::OpenHandCursor);
    this->setFlag( QGraphicsItem::ItemIsSelectable, true );

    if( type == "Connector" ) Circuit::self()->conList()->append( this );
    else                      Circuit::self()->compList()->prepend( this );
}
Component::~Component(){}

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

void Component::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton )
    {
        QPropertyEditorWidget::self()->setObject( this );
        QPropertyEditorWidget::self()->setVisible( true );
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
    connect( removeAction, SIGNAL( triggered()), this, SLOT(slotRemove()) );

    QAction* rotateCWAction = menu->addAction( QIcon( ":/rotateCW.png"),"Rotate CW" );
    connect( rotateCWAction, SIGNAL( triggered()), this, SLOT(rotateCW()));

    QAction* rotateCCWAction = menu->addAction(QIcon( ":/rotateCCW.png"),"Rotate CCW" );
    connect( rotateCCWAction, SIGNAL( triggered()), this, SLOT(rotateCCW()) );

    QAction* rotateHalfAction = menu->addAction(QIcon(":/rotateCCW.png"),"Rotate 180");
    connect( rotateHalfAction, SIGNAL( triggered()), this, SLOT(rotateHalf()) );

    menu->exec(event->screenPos());
}

void Component::slotRemove()
{
    setSelected( true );
    Circuit::self()->removeItems();
}

void Component::remove()
{
    QPropertyEditorWidget::self()->removeObject( this );
    Circuit::self()->compList()->removeOne( this );
    Circuit::self()->removeItem( this );
    //this->deleteLater();
}

void Component::rotateCW()
{
    setRotation( rotation() + 90);
    m_idLabel->rotateCCW();
    emit moved();
}

void Component::rotateCCW()
{
    setRotation( rotation() - 90);
    m_idLabel->rotateCW();
    emit moved();
}

void Component::rotateHalf()
{
    setRotation( rotation() - 180);
    m_idLabel->rotate180();
    emit moved();
}

void Component::updateLabel( Label* label, QString txt )
{
    if     ( label == m_idLabel ) m_id = txt;
    else if( label == m_valLabel )
    {
        QString value = "";
        int x;
        for( x=0; x<txt.length(); ++x ) 
        {
            QChar atx = txt.at(x);
            if( atx.isDigit() ) value.append( atx );
            else break;
        }
        QString unit = txt.mid( x, txt.length() );
        
        setUnit( unit );
        setValue( value.toDouble() );
        
    }
}

void Component::setLabelPos( int x, int y, int rot )
{
    m_idLabel->m_labelx = x;
    m_idLabel->m_labely = y;
    m_idLabel->m_labelrot = rot;
    m_idLabel->setLabelPos();
}

void Component::setLabelPos()
{
    m_idLabel->setLabelPos();
}

void Component::setValLabelPos()
{
    m_valLabel->setLabelPos();
}

void Component::setValue( double val) 
{ 
    if( val == 0 ) val = 1e-12;
    val = val*m_unitMult;
    
    int index = 4;   // We are in bare units "TGMK munp"
    m_unitMult = 1;
    while( val >= 1000 )
    {
        index--;
        m_unitMult = m_unitMult*1000;
        val = val/1000;
    }
    while( val < 1 )
    {
        index++;
        m_unitMult = m_unitMult/1000;
        val = val*1000;
    }
    m_mult = multUnits.at( index );
    if( m_mult != " " ) m_mult.prepend( " " );
    m_value = val;
    m_valLabel->setPlainText( QString::number(m_value)+m_mult+m_unit );
}

QString Component::unit()                { return m_mult+m_unit; }
void Component::setUnit( QString un ) 
{ 
    QString mul = " ";
    un.replace( " ", "" );
    if( un.size() > 0 ) 
    {
        mul = un.at(0);
        
        double unitMult = 1e12;        // We start in Tera units "TGMk munp"
        
        for( int x=0; x<9; x++ )
        {
            if( mul == multUnits.at(x) ) 
            {
                m_unitMult = unitMult;
                m_mult     = mul;
                if( m_mult != " " ) m_mult.prepend( " " );
                m_valLabel->setPlainText( QString::number(m_value)+m_mult+m_unit );
                return;
            }
            unitMult = unitMult/1000;
        }
    }
    m_unitMult = 1;
    m_mult     = " ";
    m_valLabel->setPlainText( QString::number(m_value)+m_mult+m_unit );
}

/*void Component::setIdLabel()
{
    m_idLabel->setPlainText( m_id );
    
    if( m_showId ) m_idLabel->setVisible( true );
    else           m_idLabel->setVisible( false );
}*/

bool Component::showId()               { return m_showId; }
void Component::setShowId( bool show ) 
{ 
    m_idLabel->setVisible( show );
    m_showId = show;
}

bool Component::showVal()               { return m_showVal;}
void Component::setShowVal( bool show ) 
{ 
    m_valLabel->setVisible( show );
    m_showVal = show; 
}

QString Component::itemID()         { return  m_id; }
void Component::setId( QString id ) {  m_id = id; m_idLabel->setPlainText( m_id ); }

int Component::labelx()            { return m_idLabel->m_labelx; }
void Component::setLabelX( int x ) { m_idLabel->m_labelx = x; }

int Component::labely()            { return m_idLabel->m_labely; }
void Component::setLabelY( int y ) { m_idLabel->m_labely = y; }

int Component::labelRot()              { return m_idLabel->m_labelrot; }
void Component::setLabelRot( int rot ) { m_idLabel->m_labelrot = rot; }

int Component::valLabelx()            { return m_valLabel->m_labelx; }
void Component::setValLabelX( int x ) { m_valLabel->m_labelx = x; }

int Component::valLabely()            { return m_valLabel->m_labely; }
void Component::setValLabelY( int y ) { m_valLabel->m_labely = y; }

int Component::valLabRot()              { return m_valLabel->m_labelrot; }
void Component::setValLabRot( int rot ) { m_valLabel->m_labelrot = rot; }

QString Component::itemType()  { return m_type; }
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
    : QGraphicsTextItem( parent )
{
    m_parentComp = parent;
    m_labelrot = 0;
    setCursor( Qt::OpenHandCursor );
    
    connect(document(), SIGNAL(contentsChange(int, int, int)),
            this,       SLOT(updateGeometry(int, int, int)));
}
Label::~Label() { }

void Label::updateGeometry(int, int, int)
{
    setTextWidth(-1);
    setTextWidth( boundingRect().width() );
}

void Label::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    m_parentComp->updateLabel( this, document()->toPlainText() );
    
    QGraphicsTextItem::focusOutEvent(event);
}

void Label::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if( !isEnabled() ) return;
    setTextInteractionFlags(Qt::TextEditorInteraction);

    QGraphicsTextItem::mouseDoubleClickEvent(event);

}

void Label::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if ( event->button() == Qt::LeftButton )
    {
        event->accept();
        setCursor( Qt::ClosedHandCursor );
        grabMouse();
    }
}

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    event->accept();
    setPos(  pos() + mapToItem( m_parentComp, event->pos() ) - mapToItem( m_parentComp, event->lastPos() ) );
    m_labelx = int(pos().x());
    m_labely = int(pos().y());
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

void Label::setLabelPos()
{
    setX( m_labelx );
    setY( m_labely );
    setRotation( m_labelrot );
    adjustSize();
}

void Label::rotateCW()
{
    setRotation( rotation() + 90 );
    m_labelrot = int(rotation()) ;
}

void Label::rotateCCW()
{
    setRotation( rotation() - 90 );
    m_labelrot = int(rotation()) ;
}

void Label::rotate180()
{
    setRotation( rotation() - 180 );
    m_labelrot = int(rotation()) ;
}

/*void Label::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setBrush( Qt::blue );
    painter->drawRect( boundingRect() );
    QGraphicsTextItem::paint( painter, option, widget );
}*/

#include "moc_component.cpp"
