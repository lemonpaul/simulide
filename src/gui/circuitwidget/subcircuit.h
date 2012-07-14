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

#ifndef SUBCIRCUIT_H
#define SUBCIRCUIT_H

#include <QtGui>

#include "component.h"
#include "package.h"
#include "e-source.h"

class LibraryItem;

class SubCircuit : public Package
{
	Q_OBJECT
    //Q_PROPERTY( int      Freq     READ freq    WRITE setFreq    DESIGNABLE true USER true )

    public:

        SubCircuit( QObject* parent, QString type, QString id );
        ~SubCircuit();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem * libraryItem();

        virtual void initialize();

	
    public slots:
        virtual void remove();

	protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

        virtual void initPackage();
        virtual void initSubcircuit();

        void connectEpin( ePin *epin, QString connetTo );

        int m_numItems;

        QList<eNode*> m_internal_eNode;
        QList<eElement*> m_elementList;
        QVector<QList<ePin*> > m_pinConections;
};
#endif

