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

#include "switch_base.h"
#include "connector.h"
#include "circuit.h"

SwitchBase::SwitchBase( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eResistor( id.toStdString() )
{
    m_area =  QRectF( -11, -9, 22, 11 );
    
    m_changed = true;
    
    QString pinid = m_id;
    pinid.append(QString("-lnod"));
    QPoint pinpos = QPoint(-8-8,0);
    m_ePin[0] = new Pin( 180, pinpos, pinid, 0, this);

    pinid = m_id;
    pinid.append(QString("-rnod"));
    pinpos = QPoint(8+8,0);
    m_ePin[1] = new Pin( 0, pinpos, pinid, 1, this);

    //m_idLabel->setText( QString("") );
    m_idLabel->setPos(-12,-24);
    
    m_resist = 1e38;
    //stampAdmit( 0 );
    eResistor::stamp();
    
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

void SwitchBase::updateStep()
{
    if( m_changed ) 
    {
        if( m_resist >= high_imp )
        {
            m_ePin[0]->stampAdmitance( 0 );
            m_ePin[1]->stampAdmitance( 0 );
        }
        else  eResistor::stamp();
        m_changed = false;
    }
}

void SwitchBase::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    
    Simulator::self()->remFromUpdateList( this );
    
    Component::remove();
}

