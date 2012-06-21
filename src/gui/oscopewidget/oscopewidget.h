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

#ifndef OSCOPEWIDGET_H
#define OSCOPEWIDGET_H

#include <QtGui>

#include "renderarea.h"

class OscopeWidget : public QWidget
{
    Q_OBJECT

    public:
        OscopeWidget( QWidget *parent );
        ~OscopeWidget();

 static OscopeWidget* self() { return m_pSelf; }

        int  addChannel();
        void remChannel( int channel );

        QColor getColor( int channel );

        void step();
        void setData( int channel, int data );
        void setTicksPs( int tps );
        //void setButtonText( QString text );

    private:
 static OscopeWidget* m_pSelf;

        void setupWidget();

        QHBoxLayout* m_horizontalLayout;
        QVBoxLayout* m_verticalLayout;
        QLineEdit*   m_chanLabel[4];

        //RenderArea  m_value;
        RenderArea*  m_oscope;

        QColor m_color[4];

        bool m_channel[4];
        int  m_data[4];
        int  m_numchan;
        int  m_counter;
        int  m_ticksPs;
};

#endif

