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

#ifndef SWITCH_BASE_H
#define SWITCH_BASE_H

#include "e-resistor.h"
#include "pin.h"

class SwitchBase : public Component, public eResistor
{
    Q_OBJECT

    public:

        SwitchBase( QObject* parent, QString type, QString id );
        ~SwitchBase();

        void updateStep();

        //void setRes( double resist );

    public slots:
        void remove();

    protected:
        bool m_changed;
        
        QPushButton          *m_button;
        QGraphicsProxyWidget *m_proxy;
};

#endif
