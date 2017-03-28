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

#include "gcbdebugger.h"
#include "baseprocessor.h"
#include "utils.h"

GcbDebugger::GcbDebugger( QObject* parent, OutPanelText* outPane, QString filePath ) 
            : BaseDebugger( parent,outPane, filePath )
{
    m_avra = m_appPath+"/data/codeeditor/tools/avra";
    
    QStringList lines = fileToStringList( m_appPath+"/data/codeeditor/config", "CodeEditor::CodeEditor" );

    foreach( QString line, lines )  // Get path to gcbasic folder
    {
        if( line.startsWith("#") ) continue;
        
        if( line.contains("gcbasic_Path:") )
        {
            m_gcBasic = line.remove("gcbasic_Path:").remove(" ");
        }
    }
    m_typesList["byte"]    = "uint8";
    m_typesList["integer"] = "int16";
    m_typesList["word"]    = "uint16";
    m_typesList["long"]    = "uint32";
}
GcbDebugger::~GcbDebugger()
{}

bool GcbDebugger::loadFirmware()
{
    if( BaseDebugger::loadFirmware() )
    {
        getProcName();
        mapLstToAsm();
        if( m_fileExt == ".gcb" ) mapGcbToAsm(); // if gcb file
        return true;
    }
    return false;
}

void GcbDebugger::getProcName()
{
    QString asmFileName = m_fileDir + m_fileName + ".asm";
    

    QStringList lines = fileToStringList( asmFileName, "GcbDebugger::getProcName" );

    foreach( QString line, lines )
    {
        line = line.toLower();
        if( line.contains(".include") )  // Find info in inclded header
        {
            m_device = line.remove(".include").remove(" ").remove("\"").remove("def.inc")
                           .replace("t", "attiny").replace("m", "atmega");
            qDebug() << "GcbDebugger::getProcName Fonud device name: " << m_device;
            break;
        }
    }
}

int GcbDebugger::step() // returns source line
{
    BaseProcessor::self()->stepOne();

    int pc = BaseProcessor::self()->pc();
    int line = m_flashToAsm[ pc ];
    //qDebug() <<"GcbDebugger::step"<<pc << line<<m_isAsm;
    if( m_fileExt == ".gcb"  )        
    {
        if( !m_asmToGcb.contains( line ) ) line = -1;
        else line = m_asmToGcb[ line ];
    }
    return line ;
}

int GcbDebugger::stepOver(){return 0;}

void GcbDebugger::mapGcbToAsm()  // Map asm_source_line <=> gcb_source_line
{
    m_varList.clear();
    QStringList gcbLines = fileToStringList( m_fileDir+m_fileName+".gcb", "GcbDebugger::mapGcbToAsm" );

    foreach( QString gcbLine, gcbLines )              // Get Declared Variables
    {                                                 // Search lines containing "Dim"
        QString line = gcbLine; 
        if( !line.toUpper().contains( "DIM" )) continue;
        
        gcbLine = gcbLine.replace( "\t", " " );
        QStringList wordList = gcbLine.split( " " );
        wordList.removeAll( "" );
        if( wordList.first().toUpper() != "DIM" ) continue;
        
        QString type = wordList.at(3).toLower();
        if( m_typesList.contains( type ) )
        {
            QString varName = wordList.at(1).toLower();
            if( !m_varList.contains( varName ) )
                m_varList[ varName ] = m_typesList[ type ];
            //qDebug() << "GcbDebugger::mapGcbToAsm  variable "<<type<<varName<<m_typesList[ type ];
        }
    }
    m_asmToGcb.clear();
    m_gcbToAsm.clear();
    
    QString asmFileName = m_fileDir + m_fileName + ".asm";

    QStringList asmLines = fileToStringList( asmFileName, "GcbDebugger::mapGcbToAsm" );

    bool haveVariable = false;
    int asmLineNumber = 0;
    m_lastGcbLine = 0;
    QString srcm = ";Source:F1L";           // Gcbasic parses source lines to asm file

    foreach( QString asmLine, asmLines ) 
    {
        if( asmLine.contains(srcm))
        {
            asmLine.remove( srcm );
            int gcbLineNum = asmLine.split("S").first().toInt();
            
            if( gcbLineNum > m_lastGcbLine ) m_lastGcbLine = gcbLineNum;
            
            int asmLineNum = asmLineNumber+1;
            
            m_asmToGcb[ asmLineNum ] = gcbLineNum;
            m_gcbToAsm[ gcbLineNum ] = asmLineNum;
            //qDebug()<<"GcbDebugger::mapGcbToAsm  gcb:" << gcbLineNum <<"  asm:"<< asmLineNum;
        }
        else if( asmLine.contains("locations for variables")) haveVariable = true;
        else if( asmLine.contains(";*"))                      haveVariable = false;
        else if( haveVariable )
        {
            asmLine.remove( ".EQU").remove("\t").remove(" ");
            if( asmLine != "" )
             {
                QStringList text = asmLine.split("=");
                QString name = text.first().toLower();
                int address = text.last().toInt();
                QString type = "uint8";
                if( m_varList.contains( name ) ) type = m_varList[ name ];
                BaseProcessor::self()->addWatchVar( name, address ,type  );
                //qDebug()<<asmLine<<name<< address<<type;
            }
        }
        asmLineNumber++;
    }
}

