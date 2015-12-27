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

#ifndef ESHIFTREG_H
#define ESHIFTREG_H

#include <bitset>

#include <QObject>

#include "e-logic_device.h"
//#include "e-node.h"


class eShiftReg : public eLogicDevice
{
    public:
        eShiftReg( string id );
        ~eShiftReg();

        virtual void initEpins();
        virtual void initialize();
        virtual void setVChanged();

    protected:
        //eSource* m_output[8];
        std::bitset<8> m_shiftReg;
        bool m_clockPrev;

};
#endif
