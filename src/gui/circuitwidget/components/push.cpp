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

#include "push.h"
#include "connector.h"
#include "itemlibrary.h"


Component* Push::construct( QObject* parent, QString type, QString id )
{ return new Push( parent, type, id ); }

LibraryItem* Push::libraryItem()
{
    return new LibraryItem(
            tr( "Push" ),
            tr( "Switches" ),
            "push.png",
            "Push",
            Push::construct);
}

Push::Push( QObject* parent, QString type, QString id )
    : SwitchBase( parent, type, id )
{
    connect( m_button, SIGNAL( pressed() ),
             this,     SLOT  ( onbuttonPressed() ));

    connect( m_button, SIGNAL( released() ),
             this,     SLOT  ( onbuttonReleased() ));
}
Push::~Push()
{
}

void Push::onbuttonPressed()
{
    m_resist = cero_doub;                                  // Close Push
    m_changed = true;
    update();
}
void Push::onbuttonReleased()
{
    m_resist = 1e50;                                        // Open Push
    m_button->setChecked(false);
    m_changed = true;
    update();
}

void Push::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    if( m_resist == cero_doub )             // Pushch is closed
        p->drawLine(-9, -2, 9, -2 );
    else                                    // Push is oppened
        p->drawLine(-9, -8, 9, -8 );
}

#include "moc_push.cpp"
