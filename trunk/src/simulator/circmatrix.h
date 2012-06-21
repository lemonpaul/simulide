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

#ifndef CIRCMATRIX_H
#define CIRCMATRIX_H

#include <vector>
#include <iostream>
#include <cmath>

#include "e-node.h"

class eElement;

class CircMatrix
{
    public:
        CircMatrix();
        ~CircMatrix();

 static CircMatrix* self() { return m_pSelf; }

        void printMatrix();
        void createMatrix( int numEnodes );
        void stampMatrix( int row, int col, double value );
        void stampCoef( int row, double value );
        bool solveMatrix();

    private:
 static CircMatrix* m_pSelf;

        std::vector<std::vector<double> > m_circMatrix;
        std::vector<double> m_coefVect;
        std::vector<double> m_nodeVolt;

        int m_numEnodes;
};
 #endif

