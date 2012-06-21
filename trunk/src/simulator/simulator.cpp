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

#include "simulator.h"
#include "e-element.h"
#include "oscopewidget.h"


Simulator* Simulator::m_pSelf = 0l;

Simulator::Simulator( QObject* parent ) : QObject(parent)
{
    m_pSelf = this;

    m_isrunning = false;
    m_changed   = false;

    m_timerTick  = 50;
    m_reaStepsPT = 500;
    m_stepsPrea  = 100;
    m_mcuStepsPT = 16;
    m_reaClock  = 1e4;  // steps/Sec
    m_step      = 0;
    m_numEnodes = 0;
    m_simurate  = 0;
    m_timerId   = 0;

    m_avrCpu = 0l;
}
Simulator::~Simulator() { }

void Simulator::timerEvent( QTimerEvent* e )  //update at m_timerTick rate (20 ms, 50 Hz max)
{                                             // Used for Leds, probes, for update at frame rate 50 fps
    e->accept();
    if( !m_isrunning ) return;

    for( int i=0; i<m_reaStepsPT; i++ )     // Update at m_reaClock rate (100 us, 10 KHz max)
    {                                       // Used for reactive elements
        /*timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        long ns = ts.tv_nsec;*/

        for( int j=0; j<m_stepsPrea; j++ )  // Update at m_reaClock*100 rate (1 us, 1 MHz  max)
        {                                   // Used for all non-reactive elements
            m_step ++;

            if( m_avrCpu )                  // Update at mcu clock speed rate ( 16 MHz max )
            {
                for( int k=0; k<m_mcuStepsPT; k++ ) avr_run( m_avrCpu ); //avr.step();
            }

            if( !m_changed ) continue;      // Only calc matrix when something changed
            m_changed = false;

            if( !m_matrix.solveMatrix() )   // Try to solve matrix, if fail stop simulation
            {
                std::cout << "Simulator::runStep, Failed to solve Matrix"<<std::endl;
                pauseSim();
                return;
            }                               // m_matix sets the eNode voltages
        }

        std::vector<eElement*>::iterator it;
        for( it=m_reacStepList.begin(); it!=m_reacStepList.end(); it++ )
            (*it)->setVChanged();

        /*clock_gettime(CLOCK_REALTIME, &ts);
        long ns2 = ts.tv_nsec;
        std::cout << "solved in uS: " << (ns2-ns)/1000<<std::endl;*/
    }

    for( unsigned int i=0; i<m_updateList.size(); i++ )
        m_updateList[i]->updateStep();

    OscopeWidget::self()->step();
 }


void Simulator::runContinuous()
{
    sleep( 0.1 );   // Wait for previous steps to finish

    if( avr.getLoadStatus() ) m_avrCpu = avr.getCpu();
    else                      m_avrCpu = 0l;

    for( unsigned int i=0; i<m_elementList.size(); i++ ) m_elementList[i]->initialize();

    // Initialize Matrix and vectors
    m_numEnodes = m_eNodeList.size();// + m_numSources;
    std::cout << "\n" << m_numEnodes << " eNodes" << std::endl;

    for( int i=0; i<m_numEnodes; i++ )
    {
        eNode* enode = m_eNodeList.at(i);
        enode->setNodeNumber( i+1 );
        enode->initialize();
    }
    m_matrix.createMatrix( m_numEnodes );

    for( unsigned int i=0; i<m_elementList.size(); i++ ) m_elementList[i]->initialize();
    for( unsigned int i=0; i<m_elementList.size(); i++ ) m_elementList[i]->stamp();
    for( int i=0; i<m_numEnodes; i++ ) m_eNodeList[i]->stampMatrix();

    //m_matrix->printMatrix();
    if( !m_matrix.solveMatrix() )   // Try to solve matrix, if fail stop simulation
    {
        std::cout << "Simulator::runStep, Failed to solve Matrix"<<std::endl;
        pauseSim();
        return;
    }

    std::cout << "\nTimertick:        " << m_timerTick
            << " mS \nrea Steps/tick:   " << m_reaStepsPT
            << " \nsteps/rea:        " <<m_stepsPrea
            << "\nmcuStepsPT:       " << m_mcuStepsPT
            << std::endl;

    m_isrunning = true;
    std::cout << "\n    Running \n"<<std::endl;
    m_timerId = this->startTimer( m_timerTick );
}

