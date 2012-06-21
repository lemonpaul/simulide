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

#ifndef PROBE_H
#define PROBE_H

#include "component.h"
#include "e-element.h"
#include "pin.h"

class LibraryItem;

class Probe : public Component, public eElement
{
    Q_OBJECT
    Q_PROPERTY( bool Show_volt  READ Show_volt  WRITE setShow_volt DESIGNABLE true USER true )

    public:
        QRectF boundingRect() const { return QRect( -10, -10, 20, 20 ); }

        Probe( QObject* parent, QString type, QString id );
        ~Probe();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();

        bool Show_volt();
        void setShow_volt( bool show );

        void setVolt( double volt );

        //virtual void setChanged( bool changed );

        void updateStep();

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        virtual void remove();

        void slotOscopAdd();
        void slotOscopRem();

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    private:
        double m_voltIn;
        double m_voltTrig;

        int    m_oscopLine;
        QColor m_oscopColor;

        bool  m_showVolt;

        Pin  *inputpin;
        Label* m_dispvolt;
};


#endif
