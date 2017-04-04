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

#include "serialportwidget.h"
#include "avrprocessor.h"
#include "simulator.h"
#include "utils.h"

// simavr includes
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_core.h"
#include "avr_uart.h"

//AvrProcessor* AvrProcessor::m_pSelf = 0l;

AvrProcessor::AvrProcessor( QObject* parent ) : BaseProcessor( parent )
{
    m_pSelf = this;
    m_avrProcessor = 0l;
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

    //if( m_avrProcessor ) terminate();

    fileN.replace( fileN.split(".").last(), "hex" );

    if( !QFile::exists(fileN) )     // File not found
    {
        QMessageBox::warning( 0, tr("File Not Found"),
        tr("The file \"%1\" was not found.").arg(fileN) );

        return false;
    }

    uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
    char name[16] = "";
    elf_firmware_t f = {{0}};

    //f.frequency = 16000000;

    strcpy( name, m_device.toLatin1() );

    char filename[200]="";
    strcpy( filename, fileN.toLatin1() );

    if( fileN.endsWith("hex") )
    {
        //qDebug()<<"!";

        ihex_chunk_p chunk = NULL;
        int cnt = read_ihex_chunks(filename, &chunk);

        if (cnt <= 0)
        {
            QMessageBox::warning(0,"Error:",
            tr(" Unable to load IHEX file %1\n").arg(fileN) );
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
    else                                    // File extension not valid
    {
        QMessageBox::warning(0,"Error:", tr("%1 should be .hex \n").arg(fileN) );
        return false;
    }

    if( strlen(name) ) strcpy( f.mmcu, name );
    if( !m_avrProcessor )
    {
        m_avrProcessor = avr_make_mcu_by_name(f.mmcu);

        if( !m_avrProcessor )
        {
            QMessageBox::warning(0,"Error:",
            tr("The firmware is not for %1 \n").arg(f.mmcu) );
            return false;
        }
        int started = avr_init( m_avrProcessor );
        qDebug() << "\nAvrProcessor::loadFirmware Avr Init: "<< name << (started==0);
    }

    avr_load_firmware( m_avrProcessor, &f );

    if( f.flashbase ) m_avrProcessor->pc = f.flashbase;

    m_avrProcessor->frequency = 16000000;
    m_symbolFile = fileN;

    // Usart interface
    // Irq to send data to terminal panel
    avr_irq_t* src = avr_io_getirq(m_avrProcessor, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);
    avr_irq_register_notify(src, uart_pty_out_hook, this);

    // Irq to send data to AVR:
    m_uartInIrq = avr_io_getirq(m_avrProcessor, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT);


    initialized();

    return true;
}

void AvrProcessor::reset()
{
    if( !m_loadStatus ) return;
    //avr_init( m_avrProcessor );
    avr_reset( m_avrProcessor );
    //BaseProcessor::reset();
    //m_avrProcessor->init( m_avrProcessor );
    m_avrProcessor->pc = 0;
}

void AvrProcessor::step()
{
    if( !m_loadStatus ) return;
    //avr_run( m_avrProcessor );
    for( int k=0; k<m_mcuStepsPT; k++ )m_avrProcessor->run(m_avrProcessor);
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

