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

#include "baseprocessor.h"
#include "mainwindow.h"
#include "simulator.h"
#include "utils.h"

BaseProcessor* BaseProcessor::m_pSelf = 0l;

BaseProcessor::BaseProcessor( QObject* parent )
     : QObject( parent )
{
    m_loadStatus = false;
    m_usartTerm  = false;
    m_serialPort = false;
    m_ramTable   = 0l;
    m_symbolFile = "";
    m_device     = "";
}
BaseProcessor::~BaseProcessor() 
{
}

void BaseProcessor::terminate()
{
    //qDebug() <<"\nBaseProcessor::terminate "<<m_device<<m_symbolFile<<"\n";

    if( m_ramTable )
    {
        MainWindow::self()->m_ramTabWidgetLayout->removeWidget( m_ramTable );
        //delete m_ramTable;
        m_ramTable   = 0l;
    }
    m_pSelf = 0l;
    m_loadStatus = false;
    m_symbolFile = "";
    //m_device     = "";
}

void BaseProcessor::initialized()
{
    //qDebug() << "\nBaseProcessor::initialized  Firmware: " << m_symbolFile;
    //qDebug() << "\nBaseProcessor::initialized Data File: " << m_dataFile;

    setRegisters();
    m_loadStatus = true;

    if( m_ramTable == 0l )
    {
        m_ramTable = new RamTable( this );
        MainWindow::self()->m_ramTabWidgetLayout->addWidget( m_ramTable );
        //qDebug() << "RmTable:" << m_ramTable;
    }
}

void BaseProcessor::setSteps( int steps ){ m_mcuStepsPT = steps; }

QString BaseProcessor::getFileName() { return m_symbolFile; }

void BaseProcessor::setDevice( QString device ) { m_device = device;}

QString BaseProcessor::getDevice() { return m_device;}

void BaseProcessor::setDataFile( QString datafile ) 
{ 
    m_dataFile = QCoreApplication::applicationDirPath()+"/data/"+datafile+".data";
}

/*void BaseProcessor::reset()
{
    //qDebug()<<"BaseProcessor::reset"<<m_device<<"m_loadStatus"<<m_loadStatus;
    //emit chipReset();
}*/

int BaseProcessor::getRegAddress( QString name ) { return m_regsTable.value( name ); }

void BaseProcessor::updateRamValue( QString name )
{
    QString type = "";
    if( m_typeTable.contains( name )) type = m_typeTable[ name ];
    else return;
    
    QByteArray ba;
    ba.resize(4);
    int address = getRegAddress( name );
    
    if( type.contains( "32" ) )    // 4 bytes
    {
        ba[0] = getRamValue( address );
        ba[1] = getRamValue( address+1 );
        ba[2] = getRamValue( address+2 );
        ba[3] = getRamValue( address+3 );
    }
    else if( type.contains( "16" ) )  // 2 bytes
    {
        ba[0] = getRamValue( address );
        ba[1] = getRamValue( address+1 );
        ba[2] = 0;
        ba[3] = 0;
    }
    else                                  // 1 byte
    {
        ba[0] = getRamValue( address );
        ba[1] = 0;
        ba[2] = 0;
        ba[3] = 0;
    }
    if( type.contains( "f" ) )                          // float, double
    {
        float value = 0;
        memcpy(&value, ba, 4);
        m_ramTable->setItemValue( 1, value  );
    }
    else                                              // char, int, long
    {
        int value = 0;
        
        if( type.contains( "u" ) ) 
        {
            uint val = 0;
            memcpy(&val, ba, 4);
            value = val;
        }
        else memcpy(&value, ba, 4);

        m_ramTable->setItemValue( 1, value  );
        if( type.contains( "8" ) )
            m_ramTable->setItemValue( 2, decToBase(value, 2, 8)  );
        
    }
    //qDebug()<<name<<type <<address;
    if( !type.contains( "8" ) ) m_ramTable->setItemValue( 2, type  );
}

void BaseProcessor::addWatchVar( QString name, int address, QString type )
{
    if( !m_regsTable.contains(name) ) 
    {
        m_regsTable.insert( name, address );
        m_typeTable.insert( name, type ); 
    }
}

void BaseProcessor::setRegisters()// get register addresses from data file
{
    QStringList lineList = fileToStringList( m_dataFile, "BaseProcessor::setRegisters" );

    if( !m_regsTable.isEmpty() ) m_regsTable.clear();

    foreach( QString line, lineList )
    {
        if( line.contains("equ ") )   // This line contains a definition
        {
            line = line.toLower().replace("\t"," ");
            
            QString name    = "";
            QString addrtxt = "";
            int address   = 0;
            bool isNumber = false;

            line.remove("equ");
            QStringList wordList = line.split(" "); // Split in words
            name    = wordList.takeFirst();
            while( addrtxt.isEmpty() ) addrtxt = wordList.takeFirst();

            address = addrtxt.toInt( &isNumber, 10 );
            
            if( isNumber )        // If found a valid address add to map
            {
                address = validate( address );
                addWatchVar( name, address, "u8" );        // type uint8 
            }
            //qDebug() << name << address<<"\n";
        }
    }
}

void BaseProcessor::uartOut( uint32_t value ) // Send value to OutPanelText
{
    if( m_usartTerm )
    {
        TerminalWidget::self()->uartOut( value );
    }
    if( m_serialPort )
    {
        QByteArray ba;
        ba.resize(1);
        ba[0] = value;
        //ba[1] = 0;
        SerialPortWidget::self()->writeData( ba );
    }
}

void BaseProcessor::uartIn( uint32_t value ) // Receive one byte on Uart
{
    if( m_usartTerm )
    {
        TerminalWidget::self()->uartIn( value );
    }
}
/*QStringList BaseProcessor::getDefsList( QString fileName )
{
    return QStringList( getRegsTable( fileName ).uniqueKeys() );
}*/

#include "moc_baseprocessor.cpp"
