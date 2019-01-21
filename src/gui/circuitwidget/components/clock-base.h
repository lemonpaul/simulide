/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#ifndef CLOCKBASE_H
#define CLOCKBASE_H

#include "logicinput.h"
#include <QObject>

class MAINMODULE_EXPORT ClockBase : public LogicInput
{
    Q_OBJECT
    Q_PROPERTY( int  Freq    READ freq    WRITE setFreq   DESIGNABLE true USER true )
    Q_PROPERTY( bool Running READ running WRITE setRunning )

    public:
    
        QRectF boundingRect() const { return QRectF( m_area.x()-2, m_area.y()-2, m_area.width()+4 ,m_area.height()+4 ); }

        ClockBase( QObject* parent, QString type, QString id );
        ~ClockBase();

        virtual void updateStep();
        
        int freq();
        virtual void setFreq( int freq );
        
        bool running();
        virtual void setRunning( bool running );
        
    signals:
        void freqChanged();

    public slots:
        virtual void onbuttonclicked();
        virtual void remove();

    protected:
        bool m_isRunning;
        
        int m_freq;
        int m_step;
        
        double m_stepsPC;
};

#endif
