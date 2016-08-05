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
 
#include <qtconcurrentrun.h>

#include "simulator.h"
#include "e-element.h"
#include "oscopewidget.h"
#include "plotterwidget.h"
#include "outpaneltext.h"
#include "mainwindow.h"


Simulator* Simulator::m_pSelf = 0l;

Simulator::Simulator( QObject* parent ) : QObject(parent)
{
    m_pSelf = this;

    m_isrunning = false;
    //m_changed   = false;

    m_step       = 0;
    m_numEnodes  = 0;
    m_timerId    = 0;
    m_lastStep   = 0;
    m_lastRefTime = 0;
    m_mcuStepsPT = 16;
    m_simuRate   = 1000000;

    m_avrCpu = 0l;
#ifndef NO_PIC
    m_picCpu = 0l;
#endif
    
    m_RefTimer.start();
}
Simulator::~Simulator() 
{ 
    m_CircuitFuture.waitForFinished(); 
    //m_RefTimer.invalidate();
}

void Simulator::timerEvent( QTimerEvent* e )  //update at m_timerTick rate (50 ms, 20 Hz max)
{
    e->accept();
    if( !m_isrunning ) return;
    
    // Run Circuit in a paralle thread
    m_CircuitFuture.waitForFinished();
    m_CircuitFuture = QtConcurrent::run( this, &Simulator::runCircuit );
    //runCircuit();

    // Get Real Simulation Speed
    qint64 refTime      = m_RefTimer.nsecsElapsed();
    qint64 deltaRefTime = refTime-m_lastRefTime;  

    if( deltaRefTime >= 1e9 )          // We want steps per Sec = 1e9 ns
    {
        //deltaRefTime = deltaRefTime/1e6;
        //qint64 stepsPerSec = (m_step-m_lastStep)*m_mcuStepsPT*1e3/deltaRefTime;
        qint64 stepsPerSec = (m_step-m_lastStep)*1e9/deltaRefTime;
        MainWindow::self()->setRate( (stepsPerSec*100)/m_simuRate );
        m_lastStep    = m_step;
        m_lastRefTime = refTime;
    }
    // Run Graphic Elements
    PlotterWidget::self()->step();
    OscopeWidget::self()->step();
    OutPanelText::self()->step();
    foreach( eElement* el, m_updateList ) el->updateStep();
}
        
void Simulator::runCircuit()
{
    for( int i=0; i<m_circuitRate; i++ )
    {
        m_step ++;
        
        // Run Slow Elements
        if( ++m_slowCounter == m_stepsPrea ) 
        {
            m_slowCounter = 0;
            
            foreach( eElement* el, m_changedSlow ) el->setVChanged();  
            m_changedSlow.clear();
        }
        OscopeWidget::self()->setData();
        
        // Run Mcus
        if( m_avrCpu ) for( int k=0; k<m_mcuStepsPT; k++ ) m_avrCpu->run(m_avrCpu);//avr_run( m_avrCpu ); //avr.step();
    #ifndef NO_PIC
        if( m_picCpu ) for( int k=0; k<m_mcuStepsPT; k++ ) pic.step();
    #endif
        
        // Run Fast elements
        foreach( eElement* el, m_changedFast ) el->setVChanged();
        m_changedFast.clear();

        // Solve Matrix
        foreach( eNode* node,  m_eNodeList ) node->stampMatrix();

        if( !m_matrix.solveMatrix() )            // Try to solve matrix,
        {                                     // if fail stop simulation
            std::cout << "Simulator::runStep, Failed to solve Matrix" << std::endl;
            stopSim();
            return;                  // m_matrix sets the eNode voltages
        }
        // Run Non-Linear elements 
        if( !m_nonLinear.isEmpty() )
        {
            foreach( eElement* el, m_nonLinear ) el->setVChanged();
            m_nonLinear.clear();
            // Solve Matrix
            foreach( eNode* node,  m_eNodeList ) node->stampMatrix();

            if( !m_matrix.solveMatrix() )            // Try to solve matrix,
            {                                     // if fail stop simulation
                std::cout << "Simulator::runStep, Failed to solve Matrix" << std::endl;
                MainWindow::self()->powerCircOff(); //stopSim();
                return;                  // m_matrix sets the eNode voltages
            }
        }
    }
 }

void Simulator::runContinuous()
{
    // Get Mcus
    if( avr.getLoadStatus() ) m_avrCpu = avr.getCpu();
    else                      m_avrCpu = 0l;
#ifndef NO_PIC
    if( pic.getLoadStatus() ) m_picCpu = pic.getCpu();
    else                      m_picCpu = 0l;
#endif

    // Initialize all Elements
    foreach( eElement* el, m_elementList ) el->initialize();
    
    // Initialize eNodes
    qDebug() <<"Initialize eNodes";
    m_numEnodes = m_eNodeList.size(); 
    for( int i=0; i<m_numEnodes; i++ )
    {
        eNode* enode = m_eNodeList.at(i);
        enode->setNodeNumber( i+1 );
        enode->initialize();
    }
    std::cout << "\n" << m_numEnodes << " eNodes" << std::endl;
    
    // Initialize Circuit
    m_matrix.createMatrix( m_numEnodes );
    foreach( eElement* el, m_elementList ) el->initialize();
    foreach( eElement* el, m_elementList ) el->stamp();
    foreach( eNode* node,  m_eNodeList   ) node->stampMatrix();

    // Try to solve matrix, if fail stop simulation
    if( !m_matrix.solveMatrix() )                
    {
        std::cout << "Simulator::runContinuous, Failed to solve Matrix"
                  <<  std::endl;
        MainWindow::self()->powerCircOff();
        //pauseSim();
        return;
    }
    //m_matrix.printMatrix();
    
    m_slowCounter = 0;
    
    simuRateChanged( m_simuRate );
    
    m_isrunning = true;
    std::cout << "\n    Running \n"<<std::endl;
    m_timerId = this->startTimer( m_timerTick );
}

