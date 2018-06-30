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

#include "picasmdebugger.h"
#include "baseprocessor.h"
#include "utils.h"
#include "simuapi_apppath.h"

PicAsmDebugger::PicAsmDebugger( QObject* parent, OutPanelText* outPane, QString filePath )
            : BaseDebugger( parent,outPane, filePath )
{
    setObjectName( "PIC asm Compiler" );
    
    m_compilerPath = "";
    m_compSetting = "gpasm_Path";
    
    readSettings();

    m_typesList["byte"]    = "uint8";
}
PicAsmDebugger::~PicAsmDebugger()
{}

bool PicAsmDebugger::loadFirmware()
{
    if( BaseDebugger::loadFirmware() )
    {
        mapLstToAsm();
        return true;
    }
    return false;
}

int PicAsmDebugger::step() // returns source line
{
    BaseProcessor::self()->stepOne();

    int pc = BaseProcessor::self()->pc();
    int line = m_flashToAsm[ pc ];
    //qDebug() <<"PicAsmDebugger::step"<<pc << line;
    return line ;
}

int PicAsmDebugger::stepOver(){return 0;}

int PicAsmDebugger::getValidLine( int line )
{
    int asmLine = 0;
    while( asmLine == 0 )
    {
        asmLine = m_asmToFlash.value(line);
        line++;
    }
    return m_flashToAsm[asmLine];
}

void PicAsmDebugger::mapLstToAsm()
{
    m_flashToAsm.clear();
    m_asmToFlash.clear();
    QString asmFileName = m_fileDir + m_fileName + ".asm";

    QString lstFileName = m_fileDir + m_fileName + ".lst";

    QStringList asmLines = fileToStringList( asmFileName, "PicAsmDebugger::mapLstToAsm" );
    QStringList lstLines = fileToStringList( lstFileName, "PicAsmDebugger::mapLstToAsm" );

    QString line;
    QString asmLine;
    int asmLineNumber = 0;
    int lastAsmLine = asmLines.size();

    foreach( line, lstLines )
    {
        if( line.isEmpty() )      continue;
        line = line.toUpper().replace("\t", " ");
        if( line.startsWith(" ") ) continue;
        if( line.contains("ORG") ) continue;

        while( true )
        {
            if( ++asmLineNumber >= lastAsmLine ) break; // End of asm file
            asmLine = asmLines.at( asmLineNumber ).toUpper();
            asmLine = asmLine.replace("\t", " ").remove(" ");
            if( asmLine.isEmpty() ) continue;
            if( asmLine.startsWith("_")) continue;
            if( asmLine.startsWith(";")) continue;
            if( asmLine.startsWith("#")) continue;
            if( asmLine.startsWith(".")) continue;

            QString lstline = line;
            if( lstline.remove(" ").contains(asmLine) ) break;
        }
        if( asmLineNumber >= lastAsmLine )
        {
            asmLineNumber = 0;
            continue; // End of asm file
        }
        QStringList words = line.split(' ');
        QString numberText = words.at(0);
        //QString numberText = line.left( 4 );
        bool ok = false;
        int address = numberText.toInt( &ok, 16 );  // original adress*2: instruc = 2 bytes
        if( ok )
        {
            m_flashToAsm[address] = asmLineNumber;
            //qDebug() <<"asmLineNumber"<<asmLineNumber<<"address"<<address;
        }
    }
    QHashIterator<int, int> i(m_flashToAsm);
    while( i.hasNext() )
    {
        i.next();
        int address       = i.key();
        int asmLineNumber = i.value();
        m_asmToFlash[asmLineNumber] = address;
    }
}

int PicAsmDebugger::compile()
{
    //getProcType();
    QString file = m_fileDir+m_fileName+m_fileExt;
    
    m_outPane->writeText( "-------------------------------------------------------\n" );
    QString command = m_compilerPath+"gpasm";
    
    #ifndef Q_OS_UNIX
    command  = addQuotes( command );
    file     = addQuotes( file );
    #endif

    command.append(" -L -w1  -i  -ainhx32 " + file);

    m_outPane->appendText( "Exec: ");
    m_outPane->appendText( command );
    m_outPane->writeText( "\n\n" );
    
    QProcess compAsm( this );
    compAsm.start( command  );
    compAsm.waitForFinished(-1);
    
    QString p_stdout = compAsm.readAllStandardOutput();
    QString p_stderr = compAsm.readAllStandardError();
    m_outPane->writeText( p_stdout );
    m_outPane->writeText( p_stderr );

    int error = 0;
    if( p_stdout.toUpper().contains("ERROR") ) 
    {
        QString line;
        QStringList lines = p_stdout.split("\n");
        foreach( line, lines )
        {
            if( !(line.toUpper().contains( "ERROR" )) ) continue;
            QStringList words = line.split(":");
            error = words.at(1).toInt();
            qDebug() <<line;
            break;
        }
    }
    m_firmware = m_fileDir+m_fileName+".hex";

    return error;
}

/*void PicAsmDebugger::setRegisters()  // get register addresses from lst file
{
    QString lstFileName = m_symbolFile;
    lstFileName.remove( lstFileName.lastIndexOf( "." ), 4 ).append( ".lst");
    QStringList lineList = fileToStringList( lstFileName, "PicAsmDebugger::setRegisters" );

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
}*/

#include "moc_picasmdebugger.cpp"
