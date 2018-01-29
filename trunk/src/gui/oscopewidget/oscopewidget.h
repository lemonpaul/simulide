/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include <QtWidgets>

#include "e-element.h"
#include "renderoscope.h"
#include "probe.h"

class MAINMODULE_EXPORT OscopeWidget : public QWidget, public eElement
{
    Q_OBJECT

    public:
        OscopeWidget( QWidget *parent );
        ~OscopeWidget();

 static OscopeWidget* self() { return m_pSelf; }
        
        void setProbe( Probe* probe );
        void clear();
        void step();
        void setData();
        void setTicksPs( int tps );
        void setOscopeTick( int tickUs );
        virtual void simuClockStep();
        
    public slots:
        void speedChanged( int speed );
        //void ampliChanged( int ampli );
        void centeChanged( int offset );

    private:
 static OscopeWidget* m_pSelf;

        void setupWidget();

        QHBoxLayout* m_horizontalLayout;
        QVBoxLayout* m_verticalLayout;

        RenderOscope*  m_rArea;
        
        bool newReading;
        
        int m_data[140];
        int m_counter;
        int m_newReadCount;
        int m_ticksPs;
        int m_tick;
        int m_speed;
        int m_offset;
        int m_prevSpeed;
        int m_prevOffset;
        double m_ampli;
        
        
        Probe* m_probe;
        
        QDial* m_speedDial;
        //QDial* m_ampliDial;
        QDial* m_centeDial;
};

#endif

