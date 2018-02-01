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

#include "meter.h"
#include "simulator.h"
#include "connector.h"
#include "utils.h"
#include "pin.h"

#include <math.h>   // fabs(x,y)

Meter::Meter( QObject* parent, QString type, QString id )
    : Component( parent, type, id ),
      eResistor( id.toStdString() )
{
    m_area = QRectF( -24, -24, 48, 32 );
    QString pinId = m_id;
    pinId.append(QString("-lPin"));
    QPoint pinPos = QPoint(-8, 16);
    m_ePin[0] = new Pin( 270, pinPos, pinId, 0, this);

    pinId = m_id;
    pinId.append(QString("-rPin"));
    pinPos = QPoint(8, 16);
    m_ePin[1] = new Pin( 270, pinPos, pinId, 1, this);

    m_idLabel->setPos(-12,-24);
    setLabelPos(-12,-24, 0);

    const QFont f( "Helvetica [Cronyx]", 10, QFont::Bold );
    m_valLabel->setFont(f);
    m_valLabel->setAcceptedMouseButtons(0);
    m_valLabel->setEnabled( false );
    m_valLabel->setDefaultTextColor( Qt::yellow );
    //m_valLabel->document()->setDefaultTextOption( QTextOption(Qt::AlignHCenter) );

    setValLabelX( -20 );
    setValLabelY( -22 );
    setValLabRot( 0 );
    setValLabelPos();
    setShowVal( true );

    Simulator::self()->addToUpdateList( this );
}
Meter::~Meter(){}

void Meter::updateStep()
{
    int dispVal = 0;
    
    QString sign = "";
    
    double dispValue = fabs(m_dispValue);
    
    if( dispValue > 1e-12 )
    {
        if( m_dispValue < 0 ) sign = "-";
        
        setValue( dispValue );
        dispVal = int( m_value*10+0.5 );
        
        if( dispVal > 999 ) 
        {
            setValue( dispVal/10 );
            dispVal = int( m_value*10 );
        }
        //qDebug() <<"Meter::updateStep"<<m_dispValue<< m_value<<dispVal<<m_unitMult;
    }
    m_valLabel->setHtml( "<div align='center'><pre>"+sign+decToBase( dispVal/10, 10, 3 )
                        +"."+decToBase( dispVal%10, 10, 1 )
                        +"<br/>"+m_mult+m_unit+"</pre></div>" );
}

void Meter::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();

    Simulator::self()->remFromUpdateList( this );

    Component::remove();
}

void Meter::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if( event->button() == Qt::RightButton ) 
    {
        event->accept();
        QMenu* menu = new QMenu();
        m_eventpoint = mapToScene( togrid(event->pos()) );

        runContextMenu( event->screenPos(), menu );
        menu->deleteLater();
    }
    else Component::mousePressEvent( event );
}

void Meter::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    p->setBrush( Qt::black);

    p->drawRect( m_area );
}

#include "moc_meter.cpp"
