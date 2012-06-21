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

#include "circmatrix.h"
#include "simulator.h"

CircMatrix* CircMatrix::m_pSelf = 0l;

CircMatrix::CircMatrix()
{
    m_pSelf = this;
    m_numEnodes = 0;
}
CircMatrix::~CircMatrix(){}

void CircMatrix::createMatrix( int numEnodes )
{
    // Clear matrix
    for( int i=0; i<m_numEnodes; i++ )
    {
        for( int j=0; j<m_numEnodes; j++ ) m_circMatrix[i][j] = 0;
        m_coefVect[i] = 0;
    }

    m_numEnodes = numEnodes;
    m_circMatrix.clear();
    m_circMatrix.resize( m_numEnodes , std::vector<double>( m_numEnodes , 0 ) );
    m_nodeVolt.resize( m_numEnodes , 0 );
    m_coefVect.resize( m_numEnodes , 0 );
}

void CircMatrix::stampMatrix( int row, int col, double value )
{
    //std::cout <<"CircMatrix::stampMatrix"<< row<<col<<value<<std::endl;
    Simulator::self()->setChanged( true );
    m_circMatrix[row-1][col-1] = value;
}

void CircMatrix::stampCoef( int row, double value )
{
    Simulator::self()->setChanged( true );
    m_coefVect[row-1] = value;
    //std::cout<<"\n row: "<<row <<  "value:"<< value<<std::endl;
    //std::cout<< "coefs:\n" << m_coefVect << std::endl;
}

bool CircMatrix::solveMatrix()
{
    int n = m_numEnodes;
    int ipvt[m_numEnodes];
    double b[m_numEnodes];
    double a[m_numEnodes][m_numEnodes];

    for( int i=0; i<m_numEnodes; i++)
    {
        for( int j=0; j<m_numEnodes; j++)
        {
            a[i][j] = m_circMatrix[i][j];
           // std::cout << matriz[i][j] << "\t";
        }
        b[i] = m_coefVect[i];
        //std::cout << "\t"<< coef[i] << std::endl;
    }

    // factors a matrix into upper and lower triangular matrices by
    // gaussian elimination.  On entry, a[0..n-1][0..n-1] is the
    // matrix to be factored.  ipvt[] returns an integer vector of pivot
    // indices, used in the lu_solve() routine.

    double scaleFactors[n];
    int i,j,k;

    // divide each row by its largest element, keeping track of the
    // scaling factors
    for( i = 0; i != n; i++ )
    {
        double largest = 0;
        for( j = 0; j != n; j++ )
        {
            double x = std::abs( a[i][j] );
            if( x > largest ) largest = x;
        }
        // if all zeros, it's a singular matrix
        if( largest == 0 ) return false;
        scaleFactors[i] = 1.0/largest;
    }

    // use Crout's method; loop through the columns
    for( j=0; j!=n; j++ )
    {
        // calculate upper triangular elements for this column
        for( i=0; i!=j; i++ )
        {
            double q = a[i][j];
            for( k=0; k!=i; k++ ) q -= a[i][k]*a[k][j];

            a[i][j] = q;
        }

        // calculate lower triangular elements for this column
        double largest = 0;
        int largestRow = -1;
        for( i = j; i != n; i++ )
        {
            double q = a[i][j];
            for( k = 0; k != j; k++ ) q -= a[i][k]*a[k][j];

            a[i][j] = q;
            double x = std::abs( q );
            if( x >= largest )
            {
                largest = x;
                largestRow = i;
            }
        }

        // pivoting
        if( j != largestRow )
        {
            double x;
            for( k = 0; k != n; k++ )
            {
                x = a[largestRow][k];
                a[largestRow][k] = a[j][k];
                a[j][k] = x;
            }
            scaleFactors[largestRow] = scaleFactors[j];
        }

        // keep track of row interchanges
        ipvt[j] = largestRow;

        // avoid zeros
        if( a[j][j] == 0.0 ) a[j][j]=1e-18;

        if( j != n-1 )
        {
            double mult = 1.0/a[j][j];
            for( i = j+1; i != n; i++ ) a[i][j] *= mult;
        }
    }

// Solves the set of n linear equations using a LU factorization
// previously performed by solveMatrix.  On input, b[0..n-1] is the right
// hand side of the equations, and on output, contains the solution.

    // find first nonzero b element
    for( i=0; i!=n; i++ )
    {
        int row = ipvt[i];

        double swap = b[row];
        b[row] = b[i];
        b[i] = swap;
        if( swap != 0 ) break;
    }

    int bi = i++;
    for( /*i = bi*/; i < n; i++ )
    {
        int row = ipvt[i];
        int j;
        double tot = b[row];

        b[row] = b[i];
        // forward substitution using the lower triangular matrix
        for( j = bi; j < i; j++ ) tot -= a[i][j]*b[j];

        b[i] = tot;
    }
    for( i=n-1; i>=0; i-- )
    {
        double tot = b[i];

        // back-substitution using the upper triangular matrix
        for( int j=i+1; j != n; j++ ) tot -= a[i][j]*b[j];

        b[i] = tot/a[i][i];
    }
    for( int i=0; i<m_numEnodes; i++ )  Simulator::self()->setNodeVolt( i, b[i] );
    return true;
}

void CircMatrix::printMatrix()
{
    std::cout << "\nAdmitance Matrix:\n";
    for( int i=0; i<m_numEnodes; i++ )
    {
        for( int j=0; j<m_numEnodes; j++ )
        {
            std::cout << m_circMatrix[i][j] <<"\t";
        }
        std::cout << m_coefVect[i];
        std::cout << std::endl;
    }
}
