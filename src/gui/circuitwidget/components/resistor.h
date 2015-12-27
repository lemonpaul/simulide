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

#ifndef RESISTOR_H
#define RESISTOR_H

#include "e-resistor.h"
#include "pin.h"

class LibraryItem;

class Resistor : public Component, public eResistor
{
    Q_OBJECT
    Q_PROPERTY( double Resistance READ res WRITE setRes DESIGNABLE true USER true )

    public:
        QRectF boundingRect() const { return QRectF( -11, -4.5, 22, 9 ); }

        Resistor( QObject* parent, QString type, QString id );
        ~Resistor();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();

        void setRes( double resist );

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        void remove();

    private:
        //void updateVI();
        //eResistor *resistance;

        QGraphicsSimpleTextItem* m_labelcurr;
};

#endif
