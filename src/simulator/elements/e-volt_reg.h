/***************************************************************************
 *   Copyright (C) 2018 by santiago González                               *
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

#ifndef EVOLTREG_H
#define EVOLTREG_H

#include "e-element.h"
#include "e-source.h"

class MAINMODULE_EXPORT eVoltReg : public eElement
{
    public:

        eVoltReg( std::string id );
        virtual ~eVoltReg();

        virtual void initialize();
        virtual void setVChanged();
        
        virtual double vRef()              {return m_vRef;}
        virtual void setVRef( double vref ){m_vRef = vref;}
        
    protected:
        eSource* m_output;
        
        double m_vRef;
        double m_voltPos;
        double m_voltNeg;
        double m_lastOut;
};


#endif
