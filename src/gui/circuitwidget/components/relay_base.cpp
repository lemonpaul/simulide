/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "relay_base.h"
#include "connector.h"
#include "simulator.h"
#include "pin.h"

RelayBase::RelayBase( QObject* parent, QString type, QString id )
         : Component( parent, type, id )
         , eInductor( id.toStdString() )
{
    // This is a "real" coil, so is an Inductor in series with a Resistor: -Ind-Nod-Res-
    // We need to create the resistor, internal eNode and do the connections.

    m_ePin.resize(4);
    m_pin.resize(2);

    QString nodid = m_id;            // External Left pin to inductor
    nodid.append(QString("-lPin"));
    QPoint nodpos = QPoint(-16-8,0);
    m_pin[0] = new Pin( 180, nodpos, nodid, 0, this);
    m_pin[0]->setLength(4.5);
    m_pin[0]->setPos(-16, 0 );
    m_ePin[0] = m_pin[0];

    nodid = m_id;                    // External Right pin to resistor
    nodid.append(QString("-rPin"));
    nodpos = QPoint(16+8,0);
    m_pin[1] = new Pin( 0, nodpos, nodid, 1, this);
    m_pin[1]->setLength(4.5);
    m_pin[1]->setPos( 16, 0 );
    m_ePin[3] = m_pin[1];

    QString reid = m_id;
    reid.append(QString("-resistor"));
    m_resistor = new eResistor( reid.toStdString() );

    nodid = reid;                  // Internal Left pin to inductor
    nodid.append(QString("-lIntPin"));
    m_ePin[1] = new ePin( reid.toStdString(), 1 );

    nodid = reid;                 // Internal right pin to resistor
    nodid.append(QString("-rIntPin"));
    m_ePin[2] = new ePin( reid.toStdString(), 2 );

    m_resistor->setEpin( 0, m_ePin[2] );
    m_resistor->setEpin( 1, m_ePin[3] );

    m_resistor->setRes( 50 );
    m_trigCurrent = 0.075;

    m_internalEnode = new eNode( m_id+"-internaleNode" );
    m_ePin[1]->setEnode( m_internalEnode );
    m_ePin[2]->setEnode( m_internalEnode );

    setInd( 0.1 );  // 100 mH
    //m_unit = "H";
    //setInduc( m_ind );
    setValLabelPos(-16, 6, 0);
    setShowVal( true );

    setLabelPos(-16,-24, 0);

}
RelayBase::~RelayBase()
{
}

void RelayBase::setVChanged()
{
    eInductor::setVChanged();

    if( fabs(m_curSource) > m_trigCurrent ) setSwitch( true );
    else                                    setSwitch( false );
}

void RelayBase::remove()
{
    Simulator::self()->remFromEnodeList( m_internalEnode, true );

    delete m_resistor;

    Component::remove();
}

void RelayBase::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Component::paint( p, option, widget );

    QPen pen = p->pen();
    pen.setWidth(2.8);
    p->setPen(pen);

    QRectF rectangle(-12,-4.5, 10, 10 );
    int startAngle = -45 * 16;
    int spanAngle = 220 * 16;
    p->drawArc(rectangle, startAngle, spanAngle);

    QRectF rectangle2(-5,-4.5, 10, 10 );
    startAngle = 225 * 16;
    spanAngle = -270 * 16;
    p->drawArc(rectangle2, startAngle, spanAngle);

    QRectF rectangle3(2,-4.5, 10, 10 );
    startAngle = 225 * 16;
    spanAngle = -220 * 16;
    p->drawArc(rectangle3, startAngle, spanAngle);
}
