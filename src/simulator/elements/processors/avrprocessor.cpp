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

#include "avrprocessor.h"
#include "simulator.h"
#include "utils.h"

// simavr includes
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_core.h"
#include "avr_uart.h"

//AvrProcessor* AvrProcessor::m_pSelf = 0l;

extern "C"
int elf_read_firmware_ext(const char * file, elf_firmware_t * firmware);

AvrProcessor::AvrProcessor( QObject* parent ) : BaseProcessor( parent )
{
    m_pSelf = this;
    m_avrProcessor = 0l;
    setSteps( 16 );
}
AvrProcessor::~AvrProcessor() {}

void AvrProcessor::terminate()
{
    BaseProcessor::terminate();
    if( m_avrProcessor ) avr_terminate( m_avrProcessor );
    m_avrProcessor = 0l;
}

bool AvrProcessor::loadFirmware( QString fileN )
{
    if ( fileN == "" ) return false;
    QFileInfo fileInfo(fileN);

    //fileN.replace( fileN.split(".").last(), "hex" );
    if (fileInfo.completeSuffix().isEmpty())
        fileN.append(".hex");

    if( !QFile::exists(fileN) )     // File not found
    {
        QMessageBox::warning( 0, tr("File Not Found"),
        tr("The file \"%1\" was not found.").arg(fileN) );
        return false;
    }

    char name[20] = "";
    strncpy( name, m_device.toLatin1(), sizeof(name)-1 );
    *(name + sizeof(name) -1) = 0;

    char filename[1000]="";
    strncpy( filename, fileN.toLatin1(), sizeof(filename)-1 );
    *(filename + sizeof(filename) -1) = 0;

    elf_firmware_t f = {{0}};

    if( fileN.endsWith("hex") )
    {
        ihex_chunk_p chunk = NULL;
        int cnt = read_ihex_chunks(filename, &chunk);

        if( cnt <= 0 )
        {
            QMessageBox::warning(0,"Error:",
            tr(" Unable to load IHEX file %1\n").arg(fileN) );
            return false;
        }

        int lastFChunk = 0;

        for( int ci = 0; ci < cnt; ci++ )
        {
            if( chunk[ci].baseaddr < (1*1024*1024) ) lastFChunk = ci;
        }
        f.flashbase = chunk[ 0 ].baseaddr;
        f.flashsize = chunk[ lastFChunk ].baseaddr + chunk[ lastFChunk ].size;
        f.flash = (uint8_t*) malloc( f.flashsize+1 );

        for( int ci = 0; ci < cnt; ci++ )
        {
            if( chunk[ci].baseaddr < (1*1024*1024) )
            {
                memcpy( f.flash + chunk[ci].baseaddr,
                        chunk[ci].data,
                        chunk[ci].size );
            }
            if( chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM )
            {
                f.eeprom = chunk[ci].data;
                f.eesize = chunk[ci].size;
            }
        }
    }
    else if( fileN.endsWith(".elf") )
    {
        f.flashsize = 0;
        elf_read_firmware_ext(filename, &f);
        if (!f.flashsize)
        {
            QMessageBox::warning(0,tr("Failed to load firmware: "),
                                 tr("File %1 is not in valid ELF format\n").arg(fileN) );
            return false;
        }
    }
    else                                    // File extension not valid
    {
        QMessageBox::warning(0,tr("Error:"), tr("%1 should be .hex or .elf\n").arg(fileN) );
        return false;
    }

    QString mmcu(f.mmcu);
    if (!mmcu.isEmpty())
    {
        if (mmcu != m_device) {
            QMessageBox::warning(0,tr("Warning on load firmware: "),
                                 tr("Incompatible firmware: compiled for %1 and your processor is %2\n").arg(mmcu).arg(m_device) );
            //return false;
        }
    } else {
        if( !strlen(name) ) {
            QMessageBox::warning(0,tr("Failed to load firmware: "),
                                 tr("The processor model is not specified.\n") );
            return false;
        }
        strcpy( f.mmcu, name );
    }
    if( !m_avrProcessor )
    {
        m_avrProcessor = avr_make_mcu_by_name(f.mmcu);

        if( !m_avrProcessor )
        {
            QMessageBox::warning(0,"Error:",
            tr("Simulation of this AVR processor model is not supported: %1\n").arg(f.mmcu) );
            return false;
        }
        int started = avr_init( m_avrProcessor );

        // Usart interface
            // Irq to send data to terminal panel
        avr_irq_t* src = avr_io_getirq(m_avrProcessor, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);
        avr_irq_register_notify(src, uart_pty_out_hook, this);

            // Irq to send data to AVR:
        m_uartInIrq = avr_io_getirq(m_avrProcessor, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT);

        qDebug() << "\nAvrProcessor::loadFirmware Avr Init: "<< name << (started==0);
    }
    ///TODO: Catch possible abort signal here, otherwise application will crash on the invalid firmware load
    avr_load_firmware( m_avrProcessor, &f );

    if( f.flashbase ) m_avrProcessor->pc = f.flashbase;

    m_avrProcessor->frequency = 16000000;
    m_avrProcessor->cycle = 0;
    m_symbolFile = fileN;

    initialized();

    return true;
}

void AvrProcessor::reset()
{
    if( !m_loadStatus ) return;
    //qDebug() << "AvrProcessor::reset";

    avr_reset( m_avrProcessor );
    m_avrProcessor->pc = 0;
}

void AvrProcessor::step()
{
    if( !m_loadStatus ) return;
    while( m_avrProcessor->cycle < m_nextCycle )
    {
        m_avrProcessor->run(m_avrProcessor);
    }
    m_nextCycle += m_mcuStepsPT;

    //qDebug() << m_avrProcessor->cycle;
}

void AvrProcessor::stepOne()
{
    m_avrProcessor->run(m_avrProcessor);
}

int AvrProcessor::pc()
{
    return m_avrProcessor->pc;
}


int AvrProcessor::getRamValue( int address )
{
    return m_avrProcessor->data[address];
}

int AvrProcessor::validate( int address )
{
    if( address < 64 ) address += 32;
    return address;
}

void AvrProcessor::uartIn( uint32_t value ) // Receive one byte on Uart
{
    //qDebug() << "AvrProcessor::uartIn: " << value;
    BaseProcessor::uartIn( value );

    if( m_avrProcessor ) avr_raise_irq( m_uartInIrq, value );
}

#include "moc_avrprocessor.cpp"

