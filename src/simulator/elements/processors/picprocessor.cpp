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

#include <QtGui>

#include "picprocessor.h"
#include "mcucomponent.h"
#include "simulator.h"
#include "utils.h"

// GpSim includes
#include "pic-processor.h"
#include "uart.h"
#include "pir.h"


PicProcessor::PicProcessor( QObject* parent ) 
            : BaseProcessor( parent )
            , m_hexLoader()
{
    m_pSelf = this;
    m_pPicProcessor = 0l;
    m_loadStatus    = false;
}
PicProcessor::~PicProcessor(){}

void PicProcessor::terminate()
{
    BaseProcessor::terminate();
 
    m_pPicProcessor = 0l;
}

bool PicProcessor::loadFirmware( QString fileN )
{
    if( fileN == "" ) return false;
    
    m_symbolFile = fileN; 
    
    if( !QFile::exists( m_symbolFile ) )
    {
        QMessageBox::warning( 0, tr("File Not Found")
                               , tr("The file \"%1\" was not found.").arg(m_symbolFile) );
        return false;
    }
    QByteArray symbolFile = m_symbolFile.toLocal8Bit();
    QByteArray device     = m_device.toLocal8Bit();
    
    m_loadStatus = false;
    
    if( !m_pPicProcessor )
    {
        qDebug() << "Creating Proccessor:    "<<m_device<<"\n" ;

        Processor* p = ProcessorConstructor::CreatePic( device.constData() );
        m_pPicProcessor = dynamic_cast<pic_processor*>( p );

        if( !m_pPicProcessor )
        {
            QMessageBox::warning( 0, tr("Unkown Error:")
                                   , tr("Could not Create Pic Processor: \"%1\"").arg(m_device) );
            return false;
        }
    }
    /*Cycle_Counter_breakpoint_list* l1 = &(get_cycles().active);

    while(l1->next)            // Clear CycleCounter breakpoint list
    {
        //qDebug() << "clear_break"<<l1->break_value;
        l1->clear();
        Cycle_Counter_breakpoint_list* l2 = l1;
        l1 = l1->next;
        l2->next = 0l;
    }*/
    qDebug() << "Loading HexFile:\n"<<m_symbolFile<<"\n" ;
    
    FILE* pFile  = fopen( symbolFile.constData(), "r" );
    int load = m_hexLoader.readihex16( m_pPicProcessor, pFile );
    if( load == IntelHexProgramFileType::SUCCESS ) m_loadStatus = true;
    
    if( !m_loadStatus )
    {
        QMessageBox::warning( 0, tr("Unkown Error:")
                               , tr("Could not Load: \"%1\"").arg(m_symbolFile) );
        return false;
    }
    //m_pPicProcessor->set_frequency( (double)McuComponent::self()->freq()*1e6 );
    qDebug() <<"\nProcessor Ready:\n    Device    ="<<m_pPicProcessor->name().c_str();
    qDebug() << "    Freq. MHz =" <<  McuComponent::self()->freq();
    qDebug() << "    Int. OSC  =" << (m_pPicProcessor->get_int_osc() ? "true" : "false");
    qDebug() << "    Use PLLx4 =" << (m_pPicProcessor->get_pplx4_osc() ? "true" : "false");

    //QString tmr1Freq = m_device + ".tmr1_freq";
    //device = tmr1Freq.toLocal8Bit();
    //gpsimObject* tmr1 = globalSymbolTable().find( device.constData() );
    //Float* tmr1FreqVal = dynamic_cast<Float*>( tmr1 );
    //if( tmr1FreqVal )  qDebug() << "    Tmr1_freq =" << tmr1FreqVal->getVal();
    
    int cpi = m_pPicProcessor->get_ClockCycles_per_Instruction();
    m_ipc = 1/(double)cpi;
    qDebug() << "    Cycs/Inst =" << cpi;

    initialized();
    
    m_pendingIpc = 0;
    m_nextCycle  = m_mcuStepsPT/cpi;
    if( m_nextCycle == 0 ) m_nextCycle = 1;
    
    int address = getRegAddress( "RCSTA" );
    Register* rcsta = m_pPicProcessor->rma.get_register( address );
    m_rcsta = dynamic_cast<_RCSTA*>(rcsta); 

    return true;
}

void PicProcessor::step()                 // Run 1 step 
{
    if( !m_loadStatus || m_resetStatus || m_mcuStepsPT==0 ) return;
    
    double dCycles = (double)m_mcuStepsPT*m_ipc + m_pendingIpc;
    
    int cycles = (int)dCycles;
    
    m_pendingIpc = dCycles-(double)cycles;

    for( int k=0; k<cycles; k++ ) 
    {
        m_pPicProcessor->step_cycle();
    }
}

void PicProcessor::stepOne() 
{
    if( m_nextCycle > 0 )
    {
        m_pPicProcessor->step_cycle();
        m_nextCycle--;
    }
    if( m_nextCycle == 0 )
    {
        double stepsPT = (double)McuComponent::self()->freq();
        double dCycles = stepsPT*m_ipc + m_pendingIpc;
        int cycles = (int)dCycles;
        m_pendingIpc = dCycles-(double)cycles;
        m_nextCycle = cycles;
        
        runSimuStep(); // 1 simu step = 1uS
    }
}

void PicProcessor::stepCpu()
{
    m_pPicProcessor->step_cycle();
}

int PicProcessor::pc() { return m_pPicProcessor->pc->get_value(); }

void PicProcessor::reset()
{
    if( !m_loadStatus ) return;

    m_pPicProcessor->reset( POR_RESET ); // POR_RESET  MCLR_RESET EXIT_RESET
}

int PicProcessor::getRamValue( int address )
{ 
    return m_pPicProcessor->rma[address].get_value();
}

int PicProcessor::validate( int address ) { return address; }

void PicProcessor::uartIn( uint32_t value ) // Receive one byte on Uart
{
    if( m_pPicProcessor)
    {
        BaseProcessor::uartIn( value );

        m_rcsta->queueData( value );
        //qDebug() << "PicProcessor::uartIn: " << value<<m_pPicProcessor->rma[26].get_value();
    }
}

#include "moc_picprocessor.cpp"

