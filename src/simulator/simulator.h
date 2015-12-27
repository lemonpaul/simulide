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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QtWidgets>

#include <sys/time.h>
#include <iostream>

#include "e-node.h"
#include "circmatrix.h"
#include "avrprocessor.h"

#ifndef NO_PIC
#include "gpsimprocessor.h"
#endif

class eElement;

class Simulator : public QObject
{
    Q_OBJECT
    public:
        Simulator( QObject* parent=0 );
        ~Simulator();

 static Simulator* self() { return m_pSelf; }

        void runContinuous();
        void pauseSim();
        void stopSim();
        //void reset();
        void simuRateChanged( int rate );

        int  reaClock();
        //int  simuRate();
        //int  stepsPT();
        bool isRunning();
        bool isAnimated();

        unsigned long long step();

        void addToEnodeList( eNode* nod );
        void remFromEnodeList( eNode* nod, bool del );
        void addToElementList( eElement* el );
        void remFromElementList( eElement* el );
        void addToUpdateList( eElement* el );
        void remFromUpdateList( eElement* el );
        //void addToRunList( eElement* el );
        //void remFromRunList( eElement* el );
        void addToReacList( eElement* el );
        void remFromReacList( eElement* el );

        void setNodeVolt( int enode, double v );
        void setChanged( bool changed );

        void timerEvent( QTimerEvent* e );

    private:
 static Simulator* m_pSelf;

        CircMatrix m_matrix;

        std::vector<eNode*>    m_eNodeList;
        std::vector<eElement*> m_elementList;
        std::vector<eElement*> m_reacStepList;
        std::vector<eElement*> m_updateList;

        bool m_changed;
        bool m_isrunning;
        int  m_timerId;

        int m_numEnodes;
        int m_simurate;
        int m_reaClock;
        int m_reaStepsPT;
        int m_stepsPrea;
        int m_timerTick;
        int m_mcuStepsPT;

        unsigned long long m_step;
        
        AvrProcessor avr;
        avr_t*       m_avrCpu;
        
		#ifndef NO_PIC
        GpsimProcessor pic;
        pic_processor* m_picCpu;
		#endif
};
 #endif
