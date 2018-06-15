/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "switch_base.h"
#include "circuit.h"

SwitchBase::SwitchBase( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
    , eElement( id.toStdString() )
{
    m_area =  QRectF( 0,0,0,0 );

    m_ePin.resize(2);

    m_changed = true;
    m_closed = false;

    m_idLabel->setPos(-12,-24);

    m_button = new QPushButton( );
    m_button->setMaximumSize( 16,16 );
    m_button->setGeometry(-20,-16,16,16);
    m_button->setCheckable( true );

    m_proxy = Circuit::self()->addWidget( m_button );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint(-8, 4) );

    Simulator::self()->addToUpdateList( this );
}
SwitchBase::~SwitchBase()
{
}

void SwitchBase::initialize()
{
    m_ePin[0]->setEnodeComp( m_ePin[1]->getEnode() );
    m_ePin[1]->setEnodeComp( m_ePin[0]->getEnode() );
    m_ePin[0]->stampAdmitance( 1 ); // Restart circuit afther switch closed issue
    m_ePin[1]->stampAdmitance( 1 );
    m_changed = true;
    updateStep();
}

void SwitchBase::updateStep()
{
    if( m_changed )
    {
        double admit = 1e-6;

        if( m_closed ) admit = 1e3;

        m_ePin[0]->stampAdmitance( admit );
        m_ePin[1]->stampAdmitance( admit );

        m_changed = false;
    }
}

void SwitchBase::setButtonText( QString text )
{
    m_button->setText( text );
}

void SwitchBase::remove()
{
    Simulator::self()->remFromUpdateList( this );

    Component::remove();
}

#include "moc_switch_base.cpp"
