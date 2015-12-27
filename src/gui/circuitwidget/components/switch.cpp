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

#include "switch.h"
#include "connector.h"
#include "itemlibrary.h"


Component* Switch::construct( QObject* parent, QString type, QString id )
{ return new Switch( parent, type, id ); }

LibraryItem* Switch::libraryItem()
{
    return new LibraryItem(
            tr( "Switch" ),
            tr( "Passive" ),
            "switch.png",
            "Switch",
            Switch::construct);
}

Switch::Switch( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eResistor( id.toStdString() )
{
    QString nodid = m_id;
    nodid.append(QString("lnod"));
    QPoint nodpos = QPoint(-8-8,0);
    m_ePin[0] = new Pin( 180, nodpos, nodid, 0, this);

    nodid = m_id;
    nodid.append(QString("rnod"));
    nodpos = QPoint(8+8,0);
    m_ePin[1] = new Pin( 0, nodpos, nodid, 1, this);

    label->setText( QString("") );
    label->setPos(-12,-24);
    
    m_resist = high_imp;                              // Open Switch
    stampAdmit( 0 );
    
    m_button = new QPushButton( );
    m_button->setMaximumSize( 16,16 );
    m_button->setGeometry(-20,-16,16,16);
    m_button->setCheckable( true );

    m_proxy = Circuit::self()->addWidget( m_button );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint(-8, 4) );

    connect( m_button, SIGNAL( clicked() ),
             this,     SLOT  ( onbuttonclicked() ));
}
Switch::~Switch(){}

void Switch::onbuttonclicked()
{
    if( m_resist == cero_doub )                      // switch is Closed
    {
        m_resist = high_imp;                              // Open Switch
        stampAdmit( 0 );
    }
    else                                            // Switch is Oppened
    {
        m_resist = cero_doub;                            // Close Switch
        stampAdmit( high_imp );
    }

    update();
}

void Switch::stampAdmit( double admit )
{
    m_ePin[0]->stampAdmitance( admit );
    m_ePin[1]->stampAdmitance( admit );
}

void Switch::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    Component::remove();
}

void Switch::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    if( m_resist == cero_doub )             // switch is closed
        p->drawLine(-10.5, 0, 10.5, 0 );
    else                                    // Switch is oppened
        p->drawLine(-10.5, 0, 4, -8 );
}

#include "moc_switch.cpp"
