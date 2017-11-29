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

#include <iostream>
#include <cmath>

#include "circmatrix.h"
#include "simulator.h"

CircMatrix* CircMatrix::m_pSelf = 0l;

CircMatrix::CircMatrix()
{
    m_pSelf = this;
    m_numEnodes = 0;
}
CircMatrix::~CircMatrix(){}

void CircMatrix::createMatrix( QList<eNode*> &eNodeList, QList<eElement*> &elementList )
{
    m_elementList = elementList;
    m_eNodeList = &eNodeList;
    m_numEnodes = eNodeList.size();

    m_circMatrix.clear();
    m_nodeVolt.clear();
    m_coefVect.clear();
    
    a.clear();
    b.clear();
    ipvt.clear();

    m_circMatrix.resize( m_numEnodes , std::vector<double>( m_numEnodes , 0 ) );
    m_nodeVolt.resize( m_numEnodes , 0 );
    m_coefVect.resize( m_numEnodes , 0 );
    
    
    a.resize( m_numEnodes , std::vector<double>( m_numEnodes , 0 ) );
    b.resize( m_numEnodes , 0 );
    ipvt.resize( m_numEnodes , 0 );
    
    // Initialize Matrixs & Vectors
    for( int i=0; i<m_numEnodes; i++ )
    {
        for( int j=0; j<m_numEnodes; j++ )
        {
             m_circMatrix[i][j] = 0;
             a[i][j] = 0;
         }
        m_coefVect[i] = 0;
        m_nodeVolt[i] = 0;
        ipvt[i] = 0;
        b[i] = 0;
    }
    
    m_admitChanged = false;
    m_currChanged  = false;
    
     // Initialize eNodes
    std::cout <<"\nInitializing "<< m_numEnodes << " eNodes"<< std::endl;
    for( int i=0; i<m_numEnodes; i++ )
    {
        eNode* enode = eNodeList.at(i);
        enode->setNodeNumber( i+1 );
        enode->initialize();
    }
    
    foreach( eElement* el, m_elementList ) el->initialize();
    foreach( eElement* el, m_elementList ) el->stamp();
    for( int i=0; i<m_numEnodes; i++ ) m_eNodeList->at(i)->stampMatrix();
}

void CircMatrix::stampMatrix( int row, int col, double value )
{
    if(( row == 0 )|( col == 0 )) return;
    //std::cout <<"CircMatrix::stampMatrix "<< "  Row: "<< row<< "  Col: " << col << value <<std::endl;
    m_admitChanged = true;
    m_circMatrix[row-1][col-1] = value;      // eNode numbers start at 1
}

void CircMatrix::stampCoef( int row, double value )
{
    if( row == 0 ) return;
    m_currChanged = true;
    m_coefVect[row-1] = value;
    //std::cout<<"\n row: "<<row <<  "value:"<< value<<std::endl;
    //std::cout<< "coefs:\n" << m_coefVect << std::endl;
}

bool CircMatrix::solveMatrix()
{
    if( !m_admitChanged && !m_currChanged ) return true;
    //if(!m_admitChanged )
    //qDebug() <<"---------" << m_admitChanged << m_currChanged;

    int n = m_numEnodes;

    for( int i=0; i<n; i++)
    {
        if( m_admitChanged )
        {
            for( int j=0; j<n; j++)
            {
                a[i][j] = m_circMatrix[i][j];
               // std::cout << matriz[i][j] << "\t";
            }
        }
        b[i] = m_coefVect[i];
        //m_coefVect[i] = 0;
        //std::cout << "\t"<< coef[i] << std::endl;
    }
    
    if( m_admitChanged )      // Only factor admitance matrix if changed
    {
    // factors a matrix into upper and lower triangular matrices by
    // gaussian elimination.  On entry, a[0..n-1][0..n-1] is the
    // matrix to be factored.  ipvt[] returns an integer vector of pivot
    // indices, used in the lu_solve() routine.

        double scaleFactors[n];
        int i,j,k;

        // divide each row by its largest element, keeping track of the
        // scaling factors
        for( i=0; i<n; i++ )
        {
            double largest = 0;
            for( j=0; j<n; j++ )
            {
                double x = std::fabs( a[i][j] );
                if( x > largest ) largest = x;
            }
            // if all zeros, it's a singular matrix
            if( largest == 0 ) return false;
            scaleFactors[i] = 1.0/largest;
        }

        // use Crout's method; loop through the columns
        for( j=0; j<n; j++ )
        {
            // calculate upper triangular elements for this column
            for( i=0; i<j; i++ )
            {
                double q = a[i][j];
                for( k=0; k<i; k++ ) q -= a[i][k]*a[k][j];

                a[i][j] = q;
            }

            // calculate lower triangular elements for this column
            double largest = 0;
            int largestRow = -1;
            for( i=j; i<n; i++ )
            {
                double q = a[i][j];
                for( k=0; k<j; k++ ) q -= a[i][k]*a[k][j];

                a[i][j] = q;
                double x = std::fabs( q );
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
                for( k=0; k<n; k++ )
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
                for( i=j+1; i<n; i++ ) a[i][j] *= mult;
            }
        }
    }
    
    
/*for( int i=0; i<m_numEnodes; i++ )
{
    for( int j=0; j<m_numEnodes; j++ )
    {
        std::cout << a[i][j] <<"\t";
    }
    std::cout << "\t";
    std::cout << m_coefVect[i]<< std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}*/
    
    
// Solves the set of n linear equations using a LU factorization
// previously performed by solveMatrix.  On input, b[0..n-1] is the right
// hand side of the equations, and on output, contains the solution.

    int i;
    // find first nonzero b element
    for( i=0; i<n; i++ )
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
        for( j=bi; j<i; j++ ) tot -= a[i][j]*b[j];

        b[i] = tot;
    }
    for( i=n-1; i>=0; i-- )
    {
        double tot = b[i];

        // back-substitution using the upper triangular matrix
        for( int j=i+1; j<n; j++ ) tot -= a[i][j]*b[j];

        b[i] = tot/a[i][i];
    }
    m_admitChanged = false;
    m_currChanged  = false;
    
    for( i=0; i<m_numEnodes; i++ )  
    {
        //Simulator::self()->setNodeVolt( i, b[i] );
        double volt = b[i];
        if( std::isnan( volt ) ) return false;

        m_eNodeList->at(i)->setVolt( volt ); 
    }
    return true;
}


