/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#ifndef EMOSFET_H
#define EMOSFET_H

#include "e-resistor.h"

class eSource;

class MAINMODULE_EXPORT eMosfet : public eResistor
{
    public:

        eMosfet( std::string id );
        ~eMosfet();

        void initialize();
        virtual void setVChanged();
        
        virtual bool pChannel()               { return m_Pchannel; }
        virtual void setPchannel( bool pc )   { m_Pchannel = pc; }
        
        virtual double RDSon()                { return m_RDSon; }
        virtual void  setRDSon( double rdson );
        
        virtual double threshold()            { return m_threshold; }
        virtual void  setThreshold( double th )
        { 
            m_threshold = th; 
            m_kRDSon = m_RDSon*(10-m_threshold);
            m_Gth = m_threshold-m_threshold/4;
        }
        
    protected:
        double m_accuracy;
        double m_lastCurrent;
        double m_threshold;
        double m_kRDSon;
        double m_RDSon;
        double m_gateV;
        double m_Gth;
        double m_Vs;

        bool m_Pchannel;
        bool m_Sfollow;
        
        eSource* m_gate;
};

#endif
