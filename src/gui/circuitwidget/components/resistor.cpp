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

#include "resistor.h"
#include "connector.h"
#include "itemlibrary.h"


Component* Resistor::construct( QObject* parent, QString type, QString id )
{ return new Resistor( parent, type, id ); }

LibraryItem* Resistor::libraryItem()
{
    return new LibraryItem(
            tr( "Resistor" ),
            tr( "Passive" ),
            "resistor.png",
            "Resistor",
            Resistor::construct);
}

Resistor::Resistor( QObject* parent, QString type, QString id )
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

    const QFont sansFont("Helvetica [Cronyx]", 4);
    m_labelcurr = Circuit::self()->addSimpleText( id.toLatin1().data(), sansFont );
    m_labelcurr->setParentItem( this );
    m_labelcurr->setPos(-9.2, -4.0 );
    //m_labelcurr->rotate( 180-dir );
    m_labelcurr->setText( QString("%1").arg(/*resistance->*/res()) );
}
Resistor::~Resistor(){}


void Resistor::setRes( double resist )
{
    eResistor::setRes( resist );
    m_labelcurr->setText( QString("%1").arg(/*resistance->*/res()) );
}

void Resistor::remove()
{
    if( m_ePin[0]->isConnected() ) (static_cast<Pin*>(m_ePin[0]))->connector()->remove();
    if( m_ePin[1]->isConnected() ) (static_cast<Pin*>(m_ePin[1]))->connector()->remove();
    Component::remove();
}

void Resistor::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    /*if ( Simulator::self()->isAnimated() )
    {
        if ( rightpin->isConnected() and leftpin->isConnected() )
        {
            if ( m_current > 0 )
                p->setBrush( Qt::darkGreen );
            else
                p->setBrush( Qt::green );
        }
    }
    else*/
        //p->setBrush( Qt::white );

    p->drawRect( -10.5, -4, 21, 8 );
}

#include "moc_resistor.cpp"