void CircMatrix::simplify()
{
    //printMatrix();
    QList<int> singleEls;
    for( int y=0; y<m_numEnodes; y++ )       // Find Single Element Rows
    {
        int nonCero = 0;
        for( int x=0; x<m_numEnodes; x++ )            // Find Cero Row
        {
            if( m_circMatrix[x][y] == 0 )continue;
            nonCero++;
        }
        if( nonCero == 1 ) singleEls.append( y );
    }
    m_eNodeList2.clear();
    int newY = 0;
    for( int y=0; y<m_numEnodes; y++ )
    {
        if( singleEls.contains( y ) ) 
        {
            m_eNodeList->at( y )->setSingle( true );
            continue;
        }
        newY++;
        eNode* enode = m_eNodeList->at( y );
        //enode->setNodeNumber( newY );
        //enode->initialize();
        m_eNodeList2.append( enode );
    }
    std::cout << "\nSimplify Matrix "<<std::endl;
    createMatrix( m_eNodeList2, m_elementList );
    
    //printMatrix();
    /*for( int y=0; y<m_numEnodes; y++ )
    {
        bool ceroel = false;
        int nonCero = 0;
        int toSwap = 0;
        for( int x=y+1; x<m_numEnodes; x++ )
        {
            if( m_circMatrix[x][y] == 0 )
            {
                ceroel = true;
                //std::cout << "\nceroel:\n"<< x<<y<<std::endl;
            }
            else 
            {
                nonCero++;
                if( ceroel ) toSwap = x;          // First non-cero
            }
            //std::cout << "\nCOL:"<< x<<" "<<nonCero<<" "<<toSwap<<" val= "<<m_circMatrix[x][y]<<std::endl;
        }
        //std::cout << "\nROW:"<< y<<" "<<nonCero<<" "<<toSwap<<std::endl;
        if(( nonCero == 1 )&( toSwap > 1 )) swap( toSwap, y );
    }*/
}


/*void CircMatrix::splitNode( int node )
{
    
}*/
void CircMatrix::swap( int x, int y )
{
    //std::cout << "\nNo ceroel:\n"<< x<<y<<std::endl;
    double p = m_circMatrix[x][x]; //diagonal elm
    double end = m_numEnodes-1;
    double destRow = 0;
    
    for( double Y=y; Y<x; Y++ )
        if( m_circMatrix[x][Y] != 0 ) destRow = Y+1;
        
    //if( destRow == end ) continue;

    for( double Y=y; Y<destRow; Y++ ) // Move Row & Column
    {
        m_circMatrix[destRow][Y] = m_circMatrix[x][Y];
        m_circMatrix[x][Y] = 0;
        m_circMatrix[Y][destRow] = m_circMatrix[Y][x];
        m_circMatrix[Y][x] = 0;
    }
    printMatrix();
    
    double c = m_coefVect[x];
    for( double Y=end; Y>destRow; Y-- )// Move diagonal
    {
        for( double X=end; X>destRow; X-- )
        {
            m_circMatrix[X][Y] = m_circMatrix[X-1][Y-1];
            m_circMatrix[X-1][Y-1] = 0;
        }
        m_coefVect[Y] = m_coefVect[Y-1];
    }
    m_circMatrix[destRow][destRow] = p; //diagonal elm
    m_coefVect[destRow] = c;            // Right side
    m_eNodeList->insert( destRow,( m_eNodeList->takeAt( x ) )); // Move eNodes like Right side

    printMatrix();
}

void CircMatrix::printMatrix()
{
    std::cout << "\nAdmitance Matrix:\n"<< std::endl;
    for( int i=0; i<m_numEnodes; i++ )
    {
        for( int j=0; j<m_numEnodes; j++ )
        {
            std::cout << m_circMatrix[i][j] <<"\t";
        }
        std::cout << "\t";
        std::cout << m_coefVect[i]<< std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    
    //std::cout << "\nSantized Matrix:\n"<< std::endl;
    
}

