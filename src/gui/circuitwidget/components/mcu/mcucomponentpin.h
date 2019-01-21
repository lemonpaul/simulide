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

#ifndef MCUCOMPONENTPIN_H
#define MCUCOMPONENTPIN_H

#include "mcucomponent.h"
#include "e-source.h"
#include "pin.h"


class MAINMODULE_EXPORT McuComponentPin : public QObject, public eSource
{
    Q_OBJECT
    public:
        McuComponentPin( McuComponent *mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );
        ~McuComponentPin();

        Pin* pin() const { return ( static_cast<Pin*>(m_ePin[0]) ); }

        virtual void initialize();
        virtual void resetState();
        void terminate();

        void move( int dx, int dy );
        
        void resetOutput();
        
        int angle() { return m_angle;}
        
        QString ptype() { return m_type; }

    protected:
        McuComponent* m_mcuComponent;
        
        bool m_attached;
        bool m_isInput;
        bool m_openColl;
        
        char m_port;
        int  m_pinN;

        int m_pinType;
        int m_angle;

        QString m_type;
        QString m_id;
};

#endif


