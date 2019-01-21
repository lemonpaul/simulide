/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#ifndef SWITCH_H
#define SWITCH_H

#include "switch_base.h"
#include "e-resistor.h"

class LibraryItem;

class MAINMODULE_EXPORT Switch : public SwitchBase
{
    Q_OBJECT
    Q_PROPERTY( int    Poles READ poles    WRITE setPoles    DESIGNABLE true USER true )
    Q_PROPERTY( bool   DT    READ dt       WRITE setDt       DESIGNABLE true USER true )
    Q_PROPERTY( bool Norm_Close READ nClose WRITE setNClose  DESIGNABLE true USER true )
    
    public:

        Switch( QObject* parent, QString type, QString id );
        ~Switch();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem *libraryItem();
        
        int poles() const;
        void setPoles( int poles );

        bool dt() const;
        void setDt( bool dt );
        
        bool nClose() const;
        void setNClose( bool nc );
        
        virtual void initialize();
        
        void updateStep();

        virtual void paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget );

    public slots:
        void onbuttonclicked();
        
    protected:
        virtual void setSwitch( bool on );
        void  SetupSwitches( int poles, int throws );
        
        std::vector<eResistor*> m_switches;

        bool m_closed;
        bool m_nClose;

        int m_numPoles;
        int m_numthrows;
};

#endif
