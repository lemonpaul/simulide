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

#ifndef LOGICINPUT_H
#define LOGICINPUT_H

#include "component.h"
#include "e-source.h"
#include "pin.h"

class LibraryItem;

class LogicInput : public Component
{
    Q_OBJECT
    Q_PROPERTY( double Voltage  READ volt   WRITE setVolt   DESIGNABLE true USER true )

    public:
        QRectF boundingRect() const { return QRect( -10, -10, 20, 20 ); }

        LogicInput( QObject* parent, QString type, QString id );
        ~LogicInput();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();

        double volt() const      { return m_voltHight; }
        void setVolt( double v );

        //virtual void setChanged( bool changed );

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        void onbuttonclicked();
        virtual void remove();

    private:
        double m_voltHight;
        //double m_voltLow;

        ePin    *outpin;
        eSource *m_out;

        QPushButton          *m_button;
        QGraphicsProxyWidget *m_proxy;
};


#endif
