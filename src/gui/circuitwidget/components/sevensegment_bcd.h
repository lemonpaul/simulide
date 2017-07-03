/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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

#ifndef SEVENSEGMENTBCD_H
#define SEVENSEGMENTBCD_H

#include "itemlibrary.h"
#include "logiccomponent.h"
#include "e-logic_device.h"


class SevenSegmentBCD : public LogicComponent, public eLogicDevice
{
    Q_OBJECT

    public:
        SevenSegmentBCD( QObject* parent, QString type, QString id );
        ~SevenSegmentBCD();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();

        void initialize();
        void updateStep();
        void remove();
        
        virtual void setVChanged();

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    private:
        int m_origx;
        int m_origy;
        
        std::vector<bool> m_outValue;
};

#endif
