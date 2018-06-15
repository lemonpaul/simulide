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

#include "inodebugger.h"
#include "baseprocessor.h"
#include "mainwindow.h"
#include "utils.h"
#include "simuapi_apppath.h"

InoDebugger::InoDebugger( QObject* parent, OutPanelText* outPane, QString filePath ) 
           : BaseDebugger( parent, outPane, filePath )
{
    //QSettings settings( SIMUAPI_AppPath::self()->availableDataFilePath("codeeditor/config.ini"),
    //                    QSettings::IniFormat );
    QSettings* settings = MainWindow::self()->settings();

    m_arduino = "";
     
    if( settings->contains("arduino_Path") )
        m_arduino = settings->value("arduino_Path").toString();
    
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
{}

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
    QDir arduinoDir( m_arduino );
    if( !arduinoDir.exists() )
    {
        m_outPane->appendText( "\nArduino ToolChain not found\n" );
        m_outPane->writeText( "\nRight-Click on Document Tab to set Path\n\n" );
        return -1;
    }

    QString filePath = m_fileDir+m_fileName+m_fileExt;
    // Doing build in the user directory
    QString buildPath = SIMUAPI_AppPath::self()->RWDataFolder().absoluteFilePath("codeeditor/buildIno");
    //m_appPath+"/data/codeeditor/buildIno";
    
    QDir dir(buildPath);
    dir.removeRecursively();                         // Remove old files
    dir.mkpath(buildPath+"/"+m_fileName);            // Create sketch folder
    
    QDir directory( m_fileDir );          
    QStringList fileList = directory.entryList(QDir::Files);
    
    foreach( QString fileName, fileList ) // Copy files to sketch folder
    {
        QFile::copy( m_fileDir+fileName, buildPath+"/"+m_fileName+"/"+fileName );
    }
    
    QString ProcInoFile = buildPath+"/"+m_fileName+"/"+m_fileName+m_fileExt;
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
        if( !wordList.isEmpty())      // Fix crash on empty list.first()
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
        out << inoLine << " // INOLINE " << inoLineNumber << "\n";
    }
    file.close();

    ///TODO: verify arduino version, older versions can compile, but no sorce code emited into .lst file
    /// , then debugger will hang!
    QString cBuildPath = buildPath;
    QString preferencesPath = SIMUAPI_AppPath::self()->availableDataFilePath("codeeditor/preferences.txt");
    QString command  = m_arduino +"arduino";
    
    #ifndef Q_OS_UNIX
    command    += "_debug";
    command     = addQuotes( command );
    cBuildPath  = addQuotes( cBuildPath );
    ProcInoFile = addQuotes( ProcInoFile );
    #endif
                
    command += " -v --board arduino:avr:uno --pref build.path=" + cBuildPath;
    if( !preferencesPath.isEmpty() )
        command += " --preferences-file " + preferencesPath; //m_appPath+"/data/codeeditor/preferences.txt"
    command += " --preserve-temp-files --verify " + ProcInoFile;
    m_firmware = "";
    
    m_outPane->appendText( command );
    m_outPane->writeText( "\n\n" );
    
    m_compProcess.start( command );
    m_compProcess.waitForFinished(-1);
    
    QString p_stderr = m_compProcess.readAllStandardError();
    m_outPane->appendText( p_stderr );
    m_outPane->writeText( "\n\n" );
    
    int error = -1;
    if( p_stderr=="" )
    {
        m_outPane->appendText( "\nArduino ToolChain not found\n" );
        m_outPane->writeText( "\nRight-Click on Document Tab to set Path\n\n" );
        error = -1;
    }
    else if( p_stderr.toUpper().contains("ERROR") )
    {
        QString line;
        QStringList lines = p_stderr.split("\n");
        foreach( line, lines )
        {
            if( !(line.contains( "error:" )) ) continue;
            QStringList words = line.split(":");
            error = words.at(1).toInt();
            break;
        }
    }
    else
    {
        m_firmware = buildPath + "/"+ m_fileName + ".ino.hex";

        QString objdump = m_arduino+"hardware/tools/avr/bin/avr-objdump";
        //objdump.remove( objdump.lastIndexOf( "arduino" ), 7 );
        //objdump = objdump+"hardware/tools/avr/bin/avr-objdump";
        
        QString elfPath = buildPath+"/"+m_fileName+".ino.elf";
        
        #ifndef Q_OS_UNIX
        objdump = addQuotes( objdump );
        elfPath = addQuotes( elfPath );
        #endif
        
        command  = objdump+" -h -S "+elfPath;
        
        QProcess compIno( 0l );

        compIno.setStandardOutputFile( buildPath+"/"+m_fileName+".ino.lst" );
        compIno.start( command );
        compIno.waitForFinished(-1);

        error = 0;
    }
    return error;
}

void InoDebugger::getCompilerPath()
{
        m_arduino = QFileDialog::getExistingDirectory( 0L,
                               tr("Select Arduino toolchain directory, version 1.8.0 and up"),
                               m_arduino,
                               QFileDialog::ShowDirsOnly
                             | QFileDialog::DontResolveSymlinks);
        m_arduino += "/";

        //QSettings settings( SIMUAPI_AppPath::self()->availableDataFilePath("codeeditor/config.ini"),
        //                    QSettings::IniFormat );
        MainWindow::self()->settings()->setValue("arduino_Path", m_arduino);

        m_outPane->appendText( "\nUsing Arduino Path: \n" );
        m_outPane->writeText( m_arduino+"\n\n" );
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
    QString buildPath = SIMUAPI_AppPath::self()->RWDataFolder().absoluteFilePath("codeeditor/buildIno");
    //QString buildPath = SIMUAPI_AppPath::self()->availableDataDirPath("codeeditor/buildIno");
    //m_appPath+"/data/codeeditor/buildIno";
    
    QString lstFileName = buildPath+"/"+ m_fileName + ".ino.lst";
    QStringList lstLines = fileToStringList( lstFileName, "InoDebugger::mapInoToFlash" );

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
