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

#include "pin.h"
#include "connector.h"


Pin::Pin( int angle, const QPoint &pos, QString id, int index, Component* parent )
        : QObject(), QGraphicsItem( parent ), ePin( id.toStdString(), index )
{
    m_component = parent;
    my_connector = 0l;
    //m_length = 8;
    m_angle  = angle;
    m_id   = id;

    m_area = QRect(-4, -4, 12, 8);

    setObjectName( id );
    setConnector( 0l );

    setPos( pos );

    //int xlabelpos = pos.x()+10;

    //if( angle == 0 ) xlabelpos = pos.x()-10;

    rotate( 180-angle );
    const QFont sansFont( "Helvetica [Cronyx]", 6 );
    m_label = Circuit::self()->addSimpleText( id.toLatin1().data(), sansFont );
    m_label->setParentItem( parent /*this*/ );

    m_label->setText("");//( QString("%1 v").arg(m_volt) );
    m_label->setBrush( QColor( 250, 250, 200 ) );

    setLength(8);

    setCursor(Qt::CrossCursor);
    this->setFlag( QGraphicsItem::ItemStacksBehindParent, true );
    this->setFlag( QGraphicsItem::ItemIsSelectable, true );

    connect( parent, SIGNAL( moved() ), this, SLOT( isMoved() ) );
}
Pin::~Pin(){}

void Pin::reset()
{
    //qDebug() << "Pin::reset "<< m_id << m_numConections;
    setConnector( 0l );
    ePin::reset();
    //qDebug() << "ePin::reset new:" << m_numConections;
    //m_msg = msg_dis;
    m_component->inStateChanged( m_index ); // Only used by node??
}

double Pin::getVolt()
{
    return ePin::getVolt();
}

//void Pin::setChanged( bool changed ) { m_changed = changed; }

void  Pin::setConnector( Connector* connector )
{
    my_connector = connector;
    
    if( my_connector ) 
    {
        //ePin::setEnode( connector->enode() );
        setCursor( Qt::ArrowCursor );
    }
    else               
    {
        //ePin::setEnode( 0l );
        setCursor( Qt::CrossCursor );
    }
}

/*void Pin::setConnected( bool connected )
{
    ePin::m_connected = connected;
    if( my_connector ) setCursor( Qt::ArrowCursor );
    else             { setCursor( Qt::CrossCursor );
                       my_connector = 0l; }
}*/

Connector* Pin::connector() { return my_connector; }

void Pin::isMoved()
{
    if( my_connector ) my_connector->updateConRoute( this, scenePos() );
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if( event->button() == Qt::LeftButton )
    {
        if( my_connector==0l )
        {
            event->accept();
            if( Circuit::self()->is_constarted() ) Circuit::self()->closeconnector( this );
            else                                   Circuit::self()->newconnector( this );
        }
        else event->ignore();
    }
}

void Pin::setLabelText( QString label )
{
    m_label->setText( label );
    setLabelPos();
}
void Pin::setLabelPos()
{
    QFontMetrics fm( m_label->font() );

    int xlabelpos = pos().x()+m_length+2;

    if( m_angle == 0 ) xlabelpos = pos().x() - fm.width(m_label->text())-m_length-2;//5*m_label->text().length();

    //m_label->setX( xlabelpos );

    m_label->setPos(xlabelpos, pos().y()-6 );
}

void Pin::moveBy( int dx, int dy )
{
    m_label->moveBy( dx, dy );
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

//bool Pin::changed(){ return m_changed; }

void Pin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    //painter->setBrush( Qt::red );
    //painter->drawRect( boundingRect() );
    
    if( isSelected() ) pen.setColor( Qt::darkGray);

    painter->setPen(pen);

    if( m_length < 1 ) m_length = 1;
    painter->drawLine( 0, 0, m_length-1, 0);
}
#include "moc_pin.cpp"
