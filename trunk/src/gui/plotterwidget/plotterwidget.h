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

#ifndef PLOTTERWIDGET_H
#define PLOTTERWIDGET_H

#include <QtWidgets>

#include "renderarea.h"

class PlotterWidget : public QWidget
{
    Q_OBJECT

    public:
        PlotterWidget( QWidget *parent );
        ~PlotterWidget();

 static PlotterWidget* self() { return m_pSelf; }

        int  addChannel();
        void remChannel( int channel );

        QColor getColor( int channel );

        void step();
        void setData( int channel, int data );
        void setTicksPs( int tps );
        //void setButtonText( QString text );
        void setPlotterTick( int tickUs );

    private:
 static PlotterWidget* m_pSelf;

        void setupWidget();

        QHBoxLayout* m_horizontalLayout;
        QVBoxLayout* m_verticalLayout;
        QLineEdit*   m_chanLabel[4];

        //RenderArea  m_value;
        RenderArea*  m_rArea;

        QColor m_color[4];

        bool m_channel[4];
        int  m_data[4];
        int  m_numchan;
        int  m_counter;
        int  m_ticksPs;
};

#endif

