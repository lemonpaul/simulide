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

#ifndef CLOCK_H
#define CLOCK_H

#include "logicinput.h"


class LibraryItem;

class MAINMODULE_EXPORT Clock : public LogicInput
{
    Q_OBJECT
    Q_PROPERTY( int Freq  READ freq   WRITE setFreq   DESIGNABLE true USER true )

    public:
        QRectF boundingRect() const { return QRect( -14, -8, 22, 16 ); }

        Clock( QObject* parent, QString type, QString id );
        ~Clock();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();

        void updateStep();
        
        int freq() { return m_freq; }
        void setFreq( int freq );

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        virtual void onbuttonclicked();
        virtual void setVChanged();
        virtual void remove();

    private:
        bool m_isRunning;
        bool m_changed;
        
        int m_step;
        int m_stepsPC;
        int m_freq;
};

#endif
