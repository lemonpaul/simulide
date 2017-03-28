/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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
 ***************************************************************************/

#include "avrdebugger.h"
#include "utils.h"

// simavr includes
#include "simavr/simavr/sim/sim_elf.h"
#include "simavr/simavr/sim/sim_hex.h"
#include "simavr/simavr/sim/sim_core.h"

AvrDebugger::AvrDebugger() : BaseProcessor()
{
    m_avrProcessor  = 0l;
    m_loadStatus    = false;
}
AvrDebugger::~AvrDebugger()
{
    if( m_avrProcessor ) avr_terminate( m_avrProcessor );
}

bool AvrDebugger::loadFirmware( QString fileN )
{
    if ( fileN == "" )  return false;
    if( m_loadStatus )
    {
        QMessageBox::warning( 0, tr("AvrDebugger::loadFirmware"), 
                                 tr("Debugger already running") );
        return false;
    }
    m_symbolFile = fileN;
    getProcName();

    uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
    char name[16] = "";
    elf_firmware_t f= {{0}};
    strcpy( name, m_device.toLatin1() );

    if( !QFile::exists(fileN) )     // File not found
    {
        QMessageBox::warning( 0, tr("File Not Found"), tr("The file \"%1\" was not found.").arg(fileN) );
        return false;
    }
    char filename[200]="";
    strcpy( filename, fileN.toLatin1() );

    if( fileN.endsWith("hex") )
    {
        ihex_chunk_p chunk = NULL;
        int cnt = read_ihex_chunks(filename, &chunk);
        
        if (cnt <= 0) 
        {
            QMessageBox::warning(0,"Error:", tr(" Unable to load IHEX file %1\n").arg(fileN) );
            return false;
        }
        for (int ci = 0; ci < cnt; ci++) 
        {
            if (chunk[ci].baseaddr < (1*1024*1024)) 
            {
                f.flash = chunk[ci].data;
                f.flashsize = chunk[ci].size;
                f.flashbase = chunk[ci].baseaddr;
            } 
            else if (chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM ||
                    chunk[ci].baseaddr + loadBase >= AVR_SEGMENT_OFFSET_EEPROM) 
            {
                f.eeprom = chunk[ci].data;
                f.eesize = chunk[ci].size;
            }
        }
    }
    //else if ( fileN.endsWith("axf") ) elf_read_firmware(filename, &f);
    else // File extension not valid
    {
        QMessageBox::warning(0,"Error:", tr("%1 should be .hex or .axf\n").arg(fileN) );
        return false;
    }
    if( strlen(name) ) strcpy( f.mmcu, name );
    m_avrProcessor = avr_make_mcu_by_name(f.mmcu);

    if( !m_avrProcessor )
    {
        QMessageBox::warning(0,"Error:", tr("The firmware is not for %1 \n").arg(f.mmcu) );
        return false;
    }
    int started = avr_init( m_avrProcessor );
    avr_load_firmware( m_avrProcessor, &f );
    if ( f.flashbase ) m_avrProcessor->pc = f.flashbase;//printf( "Attempted to load a bootloader at %04x\n", f.flashbase );

    avr_reset( m_avrProcessor );
    m_loadStatus = true;

    mapLstToAsm();
    setRegisters();

    return true;
}

void AvrDebugger::getProcName()
{
    QStringList lines = fileToStringList( m_symbolFile.replace(".hex", ".asm"), "AvrDebugger::getProcName" );

    foreach( QString line, lines )
    {
        line = line.toLower();
        if( line.contains(".include") )  // Find info in inclded header
        {
            m_device = line.remove(".include").remove(" ").remove("\"").remove("def.inc")
                           .replace("t", "attiny").replace("m", "atmega");
            qDebug() << "AvrDebugger::getProcName Fonud device name: " << m_device;
            break;
        }
    }
}

int AvrDebugger::step()
{
    avr_run( m_avrProcessor );
    return getSourceLine();
}

int AvrDebugger::stepOver(){return 0;}

void AvrDebugger::reset()
{ if( m_loadStatus ) avr_reset( m_avrProcessor );}

void AvrDebugger::terminate()
{ if ( m_avrProcessor ) avr_terminate( m_avrProcessor );}

int AvrDebugger::getValidLine( int line )
{
    int asmLine = 0;
    while( asmLine == 0 )
    {
        asmLine = m_asmToFlash.value(line);
        line++;
    }
    return m_flashToAsm[asmLine];
}

