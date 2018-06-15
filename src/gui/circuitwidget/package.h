/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#ifndef PACKAGE_H
#define PACKAGE_H

#include "component.h"
#include "e-element.h"


class MAINMODULE_EXPORT Package : public Component, public eElement
{
    Q_OBJECT

    public:
        Package( QObject* parent, QString type, QString id );
        ~Package();

        virtual void initEpins(){;}

        virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );

    public slots:
        virtual void remove();

    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

        virtual void addPin( QString id, QString type, QString label,
                             int pos, int xpos, int ypos, int angle );

        virtual void initPackage();

        int   m_numpins;

        QString m_dataFile;     // file containig package defs
};
#endif

