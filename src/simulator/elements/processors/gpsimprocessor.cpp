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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/
#ifndef NO_PIC
#include "gpsimprocessor.h"
#include "utils.h"

// GpSim includes
#include "gpsim/cod.h"
#include "gpsim/interface.h"
#include "gpsim/gpsim_classes.h"
#include "gpsim/pic-processor.h"

GpsimProcessor* GpsimProcessor::m_pSelf = 0l;

GpsimProcessor::GpsimProcessor() : BaseProcessor()
{
    m_pSelf = this;
    m_pPicProcessor = 0l;
    m_loadStatus    = false;

    if (!m_DoneGpsimInit)
    {
        qDebug() << "\n        Initializing GpSim...\n";
        int init = initialize_gpsim_core();
        initialization_is_complete();
        m_DoneGpsimInit = (init==0);
        if( m_DoneGpsimInit ) qDebug() << "\n    ...GpSim Initialization is complete\n\n";
        else
        {
            qDebug() << "\n    ...Error!!! GpSim Initialization Failed!!!\n\n";
            //return false;
        }
    }
    //exec();
}
GpsimProcessor::~GpsimProcessor(){ terminate(); }

void GpsimProcessor::terminate()
{
    BaseProcessor::terminate();
    if( m_pPicProcessor ) delete m_pPicProcessor; /*m_pPicProcessor->reset(EXIT_RESET);*/
    m_pPicProcessor = 0l;
}

/*void GpsimProcessor::run()
{
    while( m_running );
    qDebug() << "GpsimProcessor::run -> exit";
    if( m_pPicProcessor ) m_pPicProcessor->reset(EXIT_RESET);
}*/

bool GpsimProcessor::loadFirmware( QString fileN )
{
    if( fileN == "" ) return false;
    
    m_symbolFile = fileN.replace( fileN.split(".").last(), "cod" );

    QString lstFile = m_symbolFile;

    if( !QFile::exists(lstFile.replace(lstFile.split(".").last(), "lst") ) )
    {
        QMessageBox::warning( 0, tr("File Not Found"), tr("The lst file \"%1\" was not found.").arg(lstFile) );
        return false;
    }
    if( !QFile::exists(m_symbolFile) )
    {
        QMessageBox::warning( 0, tr("File Not Found"), tr("The cod file \"%1\" was not found.").arg(m_symbolFile) );
        return false;
    }

    const char *fileName = m_symbolFile.toAscii();
    FILE       *pFile    = fopen( fileName, "r" );
    Processor  *tempProc = 0l;

    m_loadStatus = ( ProgramFileTypeList::GetList().LoadProgramFile( &tempProc, fileName, pFile ) );

    if( !m_loadStatus )
    {
        QMessageBox::warning( 0, tr("Unkown Error:"),  tr("Could not Load: \"%1\"").arg(m_symbolFile) );
        return false;
    }
    m_pPicProcessor = dynamic_cast<pic_processor*>(tempProc);
    //m_lastDir = m_symbolFile;
    //m_lastDir = m_lastDir.replace(m_symbolFile.split("/").last(), "");

    qDebug() << "\nLoaded: " << m_symbolFile.toAscii(); //<<m_pPicProcessor->program_memory_size()
    //QString device = m_pPicProcessor->name().c_str();

    //if ( device.split("-").first().toUpper() == m_device )
    {
        //if( needAtach ) attachPins();
        //m_ready = m_loadStatus && m_haveMclr && m_haveVdd;
        reset();
    }
    setRegisters();
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

void GpsimProcessor::step()                 // Run 1 step and returns actual source line number
{
    //if( !m_loadStatus ) return 0;       // No program loaded
    m_pPicProcessor->step_cycle();
}

void GpsimProcessor::reset()
{
    if( !m_loadStatus ) return;
    m_pPicProcessor->reset(SIM_RESET); // POR_RESET  MCLR_RESET EXIT_RESET
    BaseProcessor::reset();
}

int GpsimProcessor::getRamValue( QString name ) // Returns the value stored in Ram address
{
    return m_pPicProcessor->rma[ getRegAddress( name )].get_value();
}

QHash<QString, int>  GpsimProcessor::getRegsTable( QString lstFileName )// get register addresses from lst file
{
    //QString lstFileName = m_symbolFile;
    lstFileName.replace( lstFileName.split(".").last(), "lst");
    QStringList lineList = fileToStringList( lstFileName, "GpsimProcessor::setRegisters" );

    if( !m_regsTable.isEmpty() ) m_regsTable.clear();

    foreach( QString line, lineList )
    {
        if( line.contains("EQU") )      // This line contains a definition
        {
            QStringList wordList = line.split(QRegExp("\\s+")); // Split in words
            QString temp;
            QString name    = "";
            QString addrtxt = "";
            int address   = 0;
            bool isNumber = false;

            foreach( QString word , wordList )
            {
                if( word == "EQU" ) name = temp.toLower(); // When find "EQU", name is the prev word
                if( temp == "EQU" ) addrtxt = word;        // if prev is "EQU", address is this word
                temp = word;
            }
            if( addrtxt.startsWith("H") )                  // IS hexadecimal??
            {
                addrtxt.remove("H").remove("'");           // Get the digits
                address = addrtxt.toInt( &isNumber, 16 );  // Base 16
            }
            else address = addrtxt.toInt( &isNumber );

            if( isNumber ) m_regsTable.insert(name, address); // If found a valid address add to map
            //qDebug() << name << address;
        }
    }
    return m_regsTable;
}

#include "moc_gpsimprocessor.cpp"
#endif