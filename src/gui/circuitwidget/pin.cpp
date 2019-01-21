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

#include "pin.h"
#include "connector.h"
#include "circuit.h"


Pin::Pin( int angle, const QPoint &pos, QString id, int index, Component* parent )
   : QObject()
   , QGraphicsItem( parent )
   , ePin( id.toStdString(), index )
   , m_label( parent )
{
    m_component  = parent;
    
    m_blocked = false;
    m_isBus   = false;
    m_unused  = false;
    
    my_connector = 0l;
    m_conPin     = 0l;
    m_enode      = 0l;
    m_angle      = angle;
    m_id         = id;

    m_area = QRect(-4, -4, 12, 8);

    setObjectName( id );
    setConnector( 0l );
    setPos( pos );
    setRotation( 180-angle );
    setLength(8);
    setCursor( Qt::CrossCursor );
    setFlag( QGraphicsItem::ItemStacksBehindParent, true );
    setFlag( QGraphicsItem::ItemIsSelectable, true );

    QFont sansFont( "Helvetica [Cronyx]", 5 );
    sansFont.setPixelSize(6);
    m_label.setFont( sansFont );
    m_label.setText("");
    m_label.setBrush( QColor( 250, 250, 200 ) );
    
    Circuit::self()->addPin( this, m_id );

    connect( parent, SIGNAL( moved() ), this, SLOT( isMoved() ) );
}
Pin::~Pin()
{ 
    Circuit::self()->removePin( m_id );
}

void Pin::reset()
{
    //qDebug() << "Pin::reset "<< m_id << m_numConections;
    setConnector( 0l );
    ePin::reset();
    //qDebug() << "ePin::reset new:" << m_numConections;
    m_component->inStateChanged( 1 );          // Used by node to remove
}

void Pin::setUnused( bool unused )
{
    m_unused = unused;
    if( unused ) setCursor( Qt::ArrowCursor );
}

void Pin::findConnectedPins()     // Called by node,  for connected pins
{
    if( m_blocked ) return;
    
    if( m_connected )
        m_enode->addSubEpin( this );// Notify that this pin is connected
        
    if( m_conPin ) 
        m_conPin->findNodePins(); // Call pin at other side of Connector

}

void Pin::findNodePins()     // Called by connector closing or other pin
{    
    m_blocked = true;
    
    if( m_connected )
        m_enode->addSubEpin( this );// Notify that this pin is connected
        
    m_component->inStateChanged( 0 );       // Used by node to find pins
    m_blocked = false;
}

/*double Pin::getVolt()
{
    return ePin::getVolt();
}*/

void  Pin::setConnector( Connector* connector )
{
    my_connector = connector;
    
    if( my_connector ) 
    {
        setCursor( Qt::ArrowCursor );
        if( m_isBus ) 
        {
            my_connector->setIsBus( true );
            //m_component->inStateChanged( 2 );
        }
    }
    else setCursor( Qt::CrossCursor );
}

Connector* Pin::connector() { return my_connector; }

void Pin::isMoved()
{
    if( my_connector ) my_connector->updateConRoute( this, scenePos() );
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if( m_unused ) return;
    
    if( event->button() == Qt::LeftButton )
    {
        if( my_connector==0l )
        {
            if( Circuit::self()->is_constarted() )
            {
                Connector* con = Circuit::self()->getNewConnector();
                if( con->isBus() != m_isBus ) // Avoid connect Bus with no-Bus
                {
                    event->ignore();
                    return;
                }
            }
            event->accept();
            if( Circuit::self()->is_constarted() ) Circuit::self()->closeconnector( this );
            else                                   Circuit::self()->newconnector( this );
        }
        else event->ignore();
    }
}

QString Pin::getLabelText()
{
    return m_label.text();
}

void Pin::setLabelText( QString label )
{
    m_label.setText( label );
    setLabelPos();
}
void Pin::setLabelPos()
{
    QFontMetrics fm( m_label.font() );

    int xlabelpos = pos().x();
    int ylabelpos = pos().y();

    if( m_angle == 0 )   // Right side
    {
        xlabelpos -= fm.width(m_label.text())+m_length+1;
        ylabelpos -= 5;
    }
    if( m_angle == 90 )   // Top
    {
        xlabelpos += 5;
        ylabelpos += m_length+1;
        m_label.setRotation(m_angle);
    }
    if( m_angle == 180 )   // Left
    {
        xlabelpos += m_length+1;
        ylabelpos -= 5;
    }
    if( m_angle == 270 )   //bottom
    {
        m_label.setRotation(m_angle);
        xlabelpos -= 5;
        ylabelpos -= m_length+1;
        
    }
    m_label.setPos(xlabelpos, ylabelpos );
}

void Pin::setLabelColor( QColor color )
{
    m_label.setBrush( color );
}

void Pin::moveBy( int dx, int dy )
{
    m_label.moveBy( dx, dy );
    QGraphicsItem::moveBy( dx, dy );
}

QString Pin::itemID() { return  m_id; }

void Pin::setLength( int length )
{
    m_length = length;
    setLabelPos();
}

void Pin::setConPin( Pin* pin ){ m_conPin = pin; }
Pin* Pin::conPin()             { return m_conPin; }

void Pin::setBoundingRect( QRect area )
{
    m_area = area;
}

void Pin::setIsBus( bool bus )
{
    if( m_isBus == bus ) return;
    if( !bus ) return;
    m_isBus = bus;
    
    if( my_connector ) my_connector->setIsBus( true );
    if( m_conPin ) m_conPin->setIsBus( true );
    
    m_component->inStateChanged( 2 );
}

bool Pin::isBus()
{
    return m_isBus;
}

void Pin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    //painter->setBrush( Qt::red );
    //painter->drawRect( boundingRect() );
    
    if( m_unused )     pen.setColor( QColor( 75, 120, 170 ));
    if( isSelected() ) pen.setColor( Qt::darkGray);

    painter->setPen(pen);

    if( m_length < 1 ) m_length = 1;
    painter->drawLine( 0, 0, m_length-1, 0);
    
    if( m_inverted )
    {
        //Component::paint( p, option, widget );
        painter->setBrush( Qt::white );
        QPen pen = painter->pen();
        pen.setWidth( 2 );
        //if( isSelected() ) pen.setColor( Qt::darkGray);
        painter->setPen(pen);
        QRectF rect( 3,-2.5, 5, 5 ); 
        painter->drawEllipse(rect);
    }
}
#include "moc_pin.cpp"
