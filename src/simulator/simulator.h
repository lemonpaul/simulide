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
        void resumeSim();
        void stopSim();
        //void reset();
        //int simuRate();
        int simuRateChanged( int rate );

        int  reaClock();
        void setReaClock( int value );
        void setMcuClock( int value );
        
        //int  stepsPT();
        bool isRunning();
        
        unsigned long long step();

        void addToEnodeList( eNode* nod );
        void remFromEnodeList( eNode* nod, bool del );
        
        void addToElementList( eElement* el );
        void remFromElementList( eElement* el );
        
        void addToUpdateList( eElement* el );
        void remFromUpdateList( eElement* el );
        
        //void addToReactive( eElement* el );
        //void remFromReactive( eElement* el );
        
        void addToChangedFast( eElement* el );
        void remFromChangedFast( eElement* el );
        
        void addToChangedSlow( eElement* el );
        void remFromChangedSlow( eElement* el );
        
        //void addToRunList( eElement* el );
        //void remFromRunList( eElement* el );
        
        void addToNoLinList( eElement* el );
        void remFromNoLinList( eElement* el );

        void setNodeVolt( int enode, double v );
        //void setChanged( bool changed );

        void timerEvent( QTimerEvent* e );

    private:
 static Simulator* m_pSelf;
        
        void runCircuit();
        
        QFuture<void> m_CircuitFuture;

        CircMatrix m_matrix;

        std::vector<eNode*>    m_eNodeList;
        std::vector<eElement*> m_elementList;
        std::vector<eElement*> m_updateList;
        
        //QList<eElement*> m_reactiveList;
        QList<eElement*> m_changedFast;
        QList<eElement*> m_changedSlow;
        QList<eElement*> m_nonLinear;

        //bool m_changed;
        bool m_isrunning;
        int  m_timerId;

        int m_numEnodes;
        int m_simuRate;
        //int m_reaClock;
        //int m_reaStepsPT;
        int m_stepsPrea;
        int m_timerTick;
        int m_mcuStepsPT;
        
        int m_circuitRate;
        int m_slowCounter;

        unsigned long long m_step;
        unsigned long long m_lastStep;
        
        qint64        m_lastRefTime;
        QElapsedTimer m_RefTimer;
        
        AvrProcessor avr;
        avr_t*       m_avrCpu;
        
    #ifndef NO_PIC
        GpsimProcessor pic;
        pic_processor* m_picCpu;
    #endif
};
 #endif