int GcbDebugger::getValidLine( int line )
{
    if( m_fileExt == ".asm"  ) return getValidAsmLine( line );
    else                       return getValidGcbLine( line );
    return -1;
}

int GcbDebugger::getValidGcbLine( int line )
{
    while( !m_gcbToAsm.contains(line) && line<=m_lastGcbLine ) line++;
    return line;
}

int GcbDebugger::getValidAsmLine( int line )
{
    int asmLine = 0;
    while( asmLine == 0 )
    {
        asmLine = m_asmToFlash.value(line);
        line++;
    }
    return m_flashToAsm[asmLine];
}

void GcbDebugger::mapLstToAsm()
{
    m_flashToAsm.clear();
    m_asmToFlash.clear();
    QString asmFileName = m_fileDir + m_fileName + ".asm";

    QString lstFileName = m_fileDir + m_fileName + ".lst";

    QStringList asmLines = fileToStringList( asmFileName, "GcbDebugger::mapLstToAsm" );
    QStringList lstLines = fileToStringList( lstFileName, "GcbDebugger::mapLstToAsm" );

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

int GcbDebugger::compile()
{
    int error=-2;
    
    QString file = m_fileDir+m_fileName+m_fileExt;
    if     ( m_fileExt == ".gcb" ) error = compileGcb( file );
    else if( m_fileExt == ".asm" ) error = compileAsm( file );

    return error;
}

int GcbDebugger::compileGcb( QString file )
{
    QString args = " -NP -K:L -A:GCASM  ";

    QString command = m_gcBasic + "/gcbasic" + args + file;
    
    m_outPane->appendText( command );
    m_outPane->writeText( "\n\n" );

    QProcess compGcb( 0l );
    compGcb.start( command );
    compGcb.waitForFinished(-1);
    QString p_stdout = compGcb.readAllStandardOutput();

    m_outPane->writeText( p_stdout.remove("The message has been logged to the file Errors.txt.\n") );

    int error = -1;
    if( p_stdout.toUpper().contains("DONE")) // If gcb compiled ok then assemble if needed
    {
        //file = file.replace( ".gcb", ".asm" );
        //////if(  m_processorType == 2 ) {
            //replaceInFile( filePath, ".ORG", "nop ;.ORG", "CodeEditor::compile()" );
            //compileAsm( filePath );// Avr doesn't need assembly, simavr runs on hex file.
        //}
        m_firmware = m_fileDir+m_fileName+".hex";
        error = 0;
    }
    else // some error found
    {
        QString line;
        QStringList lines = p_stdout.split("\n");
        foreach( line, lines )
        {
            if( !(line.contains( "Error:" )) ) continue;
            QStringList words = line.split(" ");
            words.removeFirst();
            error = words.first().remove("(").remove(")").remove(":").toInt();
            break;
        }
    }
    return error;
}

void GcbDebugger::replaceORG( QString filePath )
{
    //qDebug() << "CodeEditor::replaceORG" << filePath;
    QString text = fileToString( filePath, "CodeEditor::replaceORG" );
    QFile   file( filePath );
    text.replace( ".ORG" , "nop ;.ORG" );   // replace text in string

    if( !file.open(QFile::WriteOnly|QFile::Text) )
        QMessageBox::warning(0l, "CodeEditor::replaceORG", "Cannot read file "+filePath+":\n"+file.errorString() );

    QTextStream ffout(&file);
    ffout << text;
    file.flush();
    file.close();
}

int GcbDebugger::compileAsm( QString file )
{
    getProcType();
    
    m_outPane->writeText( "-------------------------------------------------------\n" );
    QString command  = "";

    if(  m_processorType == 1 )     //Pic
    {
        command = "gpasm -L -w1  -i  -ainhx32 " + file;
    }
    else if(  m_processorType == 2 ) // Avr
    {
        command = m_avra + "/avra -W NoRegDef"; // supress some warnings
        command.append(" -l "+ m_fileName+".lst" );  // output list file
        command.append(" -I " + m_avra);                  // include dir
        command.append( m_fileName+".asm" );         // File to assemble
    }
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
    if( p_stderr.toUpper().contains("ERROR ") || p_stdout.toUpper().contains("ERROR ") ) 
    { error = -1; }

    return error;
    //TODO: find Error line number and move cursor to it
}


void  GcbDebugger::getProcType()
{
   //qDebug() << "m_asmPath: " << m_asmPath;
    QStringList lines = fileToStringList( m_fileDir+m_fileName+".asm", "CodeEditor::getProcType" );

    foreach( QString line, lines )
    {
        line = line.toLower();
        if( line.contains("include") )  // Find info in included header
        {
            line.remove("include").remove("#").remove(" ").remove("<").remove(">");
            if(line.startsWith("p")) m_processorType = 1; // if mcu name starts with p then is Pic
            else                     m_processorType = 2; // Avr
            break;
        }
    }
    //qDebug() << "Processor Type: " << m_processorType;
}
/*void GcbDebugger::setRegisters()  // get register addresses from lst file
{
    QString lstFileName = m_symbolFile;
    lstFileName.remove( lstFileName.lastIndexOf( "." ), 4 ).append( ".lst");
    QStringList lineList = fileToStringList( lstFileName, "GcbDebugger::setRegisters" );

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

#include "moc_gcbdebugger.cpp"
