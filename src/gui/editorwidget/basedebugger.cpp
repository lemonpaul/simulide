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

#include "basedebugger.h"
#include "mainwindow.h"

bool BaseDebugger::m_loadStatus = false;

BaseDebugger::BaseDebugger( QObject* parent, OutPanelText* outPane, QString filePath ) 
            : QObject( parent )
            , m_compProcess( 0l )
{
    m_outPane = outPane;
    m_appPath   = QCoreApplication::applicationDirPath();
    
    m_fileDir  = filePath;
    m_fileName = filePath.split("/").last();
    m_fileDir.remove( m_fileDir.lastIndexOf( m_fileName ), m_fileName.size() );
    m_fileExt  = "."+m_fileName.split(".").last();
    m_fileName = m_fileName.remove( m_fileName.lastIndexOf( m_fileExt ), m_fileExt.size() );

    m_processorType = 0;
    
    connect( &m_compProcess, SIGNAL(readyRead()), SLOT(ProcRead()) );
}
BaseDebugger::~BaseDebugger( )
{}

bool BaseDebugger::loadFirmware()
{
    if ( m_firmware == "" )  return false;
    
    upload();
    if( m_loadStatus ) return false;
    
    m_loadStatus = true;

    return true;
}

void BaseDebugger::upload()
{
    if( m_loadStatus )
    {
        QMessageBox::warning( 0, tr("BaseDebugger::loadFirmware"),
                                 tr("Debugger already running\nStop active session") );
        return;
    }
    m_outPane->writeText( "-------------------------------------------------------\n" );
    m_outPane->appendText( tr("\nUploading: \n") );
    m_outPane->appendText( m_firmware );
    m_outPane->writeText( "\n\n" );
    
    if( McuComponent::self() ) 
    {
        McuComponent::self()->load( m_firmware );
        m_outPane->appendText( tr("\nFirmWare Uploaded to ")+McuComponent::self()->device()+"\n" );
        m_outPane->writeText( "\n\n" );
    }
}

void BaseDebugger::stop()
{
    m_loadStatus = false;
}

void BaseDebugger::getProcName()
{
}

int BaseDebugger::step()
{
    return 0;
}

int BaseDebugger::stepOver(){return 0;}

int BaseDebugger::getValidLine( int line )
{
    Q_UNUSED( line );
    return 0;
}

void BaseDebugger::ProcRead()
{
    while( m_compProcess.canReadLine() ) 
    {
        m_outPane->appendText( m_compProcess.readLine() );
        m_outPane->writeText( "\n" );
    }
}

QString BaseDebugger::compilerPath()
{
    return m_compilerPath;
}

void BaseDebugger::readSettings()
{
    QSettings* settings = MainWindow::self()->settings();
    
    if( settings->contains( m_compSetting ) )
        m_compilerPath = settings->value( m_compSetting ).toString();
}

void BaseDebugger::getCompilerPath()
{
        m_compilerPath = QFileDialog::getExistingDirectory( 0L,
                               tr("Select Compiler toolchain directory"),
                               m_compilerPath,
                               QFileDialog::ShowDirsOnly
                             | QFileDialog::DontResolveSymlinks);

        if( m_compilerPath != "" ) m_compilerPath += "/";

        MainWindow::self()->settings()->setValue( m_compSetting, m_compilerPath);

        m_outPane->appendText( "Using Compiler Path: \n" );
        m_outPane->writeText( m_compilerPath+"\n\n" );
}

void BaseDebugger::setCompilerPath( QString path )
{
    m_compilerPath = path;
    MainWindow::self()->settings()->setValue( m_compSetting, m_compilerPath );
}
#include "moc_basedebugger.cpp"

