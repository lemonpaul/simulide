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

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "component.h"
#include "e-resistor.h"
#include "e-element.h"
#include "dialwidget.h"
#include "pin.h"

class LibraryItem;

class Potentiometer : public Component, public eElement
{
    Q_OBJECT
    Q_PROPERTY( double Resistance  READ res   WRITE setRes   DESIGNABLE true USER true )

    public:

        Potentiometer( QObject* parent, QString type, QString id );
        ~Potentiometer();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem* libraryItem();
        
        void initialize();

        double res() const      { return m_resist; }
        void setRes( double v );//{ m_voltHight = v; outpin->compChanged( msg_vth, m_voltOut ); }

        virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );

    public slots:
        void resChanged( int volt );
        virtual void remove();

    private:
        double m_resist;
        double m_voltOut;
        
        Pin m_pinA;
        Pin m_pinM;
        Pin m_pinB;
        ePin m_ePinA;
        ePin m_ePinB;
        
        eResistor m_resA;
        eResistor m_resB;
        
        DialWidget m_dialW;

        QDial* m_dial;
        QGraphicsProxyWidget* m_proxy;
};

#endif

