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

#include "mcucomponentpin.h"
#include "mcucomponent.h"


McuComponentPin::McuComponentPin( McuComponent *mcuComponent, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
    : QObject(mcuComponent), eSource(0l)// , m_id( id )
{
    //m_gOutHigh = 0.0;
    //m_gOutLow = 0.0;
    m_id = id;
    //m_inputImp = high_imp;
    //m_outImp   = 10; //cero_doub;
    //m_imp      = high_imp;
    m_Rth = high_imp;
    m_Vth = cero_doub;
    m_type = type;
    m_mcuComponent = mcuComponent;
    
    m_attached = false;

    QString nodid = m_mcuComponent->objectName();
    nodid.append("-");
    nodid.append( id );
    QPoint nodpos = QPoint ( xpos, ypos );
    Pin *pin = new Pin( angle, nodpos, nodid, pos, m_mcuComponent );
    pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false );
    m_ePin[0] = pin;

    eSource::setImp(high_imp);
    eSource::setVoltHigh(5);
    eSource::setOut(false);

    int xlabelpos = xpos+10;

    if( angle == 0 ) xlabelpos = xpos - 5*label.length()-10;

    const QFont sansFont("Helvetica [Cronyx]", 6);
    m_pinLabel = Circuit::self()->addSimpleText( label.toLatin1().data(), sansFont);
    m_pinLabel->setParentItem( m_mcuComponent );
    m_pinLabel->setPos( xlabelpos, ypos-6 );
    m_pinLabel->setBrush( QColor( 250, 250, 200 ) );
}
McuComponentPin::~McuComponentPin()
{
    /*if( m_ePin[0]->isConnected() )
        m_ePin[0]->getEnode()->remFromChangedList(this);*/
}

void McuComponentPin::terminate()
{
    if( m_ePin[0]->isConnected() )
        m_ePin[0]->getEnode()->remFromChangedList(this);
    
    m_attached = false;
}

void McuComponentPin::initialize()
{
    if( m_ePin[0]->isConnected() && m_attached /*&& m_imp==high_imp*/)
        m_ePin[0]->getEnode()->addToChangedList(this);

    eSource::initialize();
}

void McuComponentPin::resetOutput()
{
    setOut(false);
}

void McuComponentPin::move( int dx, int dy )
{
    pin()->moveBy( dx, dy );
}

void McuComponentPin::moveLabel( int dx, int dy )
{
    m_pinLabel->moveBy( dx, dy );
}

#include "moc_mcucomponentpin.cpp"