void Simulator::stopSim()
{
    for( int i=0; i<m_numEnodes; i++ ) m_eNodeList[i]->setVolt( 0 ); //clear voltages
    for( unsigned int i=0; i<m_updateList.size(); i++ ) m_updateList[i]->updateStep();
    pauseSim();
}

void Simulator::pauseSim()
{
    //std::cout << "Pause\n"<<std::endl;
    m_isrunning = false;

    if( m_timerId != 0 )
    {
        this->killTimer( m_timerId );
        m_timerId = 0;
    }
}

void Simulator::simuRateChanged( int rate )
{
    if( rate != m_simurate )
    {
        m_simurate   = rate;
        m_timerTick  = 50;
        m_reaStepsPT = 500;
        m_stepsPrea  = 100;
        m_mcuStepsPT = rate/1000000;

        if( rate < 1000000 )
        {
            m_mcuStepsPT = 1;
            m_stepsPrea = rate/m_reaClock;
            if( rate<m_reaClock )
            {
                m_stepsPrea = 1;
                int timerSps = 1000/m_timerTick;
                m_reaStepsPT = rate/timerSps;
                if( rate<timerSps )
                {
                    m_reaStepsPT = 1;
                    m_timerTick = 1000/rate;
                }
            }
        }
        if( m_isrunning ) { pauseSim(); runContinuous(); }
    }
}

void Simulator::setChanged( bool changed ) { m_changed = changed; }
void Simulator::setNodeVolt( int enode, double v ) { m_eNodeList.at(enode)->setVolt( v ); }
bool Simulator::isRunning()  { return m_isrunning; }
//int  Simulator::simuRate()   { return m_simurate; }
int  Simulator::reaClock()  { return m_reaClock; }
//int  Simulator::stepsPT()    { return m_reaStepsPT; }
unsigned long long Simulator::step() { return m_step; }

void Simulator::addToEnodeList( eNode* nod )
{
    m_eNodeList.push_back( nod );
}
void Simulator::remFromEnodeList( eNode* nod, bool del )
{
    std::vector<eNode*>::iterator it;

    for( it=m_eNodeList.begin(); it!=m_eNodeList.end(); it++ )
        if( *it == nod ) { m_eNodeList.erase(it); break; }

    if( del ){ /*std::cout << " Simulator::remFromEnodeList Deleting eNod"<<std::endl;*/ delete nod; }
}

void Simulator::addToElementList( eElement* el )
{
    m_elementList.push_back( el );
}
void Simulator::remFromElementList( eElement* el )
{
    std::vector<eElement*>::iterator it;

    for( it=m_elementList.begin(); it!=m_elementList.end(); it++ )
        if( *it == el ) { m_elementList.erase(it); break; }
}

void Simulator::addToReacList( eElement* el )
{
    m_reacStepList.push_back( el );
}
void Simulator::remFromReacList( eElement* el )
{
    std::vector<eElement*>::iterator it;

    for( it=m_reacStepList.begin(); it!=m_reacStepList.end(); it++ )
        if( *it == el ) { m_reacStepList.erase(it); break; }
}

void Simulator::addToUpdateList( eElement* el )
{
    m_updateList.push_back( el );
}
void Simulator::remFromUpdateList( eElement* el )
{
    std::vector<eElement*>::iterator it;

    for( it=m_updateList.begin(); it!=m_updateList.end(); it++ )
        if( *it == el ) { m_updateList.erase(it); break; }
}

/*void Simulator::addToMcuList( eElement* el )
{
    if( !m_mcuStepList.contains(el) ) m_mcuStepList.append( el );
}
void Simulator::remFromMcuList( eElement* el ) { m_mcuStepList.removeOne( el ); }*/

#include "moc_simulator.cpp"
