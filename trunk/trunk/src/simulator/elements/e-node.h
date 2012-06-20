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

#ifndef ENODE_H
#define ENODE_H

#include <QtGui>

//#include <string>
//#include <iostream>
//#include <unordered_map>

#include "e-pin.h"

class eElement;

class eNode
{
    public:
        eNode( QString id );
        ~eNode();

        QString itemId();

        //void connectorChanged( int c ); // Keep the num of connectors using this eNode
        void addEpin( ePin* epin );
        void remEpin( ePin* epin );

        void addToChangedList( eElement* el );
        void remFromChangedList( eElement* el );

        void stampCurrent( ePin* epin, double data );
        void stampAdmitance( ePin* epin, double data );

        void pinChanged( ePin* epin, int enodeNum );

        int  getNodeNumber();
        void setNodeNumber( int n );

        double getVolt();
        void  setVolt( double volt );

        void initialize();
        void stampMatrix();

        QList<ePin*> getEpins();

    private:
        QList<ePin*>     m_ePinList;
        QList<eElement*> m_changedList;

        QHash<ePin*, double> m_admitList;
        QHash<ePin*, double> m_currList;
        QHash<ePin*, int>    m_nodeList;
        //std::Hash<ePin*, double> m_pru;

        bool m_initialized;

        double m_volt;
        int   m_nodeNum;
        int   m_numCons;

        QString m_id;
};
#endif
