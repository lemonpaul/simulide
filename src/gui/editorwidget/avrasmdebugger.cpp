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

#include "avrasmdebugger.h"
#include "baseprocessor.h"
#include "utils.h"
#include "simuapi_apppath.h"

AvrAsmDebugger::AvrAsmDebugger( QObject* parent, OutPanelText* outPane, QString filePath )
            : BaseDebugger( parent,outPane, filePath )
{
    //m_avra = m_appPath+"/data/codeeditor/tools/avra";
    m_avraInc = SIMUAPI_AppPath::self()->availableDataDirPath("codeeditor/tools/avra");
    #ifndef Q_OS_UNIX
    m_avraInc = addQuotes( m_avraInc );
    #endif
    m_avra = "";
    /*//m_appPath+"/data/codeeditor/config"
    QStringList lines = fileToStringList( SIMUAPI_AppPath::self()->availableDataFilePath("codeeditor/config"),
                                          "CodeEditor::CodeEditor" );

    foreach( QString line, lines )  // Get path to avra executable folder
    {
        if( line.contains("avra_Path:") )
        {
            m_avra = line.remove("avra_Path:");
            while( m_avra.startsWith(" ") )
                m_avra.remove( 0, 1 );
                
            while( m_avra.endsWith(" ") )
                m_avra.remove( m_avra.length()-1, 1 );
            
            m_avra = m_avra+"/";
            break;
        }
    }*/
    m_typesList["byte"]    = "uint8";
}
AvrAsmDebugger::~AvrAsmDebugger()
{}

bool AvrAsmDebugger::loadFirmware()
{
    if( BaseDebugger::loadFirmware() )
    {
        mapLstToAsm();
        return true;
    }
    return false;
}

int AvrAsmDebugger::step() // returns source line
{
    BaseProcessor::self()->stepOne();

    int pc = BaseProcessor::self()->pc();
    int line = m_flashToAsm[ pc ];
    //qDebug() <<"AvrAsmDebugger::step"<<pc << line;
    return line ;
}

int AvrAsmDebugger::stepOver(){return 0;}

int AvrAsmDebugger::getValidLine( int line )
{
    int addr = 0;
    while( addr == 0 )
    {
        addr = m_asmToFlash.value(line);
        line++;
    }
    return line-1;
}

int AvrAsmDebugger::getProgramStart()
{
    return m_flashToAsm[ 0 ];
}

void AvrAsmDebugger::mapLstToAsm()
{
    m_flashToAsm.clear();
    m_asmToFlash.clear();
    QString asmFileName = m_fileDir + m_fileName + ".asm";

    QString lstFileName = m_fileDir + m_fileName + ".lst";

    QStringList asmLines = fileToStringList( asmFileName, "AvrAsmDebugger::mapLstToAsm" );
    QStringList lstLines = fileToStringList( lstFileName, "AvrAsmDebugger::mapLstToAsm" );

    QString asmLine;
    int asmLineNumber = 0;
    int lastAsmLine = asmLines.size();

    foreach( QString lstLine, lstLines )
    {
        if( !lstLine.startsWith( "C:") ) continue;            // avra code lines start with C:

        lstLine = lstLine.toUpper().remove(0,2).replace("\t", " ").remove((" "));

        while( true )
        {
            if( ++asmLineNumber >= lastAsmLine ) break;                   // End of asm file
            asmLine = asmLines.at( asmLineNumber ).toUpper();
            asmLine = asmLine.replace("\t", " ").remove(" ");
            if( asmLine.isEmpty() ) continue;
            if( asmLine.startsWith(";")) continue;
            if( asmLine.startsWith("#")) continue;
            if( asmLine.startsWith(".")) continue;

            //qDebug() <<"AvrAsmDebugger::mapLstToAsm() "<<asmLine;
            //qDebug() <<"AvrAsmDebugger::mapLstToAsm() "<<lstLine;

            if( lstLine.contains(asmLine) ) break;                            // Line found
        }
        QString numberText = lstLine.left( 6 );    // first 6 digits in lst file is address
        bool ok = false;
        int address = numberText.toInt( &ok, 16 )*2;         // adress*2: instruc = 2 bytes
        if( ok )
        {
            m_flashToAsm[address] = asmLineNumber;
            //qDebug() <<"AvrAsmDebugger::mapLstToAsm() "<<address<<asmLineNumber;
        }
    }
    QHashIterator<int, int> i(m_flashToAsm);
    while( i.hasNext() )
    {
        i.next();
        int address       = i.key();
        int asmLineNumber = i.value();
        m_asmToFlash[asmLineNumber] = address;
        //qDebug() <<"AvrAsmDebugger::mapLstToAsm() "<<address<<asmLineNumber;
    }
}

int AvrAsmDebugger::compile()
{
    //getProcType();
    QString file = m_fileDir+m_fileName+m_fileExt;
    
    m_outPane->writeText( "-------------------------------------------------------\n" );
    
    QString listFile = m_fileDir+m_fileName+".lst";
    QString command  = m_avra+"avra";
    
    #ifndef Q_OS_UNIX
    command  = addQuotes( command );
    listFile = addQuotes( listFile );
    file     = addQuotes( file );
    #endif
    
    command.append(" -W NoRegDef");             // supress some warnings
    command.append(" -l "+ listFile );               // output list file
    command.append(" -I "+ m_avraInc+" ");                // include dir
    command.append( file );                          // File to assemble

    m_outPane->appendText( "Exec: ");
    m_outPane->appendText( command );
    m_outPane->writeText( "\n\n" );
    
    m_compProcess.start( command );
    m_compProcess.waitForFinished(-1);
    
    QString p_stderr = m_compProcess.readAllStandardError();
    m_outPane->appendText( p_stderr );
    m_outPane->writeText( "\n\n" );

    int error = 0;
    if( p_stderr.toUpper().contains("ERROR ") ) 
    { 
        QString line;
        QStringList lines = p_stderr.split("\n");
        foreach( line, lines )
        {
            if( !(line.toUpper().contains( "ERROR " )) ) continue;
            QStringList words = line.split(":");
            QString filePath = m_fileDir+m_fileName+m_fileExt;
            error = words.first().remove(filePath).remove("(").remove(")").toInt();
            break;
        }
    }
    m_firmware = m_fileDir+m_fileName+".hex";

    return error;
}

/*void AvrAsmDebugger::setRegisters()  // get register addresses from lst file
{
    QString lstFileName = m_symbolFile;
    lstFileName.remove( lstFileName.lastIndexOf( "." ), 4 ).append( ".lst");
    QStringList lineList = fileToStringList( lstFileName, "AvrAsmDebugger::setRegisters" );

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

#include "moc_avrasmdebugger.cpp"