int AvrDebugger::getSourceLine()
{
    int asmLine = m_flashToAsm[m_avrProcessor->pc];
    return asmLine;
}

int AvrDebugger::getRamValue( QString name )
{ return m_avrProcessor->data[getRegAddress(name)]; }

int AvrDebugger::getRegAddress( QString name )
{
    if( regsTable.isEmpty() ) return -1;
    bool isNumber = false;
    int address = name.toInt( &isNumber );      // Try to convert to integer
    if( !isNumber ) address = regsTable[name];  // Is a register name
    return address;
}

QStringList AvrDebugger::getDefsList()
{ return QStringList( regsTable.uniqueKeys() ); }

void AvrDebugger::mapLstToAsm()
{
    m_flashToAsm.clear();
    m_asmToFlash.clear();
    QString asmFileName = m_symbolFile;
    asmFileName.replace( asmFileName.split(".").last(), "asm" );

    QString lstFileName = m_symbolFile;
    lstFileName.replace( lstFileName.split(".").last(), "lst");

    QStringList asmLines = fileToStringList( asmFileName, "AvrDebugger::mapLstToAsm" );
    QStringList lstLines = fileToStringList( lstFileName, "AvrDebugger::mapLstToAsm" );

    QString line;
    QString asmLine;
    int asmLineNumber = 0;
    int lastAsmLine = asmLines.size();

    foreach( asmLine, asmLines ) // Go to the program start in asm file
    {
        if( asmLine.contains( "BASPROGRAMSTART:" )) break;
        asmLineNumber++;
    }
    bool started = false;

    foreach( line, lstLines )
    {
        if( line == "         " ) continue; //avra
        if( line.isEmpty() )      continue;
        line = line.toUpper();
        if( started )
        {
            line.replace("\t", " ").remove("C:");
            
            while( true )
            {
                if( ++asmLineNumber >= lastAsmLine ) break; // End of asm file
                asmLine = asmLines.at( asmLineNumber ).toUpper();
                asmLine = asmLine.replace("\t", " ").toUpper();
                if( asmLine.isEmpty() ) continue;
                else if( asmLine.endsWith(":")) asmLine = asmLine.remove(":");
                if( line.contains(asmLine) ) break;
            }
            QString numberText = line.split(" ").first(); // first exp in lst file is address
            bool ok = false;
            int address = numberText.toInt( &ok, 16 )*2;  // original adress*2: instruc = 2 bytes
            if( ok )
            {
                m_flashToAsm[address] = asmLineNumber;
                m_asmToFlash[asmLineNumber] = address;
            }
            continue;
        }
        if( line.contains("		BASPROGRAMSTART")
         || line.contains("BASPROGRAMSTART:") )
            started = true;
    }
}

void AvrDebugger::setRegisters()  // get register addresses from lst file
{
    QString lstFileName = m_symbolFile;
    lstFileName.replace( lstFileName.split(".").last(), "lst");
    QStringList lineList = fileToStringList( lstFileName, "AvrDebugger::setRegisters" );

    if( !regsTable.isEmpty() ) regsTable.clear();

    foreach( QString line, lineList )
    {
        line = line.toLower().replace("\t"," ").replace("="," ");
        if( line.contains("equ ") || line.contains("def "))      // This line contains a definition
        {
            QString name    = "";
            QString addrtxt = "";
            int address   = 0;
            bool isNumber = false;

            line.remove("equ").remove(".def").remove(".");
            QStringList wordList = line.split(QRegExp("\\s+")); // Split in words
            while( name.isEmpty() ) name = wordList.takeFirst();
            while( addrtxt.isEmpty() ) addrtxt = wordList.takeFirst();

            if( addrtxt.startsWith("H") )                  // IS hexadecimal??
            {
                addrtxt.remove("H").remove("'");           // Get the digits
                address = addrtxt.toInt( &isNumber, 16 );  // Base 16
            }
            else if( addrtxt.startsWith("0x") )            // IS hexadecimal??
            {
                addrtxt.remove(0, 2);                      // Get the digits
                address = addrtxt.toInt( &isNumber, 16 );  // Base 16
            }
            else
            {
                if( addrtxt.startsWith("r"))
                {
                    addrtxt.remove("r");
                    address = addrtxt.toInt( &isNumber );
                }
                else
                {
                    address = addrtxt.toInt( &isNumber );
                    if( isNumber && address < 64 ) address += 32;
                }
            }
            if( isNumber ) regsTable.insert(name, address); // If found a valid address add to map
        }
    }
}

#include "moc_avrprocessor.cpp"
