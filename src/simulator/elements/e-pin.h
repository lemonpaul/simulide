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

#ifndef EPIN_H
#define EPIN_H

#include <iostream>
#include <string>
using namespace std;

class eNode;

class ePin
{
    public:
        ePin( string id, int index );
        ~ePin();

        bool isConnected();
        void setConnected( bool connected );

        double getVolt();
        //void setVolt( double v );

        eNode* getEnode();
        void   setEnode( eNode* enode );

        //eNode* getEnodeComp();
        void   setEnodeComp( eNode* enode ); // The enode at other side of component

        void stampAdmitance( double data );
        void stampCurrent( double data );

        void reset();
        
        string getId() { return m_id; }

    protected:
        eNode* m_enode;
        eNode* m_enodeCon;

        string m_id;
        int m_index;

        bool m_connected;
};

#endif
