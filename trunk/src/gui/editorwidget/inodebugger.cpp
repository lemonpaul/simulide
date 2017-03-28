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

#include "inodebugger.h"
#include "baseprocessor.h"
#include "utils.h"

InoDebugger::InoDebugger( QObject* parent, OutPanelText* outPane, QString filePath ) 
            : BaseDebugger( parent,outPane, filePath )
{
    QStringList lines = fileToStringList( m_appPath+"/data/codeeditor/config", "CodeEditor::CodeEditor" );

    foreach( QString line, lines )  // Get path to arduino folder
    {
        if( line.startsWith("#") ) continue;
        
        if( line.contains("arduino_Path:") )
        {
            m_arduino = line.remove("arduino_Path:").remove(" ");
        }
    }
    m_typesList["char"]   = "int8";
    m_typesList["uchar"]  = "uint8";
    m_typesList["int"]    = "int16";
    m_typesList["uint"]   = "uint16";
    m_typesList["short"]  = "int16";
    m_typesList["ushort"] = "uint16";
    m_typesList["long"]   = "int32";
    m_typesList["ulong"]  = "uint32";
    m_typesList["float"]  = "float32";
}
InoDebugger::~InoDebugger()
{
    /*QString command  = m_arduino + " --pref build.path= --board arduino:avr:uno --save-prefs";
    
    QProcess compIno( 0l );
    compIno.start( command );
    compIno.waitForFinished(-1);*/
}

bool InoDebugger::loadFirmware()
{
    if( BaseDebugger::loadFirmware() )
    {
        mapInoToFlash();
        return true;
    }
    return false;
}

int InoDebugger::compile()
{
    QString filePath = m_fileDir+m_fileName+m_fileExt;
    QString buildPath = m_appPath+"/data/codeeditor/buildIno";
    
    QDir dir(buildPath);
    dir.removeRecursively();
    dir.mkpath(buildPath +"/preino");
    
    QString ProcInoFile = buildPath +"/preino/"+m_fileName+m_fileExt;
    QFile file( ProcInoFile );

    if( !file.open(QFile::WriteOnly | QFile::Text) )
    {
          QMessageBox::warning(0l, "InoDebugger::compile",
          tr("Cannot write file %1:\n%2.").arg(ProcInoFile).arg(file.errorString()));
          return -1;
    }
    QTextStream out(&file);
    
    QStringList inoLines = fileToStringList( filePath, "InoDebugger::compile" );
    m_lastInoLine = inoLines.size();
    QString line;
    QString inoLine;
    int inoLineNumber = 0;
    
    m_varList.clear();
    foreach( inoLine, inoLines )                        // Get Variables
    {
        line = inoLine;
        line = line.replace( "\t", " " ).remove(";");
        QStringList wordList= line.split( " " );
        wordList.removeAll( "" );
        if( !wordList.isEmpty() )
        {
            QString type = wordList.first();
            if( m_typesList.contains( type ) )
            {
                QString varName = wordList.at(1);
                if( !m_varList.contains( varName ) )
                    m_varList[ varName ] = m_typesList[ type ];
                    
                //qDebug() << "InoDebugger::compile  variable "<<type<<varName<<m_typesList[ type ];
            }
        }
        inoLineNumber++;
        qDebug() << inoLine << " // INOLINE " << inoLineNumber;
        out << inoLine << " // INOLINE " << inoLineNumber << "\n";
    }
    file.close();
    
    QString command  = m_arduino +"/arduino"
                #ifdef Win
                     + "_debug"
                #endif
                     + " -v --board arduino:avr:uno --pref build.path=" 
                     + buildPath + " --verify "+ ProcInoFile;
    m_firmware = "";
    
    m_outPane->appendText( command );
    m_outPane->writeText( "\n\n" );
    
    QProcess compIno( 0l );
    compIno.start( command );
    compIno.waitForFinished(-1);
    compIno.waitForReadyRead(-1);
    
    QString p_stdout = compIno.readAllStandardOutput();
    QString p_stderr = compIno.readAllStandardError();
    m_outPane->appendText( p_stderr );
    m_outPane->appendText( "\n" );
    m_outPane->appendText( p_stdout );
    m_outPane->writeText( "\n\n" );
    
    int error = -1;
    if( p_stderr.toUpper().contains("ERROR") || p_stdout.toUpper().contains("ERROR") )
    {
        QString line;
        QStringList lines = p_stderr.split("\n");
        foreach( line, lines )
        {
            if( !(line.contains( "error:" )) ) continue;
            QStringList words = line.split(" ");
            //error = words.first().remove( m_fileName ).remove(":").toInt();
            break;
        }
    }
    else
    {
        m_firmware = buildPath + "/"+ m_fileName + ".ino.hex";

        QString objdump = m_arduino;
        //objdump.remove( objdump.lastIndexOf( "arduino" ), 7 );
        objdump = objdump+"/hardware/tools/avr/bin/avr-objdump";
        
        command  = objdump+" -S "+buildPath+"/"+m_fileName+".ino.elf";
        
        compIno.setStandardOutputFile( buildPath+"/"+m_fileName+".ino.lst" );
        compIno.start( command );
        compIno.waitForFinished(-1);

        error = 0;
    }
    return error;
}

int InoDebugger::step() // returns source line
{
    BaseProcessor::self()->stepOne();

    int pc = BaseProcessor::self()->pc();
    int line = m_flashToIno[ pc ];

    return line ;
}

int InoDebugger::stepOver(){return 0;}

int InoDebugger::getValidLine( int line )
{
    while( !m_inoToFlash.contains(line) && line<=m_lastInoLine ) line++;
    return line;
}

void InoDebugger::mapInoToFlash()
{
    m_flashToIno.clear();
    m_inoToFlash.clear();
    
    QString buildPath = m_appPath+"/data/codeeditor/buildIno";
    
    QString lstFileName = buildPath+"/"+ m_fileName + ".ino.lst";
    QStringList lstLines = fileToStringList( lstFileName, "InoDebugger::mapLstToAsm" );

    bool readFlasAddr = false;
    int inoLineNum = -1;
    QString line;
    foreach( line, lstLines )
    {
        if( readFlasAddr ) // Last line contained source line
        {
            bool ok = false;
            int flashAddr = line.split( ":" ).first().toInt( &ok, 16 );
            if( ok )
            {
                m_flashToIno[ flashAddr ] = inoLineNum;
                m_inoToFlash[ inoLineNum ] = flashAddr;
            }
            readFlasAddr = false;
            //qDebug()<<"InoDebugger::mapInoToFlash ino-flash:" << inoLineNum << flashAddr ;
        }
        if( line.contains("INOLINE") )
        {
            inoLineNum = line.split( " " ).last().toInt()-1;
            readFlasAddr = true;
        }
        QHashIterator<QString, QString> i( m_varList );
        while (i.hasNext())                             // Find Variable 
        {
            i.next();
            QString varName = "<"+i.key()+">";
            if( line.contains( varName ) )       // Get variable address
            {
                line = line.remove( " "+varName ).split( " " ).last().remove( "0x80" );
                bool ok = false;
                int address = line.toInt( &ok, 16 );
                if( ok ) BaseProcessor::self()->addWatchVar( i.key(), address, i.value() );
                
                //qDebug() << "InoDebugger::mapInoToFlash  variable "<<line<<i.key()<<address<<i.value();
                
                break;
            }
        }
        
    }
}

#include "moc_inodebugger.cpp"
