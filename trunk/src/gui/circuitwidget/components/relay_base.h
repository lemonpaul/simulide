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

#ifndef RELAY_BASE_H
#define RELAY_BASE_H

#include "e-inductor.h"
#include "component.h"


class MAINMODULE_EXPORT RelayBase : public Component, public eInductor
{
    Q_OBJECT
    Q_PROPERTY( double Rcoil READ rCoil    WRITE setRCoil    DESIGNABLE true USER true )
    Q_PROPERTY( double Itrig READ iTrig    WRITE setITrig    DESIGNABLE true USER true )

    public:

        RelayBase( QObject* parent, QString type, QString id );
        ~RelayBase();

        double rCoil() const { return m_resistor->res(); }
        void setRCoil(double res) { if (res > 0.0f) m_resistor->setResSafe(res); }
        double iTrig() const { return m_trigCurrent; }
        void setITrig(double current) { if (current > 0.0f) m_trigCurrent = current; }

        void setVChanged();
        virtual void initialize();

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        virtual void remove();

    protected:
        virtual void setSwitch( bool on )=0;

        eResistor* m_resistor;

        std::vector<eResistor*> m_switches;
        std::vector<Pin*> m_pin;

        eNode* m_internalEnode;
        double m_trigCurrent;
        bool m_state;
};

#endif
