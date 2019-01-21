/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#ifndef RAM8BIT_H
#define RAM8BIT_H

#include "itemlibrary.h"
#include "logiccomponent.h"
#include "e-ram8bit.h"

class MAINMODULE_EXPORT Ram8bit : public LogicComponent, public eRam8bit
{
    Q_OBJECT

    public:
        Ram8bit( QObject* parent, QString type, QString id );
        ~Ram8bit();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();
};

#endif

