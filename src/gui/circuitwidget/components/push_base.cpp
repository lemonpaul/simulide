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

#include "push_base.h"


PushBase::PushBase( QObject* parent, QString type, QString id )
    : SwitchBase( parent, type, id )
{
    connect( m_button, SIGNAL( pressed() ),
                 this, SLOT  ( onbuttonPressed() ));

    connect( m_button, SIGNAL( released() ),
                 this, SLOT  ( onbuttonReleased() ));
}
PushBase::~PushBase()
{
}

void PushBase::onbuttonPressed()
{
    m_closed = true;
    m_changed = true;
    update();
}

void PushBase::onbuttonReleased()
{
    m_closed = false;
    m_button->setChecked(false);
    m_changed = true;
    update();
}


#include "moc_push_base.cpp"
