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

#ifndef EDIODE_H
#define EDIODE_H

#include "e-resistor.h"

//class LibraryItem;

class eDiode : public eResistor
{
    public:
        eDiode(  string id );
        ~eDiode();

        double threshold() { return m_threshold; }
        void  setThreshold( double threshold );

        void initialize();

        virtual void setVChanged();

        void  setRes( double resist );

    protected:
        void updateVI();

        double m_pVolt;
        double m_nVolt;
        double m_deltaV;
        double m_threshold;
        double m_imped;
};
#endif
