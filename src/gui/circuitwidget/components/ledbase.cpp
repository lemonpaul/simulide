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

#include "ledbase.h"
#include "connector.h"
#include "simulator.h"
#include "pin.h"

LedBase::LedBase( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eLed( id.toStdString() )
{
    m_grounded = false;
    m_ground   = 0l;
    m_scrEnode = 0l;
    
    m_color = QColor( Qt::black );
    setColor( yellow );
    
    m_valLabel->setEnabled( false );
    m_valLabel->setVisible( false );

    Simulator::self()->addToUpdateList( this );
}
LedBase::~LedBase()
{ 
}

void LedBase::updateStep()
{
    update();
}

bool LedBase::grounded()
{
    return m_grounded;
}

void LedBase::setGrounded( bool grounded )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();

    if( grounded )
    {
        if( m_grounded ) return;
        
        Pin* pin1 = (static_cast<Pin*>(m_ePin[1]));
        if( m_ePin[1]->isConnected() ) pin1->connector()->remove();
        pin1->setEnabled( false );
        pin1->setVisible( false );
        
        QString nodid = m_id;
        nodid.append(QString("Gnod-eSource"));
        
        m_ground = new eSource( nodid.toStdString(), m_ePin[1] );
        
        m_scrEnode = new eNode( nodid+"scr" );
        m_scrEnode->setNodeNumber(0);
        Simulator::self()->remFromEnodeList( m_scrEnode, /*delete=*/ false );
        
        m_ePin[1]->setEnode( m_scrEnode );
    }
    else
    {
        if( !m_grounded ) return;
        
        Pin* pin1 = (static_cast<Pin*>(m_ePin[1]));
        //if( m_ePin[1]->isConnected() ) pin1->connector()->remove();
        pin1->setEnabled( true );
        pin1->setVisible( true );
        
        delete m_ground;
        delete m_scrEnode;

        m_ground = 0l;
        m_scrEnode = 0l;
        
        m_ePin[1]->setEnode( 0l );
    }
    m_grounded = grounded;

    if( pauseSim ) Simulator::self()->runContinuous();
}

void LedBase::remove()
{
    if( m_ePin[0] && m_ePin[0]->isConnected() )
        (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if(( m_ePin[1] && m_ePin[1]->isConnected() )&( !m_grounded ))
        (static_cast<Pin*>(m_ePin[1]))->connector()->remove();

    if( m_ground )   delete m_ground;
    
    Simulator::self()->remFromUpdateList( this ); 
    
    Component::remove();
}

void LedBase::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    eLed::updateBright();

    QPen pen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    if( m_bright > 255+75 )                               // Max Current
    {
        m_bright = 0;
        p->setBrush( Qt::white );
        pen.setColor( QColor( Qt::white ));
    }

    int overBight = 100;
    if( m_bright > 40 )
    {
        m_bright += 10;                          // Set a Minimun Bright
        if( m_bright > 255 ) 
        {
            overBight += m_bright-255;
            m_bright = 255;
        }
    }

    p->setPen(pen);

    drawBackground( p );
    
    QColor color = QColor( m_bright, m_bright, overBight ); // Default = yellow
    
    if     ( m_ledColor == red )    color = QColor( m_bright,  m_bright/3, overBight );
    else if( m_ledColor == green )  color = QColor( overBight, m_bright,   m_bright*2/3 );
    else if( m_ledColor == blue )   color = QColor( overBight, m_bright/2, m_bright );
    else if( m_ledColor == orange ) color = QColor( m_bright,  m_bright*2/3, overBight );
    else if( m_ledColor == purple ) color = QColor( m_bright,  overBight,  m_bright*2/3 );
    

    pen.setColor( color );
    pen.setWidth(2.5);
    p->setPen(pen);
    p->setBrush( color );

    drawForeground( p );
}

#include "moc_ledbase.cpp"
