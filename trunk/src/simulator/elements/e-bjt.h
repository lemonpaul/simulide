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

#ifndef EBJT_H
#define EBJT_H

#include "e-diode.h"


class MAINMODULE_EXPORT eBJT : public eResistor
{
    public:

        eBJT( std::string id );
        virtual ~eBJT();

        virtual void initialize();
        virtual void setVChanged();
        
        virtual double gain()              {return m_gain;}
        virtual void setGain( double gain ){m_gain = gain;}
        
        virtual double pnp()              {return m_PNP;}
        virtual void setPnp( double pnp ) {m_PNP = pnp;}
        
    protected:
        double m_voltPos;
        double m_voltNeg;
        double m_lastOut;
        
        int m_gain;
        
        bool m_PNP;
        
        eDiode* m_BEdiode;
        eDiode* m_BCdiode;
};


#endif
