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

#include "voltsource.h"
#include "connector.h"
#include "itemlibrary.h"


Component* VoltSource::construct( QObject* parent, QString type, QString id )
{
    return new VoltSource( parent, type, id );
}

LibraryItem* VoltSource::libraryItem()
{
    return new LibraryItem(
        tr( "Volt. Source" ),
        tr( "Sources" ),
        "voltsource.png",
        "Voltage Source",
		VoltSource::construct );
}

VoltSource::VoltSource( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
{
    m_voltOut   = 0.0;
    //m_voltLow   = 0.0;
    m_voltHight = 0.0;//5.0;

    //VoltWidget voltw = new VoltWidget();
    m_voltw.setFixedSize( 48,72 );
    m_proxy = Circuit::self()->addWidget( &m_voltw );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint(-40, -64) );
    m_proxy->setFlag(QGraphicsItem::ItemStacksBehindParent, true );

    m_button = m_voltw.pushButton;
    m_dial   = m_voltw.dial;

    m_button->setText( QString("%1").arg(float(int(m_voltHight*100))/100));

    QString nodid = id;
    nodid.append(QString("-outnod"));
    QPoint nodpos = QPoint(16,0);
    outpin = new Pin( 0, nodpos, nodid, 0, this );

    nodid.append("-eSource");
    m_out = new eSource( nodid.toStdString(), outpin );
    //setVolt(5.0);

    connect( m_button, SIGNAL(clicked()),
             this,     SLOT  (onbuttonclicked()) );

    connect( m_dial,   SIGNAL(valueChanged(int)),
             this,     SLOT  (voltChanged(int)) );
}

VoltSource::~VoltSource() { delete m_out; }

void VoltSource::onbuttonclicked()
{
    m_out->setOut( m_button->isChecked() );

    QString msg = QString("%1 V").arg(float(int(m_voltOut*100))/100);
    m_button->setText( msg );
}

void VoltSource::voltChanged( int volt )
{
    m_voltOut = double( volt )/100;
    m_out->setVoltHigh( double( volt )/100 );
    onbuttonclicked();
}

void VoltSource::setVolt( double v )
{
    m_voltHight = v;
    m_dial->setMaximum( int(v*100) );
    //update();
}

/*void VoltSource::setChanged( bool changed )
{
    m_changed = changed;

    if( !changed ) outpin->setChanged( false );
}*/

void VoltSource::remove()
{
    if ( outpin->isConnected() ) outpin->connector()->remove();
    Component::remove();
}

void VoltSource::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    p->drawRoundedRect( QRect( -40, -64, 48, 72 ), 1, 1 );

    p->setBrush(Qt::darkGray);
    p->drawRoundedRect( QRect( 8, -56, 8, 40 ), 1, 1 );

    Component::paint( p, option, widget );
}

#include "moc_voltsource.cpp"
