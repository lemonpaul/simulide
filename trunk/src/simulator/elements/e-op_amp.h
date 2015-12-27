/***************************************************************************
 *   Copyright (C) 2003-2006 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef EOPAMP_H
#define EOPAMP_H

#include "e-element.h"
#include "e-resistor.h"
#include "e-source.h"

class eOpAmp : public eElement
{
    public:

        eOpAmp( string id );
        virtual ~eOpAmp();

        virtual void initialize();
        virtual void setVChanged();
        
        virtual double gain()              {return m_gain;}
        virtual void setGain( double gain ){m_gain = gain;}
        
    protected:
        eSource* m_output;
        eResistor* m_inResistor;
        //eSource* m_inputInv;
        //eSource* m_inputNinv;
        //eSource* m_powerPos;
        //eSource* m_powerNeg;
        
        double m_gain;
        double m_voltPos;
        double m_voltNeg;
        double m_lastVolt;
};


#endif
