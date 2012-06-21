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

#include "connector.h"
#include "itemlibrary.h"
#include "logicinput.h"


Component* LogicInput::construct( QObject* parent, QString type, QString id )
{ return new LogicInput( parent, type, id ); }

LibraryItem* LogicInput::libraryItem()
{
    return new LibraryItem(
        tr( "Fixed Volt." ),
        tr( "Sources" ),
        "voltage.png",
        "Fixed Voltage",
		LogicInput::construct );
}

LogicInput::LogicInput( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
{
    m_labelx = -64;
    m_labely = -24;
    setLabelPos();

    QString nodid = id;
    nodid.append(QString("-outnod"));
    QPoint nodpos = QPoint(16,0);
    outpin = new Pin( 0, nodpos, nodid, 0, this);

    nodid.append(QString("-eSource"));
    m_out = new eSource( nodid.toStdString(), outpin );
    setVolt(5.0);
    //m_power = false;
    //m_voltLow   = cero_doub;

    m_button = new QPushButton( );
    m_button->setMaximumSize( 16,16 );
    m_button->setGeometry(-20,-16,16,16);
    m_button->setCheckable( true );

    m_proxy = Circuit::self()->addWidget( m_button );
    m_proxy->setParentItem( this );
    m_proxy->setPos( QPoint(-32, -8) );

    connect( m_button, SIGNAL( clicked() ),
             this,     SLOT  ( onbuttonclicked() ));
}

LogicInput::~LogicInput() {  delete m_button; delete m_out;}

void LogicInput::onbuttonclicked()
{
    //if( m_out->power() )
    {
        m_out->setOut( !m_out->out() );
        update();
    }
}

void LogicInput::setVolt( double v )
{
    m_voltHight = v;
    m_out->setVoltHigh(v);
    update();
}

/*void LogicInput::setChanged( bool changed )
{
    m_changed = changed;

    //if( !changed ) outpin->setChanged( false );
}*/

void LogicInput::remove()
{
    if( outpin->isConnected() ) (static_cast<Pin*>(outpin))->connector()->remove();
    Component::remove();
}


void LogicInput::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    if ( /* m_out->power() &&*/ m_out->out() )
        p->setBrush( QColor( 255, 166, 0 ) );
    else
        p->setBrush( QColor( 230, 230, 255 ) );

    //p->drawEllipse( -8, -8, 16, 16 );
    p->drawRoundedRect( QRectF( -8, -8, 16, 16 ), 2, 2);
}

#include "moc_logicinput.cpp"

