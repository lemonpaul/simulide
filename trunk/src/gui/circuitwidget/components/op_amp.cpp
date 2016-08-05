/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2010 by santiago González                               *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "op_amp.h"

#include "itemlibrary.h"
#include "connector.h"
#include "pin.h"


Component* OpAmp::construct( QObject* parent, QString type, QString id )
{
        return new OpAmp( parent, type, id );
}

LibraryItem* OpAmp::libraryItem()
{
    return new LibraryItem(
        tr( "OpAmp" ),
        tr( "Active" ),
        "opamp.png",
        "opAmp",
        OpAmp::construct );
}

OpAmp::OpAmp( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eOpAmp( id.toStdString() )
{
    m_area = QRect( -18, -8*2, 36, 8*2*2 );
    setGain( 1000 );
    m_voltPos = 5;
    m_voltNeg = 0;
    
    QString newId = id;
    //newId.append(QString("-eResistor"));
    //m_inResistor = new eResistor( newId.toStdString() );
    
    newId = id;
    newId.append(QString("inputNinv"));
    Pin* newPin = new Pin( 180, QPoint(-16-8,-8), newId, 0, this );
    newPin->setLabelText( "+" );
    newPin->setLabelColor( QColor( 0, 0, 0 ) );
    m_ePin[0] = newPin;
    //m_inResistor->setEpin( 0, m_ePin[0] );
    //newId.append("-eSource");
    //m_inputNinv = new eSource( newId.toStdString(), m_ePin[0] );
    //m_inputNinv->setImp( high_imp );
    
    newId = id;
    newId.append(QString("inputInv"));
    newPin = new Pin( 180, QPoint(-16-8,8), newId, 1, this );
    newPin->setLabelText( " -" );
    newPin->setLabelColor( QColor( 0, 0, 0 ) );
    m_ePin[1] = newPin;
    //m_inResistor->setEpin( 1, m_ePin[1] );
    //newId.append("-eSource");
    //m_inputInv = new eSource( newId.toStdString(), m_ePin[1] );
    //m_inputInv->setImp( high_imp );
    
    /*newId = id;
    newId.append(QString("powerPos"));
    m_ePin[2] = new Pin( 0, QPoint(0,-16), newId, 0, this );
    newId.append("-eSource");
    m_powerPos = new eSource( newId.toStdString(), m_ePin[2] );
    
    newId = id;
    newId.append(QString("powerNeg"));
    m_ePin[3] = new Pin( 0, QPoint(0,16), newId, 0, this );
    newId.append("-eSource");
    m_powerNeg = new eSource( newId.toStdString(), m_ePin[3] );*/
    
    newId = id;
    newId.append(QString("output"));
    m_ePin[2] = new Pin( 0, QPoint(16+8,0), newId, 2, this );
    newId.append("-eSource");
    m_output = new eSource( newId.toStdString(), m_ePin[2] );
    //m_output->setImp( 40 );
    m_output->setOut( true );
    
    //m_inResistor->setRes( high_imp );
}
OpAmp::~OpAmp()
{
    delete m_output;
    //delete m_inputInv;
    //delete m_inputNinv;
    //delete m_inResistor;
}

void OpAmp::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    if( m_ePin[2]->isConnected() ) (static_cast<Pin*>(m_ePin[2]))->connector()->remove();
    Component::remove();
}

void OpAmp::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    QPen pen = p->pen();
    pen.setWidth(2);
    p->setPen(pen);

    static const QPointF points[4] = {
        QPointF(-16,-16 ),
        QPointF(-16, 16 ),
        QPointF( 16,  1 ),
        QPointF( 16, -1 )            };

    p->drawPolygon(points, 4);
}

#include "moc_op_amp.cpp"