void Simulator::stopSim()
{
    m_CircuitFuture.waitForFinished();

    if( !m_isrunning ) return;
    
    pauseSim();
    
    foreach( eNode* node,  m_eNodeList  ) node->setVolt( 0 ); //clear voltages
    foreach( eElement* el, m_updateList ) el->updateStep();
    if( m_avrCpu ) avr_reset( m_avrCpu ); 
#ifndef NO_PIC
    if( m_picCpu ) pic.reset();
#endif
    
    MainWindow::self()->setRate( 0 );
}

void Simulator::pauseSim()
{
    m_CircuitFuture.waitForFinished();
    
    if( m_timerId != 0 )
    {
        this->killTimer( m_timerId );
        m_timerId = 0;
    }
    m_isrunning = false;
    
    std::cout << "\n    Simulation Paused \n" << std::endl;
}

void Simulator::resumeSim()
{
    std::cout << "\n    Resuming Simulation\n" << std::endl;
    m_timerId = this->startTimer( m_timerTick );
    m_isrunning = true;
}

int Simulator::simuRateChanged( int rate )
{
    if( rate > 1e6 ) rate = 1e6;
    
    m_timerTick  = 50;
    int fps = 1000/m_timerTick;
    
    m_circuitRate = rate/fps;
    
    if( rate < fps )
    {
        fps = rate;
        m_circuitRate = 1;
        m_timerTick = 1000/rate;
    }

    if( m_isrunning ) 
    {
        pauseSim();
        resumeSim();
    }
    PlotterWidget::self()->setPlotterTick( m_circuitRate*20 );
    
    m_simuRate = m_circuitRate*fps;
    
    std::cout << "\nFPS:              " << fps
              << "\nCircuit Rate:     " << m_circuitRate
              << "\nMcu     Rate:     " << m_mcuStepsPT
              << std::endl
              << "\nSimulation Speed: " << m_simuRate
              << "\nReactive   Speed: " << m_simuRate/m_stepsPrea
              << "\nReactive SubRate: " << m_stepsPrea
              << std::endl;

    
    return m_simuRate;
}

/*int  Simulator::simuRate()   
{ 
    return m_simuRate; 
}*/

/*void Simulator::setChanged( bool changed ) 
{ 
    m_changed = changed; 
}*/

void Simulator::setNodeVolt( int enode, double v ) 
{ 
    if( isnan(v) )
    {
        //qDebug() << "Simulator::setNodeVolt NaN";
        MainWindow::self()->setRate( -1 );
        return;
    }
    m_eNodeList.at(enode)->setVolt( v ); 
}

bool Simulator::isRunning()  
{ 
    return m_isrunning; 
}

int Simulator::reaClock()  
{ 
    return m_stepsPrea;
}

void Simulator::setReaClock( int value )
{
    bool running = m_isrunning;
    if( running ) stopSim();
    
    if     ( value < 3  ) value = 3;
    else if( value > 50 ) value = 50;
    
    m_stepsPrea = value;
    
    if( running ) runContinuous();
}
void Simulator::setMcuClock( int value )
{
    m_mcuStepsPT = value;
}

//int  Simulator::stepsPT()    { return m_reaStepsPT; }

unsigned long long Simulator::step() 
{ 
    return m_step; 
}

void Simulator::addToEnodeList( eNode* nod )
{
    m_eNodeList.push_back( nod );
}

void Simulator::remFromEnodeList( eNode* nod, bool del )
{
    std::vector<eNode*>::iterator it;

    for( it=m_eNodeList.begin(); it!=m_eNodeList.end(); it++ )
        if( *it == nod ) { m_eNodeList.erase(it); break; }

    if( del ){ delete nod; }
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

/*void Simulator::addToReactive( eElement* el )   
{ 
    if( !m_reactiveList.contains(el) ) m_reactiveList.append(el); 
}

void Simulator::remFromReactive( eElement* el ) 
{ 
    m_reactiveList.removeOne(el); 
}*/

void Simulator::addToChangedFast( eElement* el )   
{ 
    if( !m_changedFast.contains(el) ) m_changedFast.append(el); 
}

void Simulator::remFromChangedFast( eElement* el ) 
{ 
    m_changedFast.removeOne(el); 
}

void Simulator::addToChangedSlow( eElement* el )   
{ 
    if( !m_changedSlow.contains(el) ) m_changedSlow.append(el); 
}

void Simulator::remFromChangedSlow( eElement* el ) 
{ 
    m_changedSlow.removeOne(el); 
}

void Simulator::addToNoLinList( eElement* el )
{
    if( !m_nonLinear.contains(el) ) m_nonLinear.append(el);
}

void Simulator::remFromNoLinList( eElement* el )
{
    m_nonLinear.removeOne(el);
}

/*void Simulator::addToMcuList( eElement* el )
{
    if( !m_mcuStepList.contains(el) ) m_mcuStepList.append( el );
}
void Simulator::remFromMcuList( eElement* el ) { m_mcuStepList.removeOne( el ); }*/

#include "moc_simulator.cpp"
