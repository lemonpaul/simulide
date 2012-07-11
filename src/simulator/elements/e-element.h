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

#ifndef EELEMENT_H
#define EELEMENT_H

#include "simulator.h"

class ePin;

class eElement
{
    public:
        eElement(string id=0);
        ~eElement();

        virtual void initEpins();
        virtual void setNumEpins( int n );

        virtual void initialize(){;}
        virtual void stamp(){;}

        virtual void updateStep(){;}
        virtual void setVChanged(){;}

        virtual ePin* getEpin( int pin );
        virtual ePin* getEpin( QString pinName );

        string getId(){ return m_elmId; }

        const double cero_doub;
        const double high_imp;
        const double digital_high;
        const double digital_low;
        const double digital_threshold;


    protected:
        std::vector<ePin*> m_ePin;

        string m_elmId;
};

#endif

