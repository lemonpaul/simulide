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
#ifndef NO_PIC

#include <QtGui>

#include "picprocessor.h"
#include "simulator.h"
#include "utils.h"

// GpSim includes
#include "gpsim/cod.h"
#include "gpsim/interface.h"
#include "gpsim/gpsim_classes.h"
#include "gpsim/pic-processor.h"

PicProcessor::PicProcessor( QObject* parent ) : BaseProcessor( parent )
{
    m_pSelf = this;
    m_pPicProcessor = 0l;
    m_loadStatus    = false;
    m_DoneGpsimInit = false;
}
PicProcessor::~PicProcessor(){}

void PicProcessor::terminate()
{
    //qDebug() << "PicProcessor::terminate";
    BaseProcessor::terminate();
    m_pPicProcessor = 0l;
}

bool PicProcessor::loadFirmware( QString fileN )
{
    if( fileN == "" ) return false;
    
    m_symbolFile = fileN;
    
    //qDebug() << m_device;

    if( !QFile::exists(m_symbolFile) )
    {
        QMessageBox::warning( 0, tr("File Not Found"), tr("The file \"%1\" was not found.").arg(m_symbolFile) );
        return false;
    }
    
    if( !m_DoneGpsimInit ) 
    {
        qDebug() << "\n        Initializing GpSim...\n";
        int init = initialize_gpsim_core();
        initialization_is_complete();
        m_DoneGpsimInit = (init==0);
        
        if( m_DoneGpsimInit ) 
            qDebug() << "\n    ...GpSim Initialization is complete\n\n";
        else
            qDebug() << "\n    ...Error!!! GpSim Initialization Failed!!!\n\n";
            
        // Load  Dummy .cod File to get the processor
        QString codFile = QCoreApplication::applicationDirPath()+"/data/pic/"+m_device+".cod";
        //qDebug() << "PicProcessor::loadFirmware codFile"<<codFile ;
        
        QByteArray ba = codFile.toLatin1();
        const char *fileName = ba.data();
        FILE       *pFile    = fopen( fileName, "r" );
        Processor  *tempProc = 0l;
        m_loadStatus = ( ProgramFileTypeList::GetList().LoadProgramFile( &tempProc, fileName, pFile ) );
        m_pPicProcessor = dynamic_cast<pic_processor*>(tempProc);
    }
    // Load the actual .hex File
    if( m_pPicProcessor )
    {
        m_symbolFile = fileN;//.replace( fileN.split(".").last(), "hex" );
        qDebug() << "PicProcessor::loadFirmware hexFile"<<m_symbolFile ;
        QByteArray ba = m_symbolFile.toLatin1();
        const char *fileName = ba.data();
        FILE       *pFile    = fopen( fileName, "r" );
        ba = m_device.toLatin1();
        const char *procName = ba.data();
        m_loadStatus = m_pPicProcessor->LoadProgramFile( fileName, pFile, procName );
    }
    else
    {
        QMessageBox::warning( 0, tr("Unkown Error:"),  tr("Could not Create Pic Processor for: \"%1\"").arg(m_symbolFile) );
        return false;
    }

    if( !m_loadStatus )
    {
        QMessageBox::warning( 0, tr("Unkown Error:"),  tr("Could not Load: \"%1\"").arg(m_symbolFile) );
        return false;
    }
    QString name = m_pPicProcessor->name().c_str();
    qDebug() << "PicProcessor::loadFirmware      Device: " << name;

    //if ( device.split("-").first().toUpper() == m_device )
    {
        //if( needAtach ) attachPins();
        //m_ready = m_loadStatus && m_haveMclr && m_haveVdd;
        //reset();
    }
    m_lastTrmtBit = 0; // for usart
    
    initialized();
    
    return true;
    /*else
    {
        QMessageBox::warning( 0, tr("Device Type Error"),
                                 tr("ERROR: The cod file \"%1\"\nIs for %2.\nCannot load to %3.")
                                 .arg(m_symbolFile).arg(device).arg(m_device) );
    }*/
    //qDebug() << "mem: " << m_pPicProcessor->rma[329].get_value(); ******** get RAM value *********
    //m_pPicProcessor->list(2, 0, 0, 100);
    //qDebug() << m_pPicProcessor->pma->get_src_line(5);
    //m_pPicProcessor->pma->get_PC();
    //m_pPicProcessor->pma->address_has_break(address);
    //find_address_from_line(FileContext *fc, int src_line)
    //m_pPicProcessor->pma->set_break_at_line(0,40);
    //m_pPicProcessor->run(false);
    //int cpupc = m_pPicProcessor->pc->get_value();
    //m_pPicProcessor->pma->find_closest_address_to_line(0,line);
    //m_pPicProcessor->pma->find_address_from_line(0, line);
    //qDebug() << "PC: " << cpupc << "line:" << m_pPicProcessor->pma->get_src_line(cpupc);
}

void PicProcessor::step()                 // Run 1 step 
{
    if( !m_loadStatus ) return;
    for( int k=0; k<m_mcuStepsPT; k++ )
    {
        if( m_usartTerm ) readUsart();
        m_pPicProcessor->step_cycle();
    }
}

void PicProcessor::stepOne()
{ 
    m_pPicProcessor->step_cycle();
}

int PicProcessor::pc()
{
    return m_pPicProcessor->pc->get_value();
}

void PicProcessor::reset()
{
    if( !m_loadStatus ) return;
    m_pPicProcessor->reset( POR_RESET ); // POR_RESET  MCLR_RESET EXIT_RESET
    //BaseProcessor::reset();
}

int PicProcessor::getRamValue( int address )
{
    return m_pPicProcessor->rma[address].get_value();
}

int PicProcessor::validate( int address )
{
    return address;
}

void PicProcessor::uartIn( uint32_t value ) // Receive one byte on Uart
{
    //qDebug() << "AvrProcessor::uartIn: " << value;
    //BaseProcessor::uartIn( value );
    if( m_usartTerm )
    {
        QString text = " PIC Uart Input NOT Yet Implemented: \n";
        TerminalWidget::self()->uartIn( value );
    }

}

void PicProcessor::readUsart()
{
static int txreg;
static int txreg0;

    int txstaReg = BaseProcessor::getRamValue( "txsta" );
    if( !(txstaReg & 32) ) return;                           // TXEN bit

    bool trmtBit = txstaReg & 2;
    
    txreg = BaseProcessor::getRamValue( "txreg" );

    if( txreg != txreg0  )
    {
        if( !m_lastTrmtBit & !trmtBit ) BaseProcessor::uartOut( txreg );
        txreg0 = txreg;
        //qDebug() << "PicProcessor::readUsart:.... " << text;
    }
    if( m_lastTrmtBit != trmtBit ) 
    {
        if(  m_lastTrmtBit & !trmtBit ) BaseProcessor::uartOut( txreg );
        m_lastTrmtBit = trmtBit;
        //qDebug() << "PicProcessor::readUsart: " << text;
    }
}
#include "moc_picprocessor.cpp"
#endif


