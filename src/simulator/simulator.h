/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <qtconcurrentrun.h>
#include <QElapsedTimer>

#include "e-node.h"
#include "e-element.h"
#include "circmatrix.h"
#include "baseprocessor.h"

class MAINMODULE_EXPORT Simulator : public QObject
{
    Q_OBJECT
    public:
        Simulator( QObject* parent=0 );
        ~Simulator();

 static Simulator* self() { return m_pSelf; }

        void runContinuous();
        void stopTimer();
        void pauseSim();
        void resumeSim();
        void stopSim();
        void runExtraStep();
        
        int simuRate() { return m_simuRate; }
        int simuRateChanged( int rate );

        int  reaClock();
        void setReaClock( int value );
        int  noLinClock();
        void setNoLinClock( int value );
        void setMcuClock( int value );
        int  nlAcc();
        void setNlAcc( int ac );
        double NLaccuracy();
        
        bool isRunning();
        
        unsigned long long step();

        QList<eNode*> geteNodes() { return m_eNodeList; }

        void addToEnodeList( eNode* nod );
        void remFromEnodeList( eNode* nod, bool del );
        
        void addToChangedNodeList( eNode* nod );
        void remFromChangedNodeList( eNode* nod );
        
        void addToElementList( eElement* el );
        void remFromElementList( eElement* el );
        
        void addToUpdateList( eElement* el );
        void remFromUpdateList( eElement* el );
        
        void addToChangedFast( eElement* el );
        void remFromChangedFast( eElement* el );
        
        void addToReactiveList( eElement* el );
        void remFromReactiveList( eElement* el );
        
        void addToSimuClockList( eElement* el );
        void remFromSimuClockList( eElement* el );
        
        void addToNoLinList( eElement* el );
        void remFromNoLinList( eElement* el );
        
        void addToMcuList( BaseProcessor* proc );
        void remFromMcuList( BaseProcessor* proc );

        void timerEvent( QTimerEvent* e );

    private:
 static Simulator* m_pSelf;
        
        void runCircuit();
        
        inline void solveMatrix();

        QFuture<void> m_CircuitFuture;

        CircMatrix m_matrix;

        QList<eNode*>    m_eNodeList;
        QList<eNode*>    m_eChangedNodeList;
        
        QList<eElement*> m_elementList;
        QList<eElement*> m_updateList;
        
        QList<eElement*> m_changedFast;
        QList<eElement*> m_reactiveList;
        QList<eElement*> m_nonLinear;
        QList<eElement*> m_simuClock;
        QList<BaseProcessor*> m_mcuList;

        bool m_isrunning;
        bool m_paused;
        bool m_error;
        int  m_timerId;
        
        int m_nlAcc;

        int m_numEnodes;
        int m_simuRate;
        int m_stepsPrea;
        int m_stepsNolin;
        int m_timerTick;
        int m_mcuStepsPT;
        
        int m_circuitRate;
        int m_noLinCounter;
        int m_reacCounter;

        unsigned long long m_step;
        unsigned long long m_lastStep;
        
        qint64        m_lastRefTime;
        QElapsedTimer m_RefTimer;
};
 #endif
