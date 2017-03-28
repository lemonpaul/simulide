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

#include "basedebugger.h"

bool BaseDebugger::m_loadStatus = false;

BaseDebugger::BaseDebugger( QObject* parent, OutPanelText* outPane, QString filePath ) 
            : QObject( parent )
{
    m_outPane = outPane;
    m_appPath   = QCoreApplication::applicationDirPath();
    
    m_fileDir  = filePath;
    m_fileName = filePath.split("/").last();
    //m_fileDir  = m_fileDir.remove( m_fileName );
    m_fileDir.remove( m_fileDir.lastIndexOf( m_fileName ), m_fileName.size() );
    m_fileExt  = "."+m_fileName.split(".").last();
    m_fileName = m_fileName.remove( m_fileExt );
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
        QMessageBox::warning( 0, tr("InoDebugger::loadFirmware"), 
                                 tr("Debugger already running\nStop active session") );
        return;
    }
    m_outPane->writeText( "-------------------------------------------------------\n" );
    m_outPane->appendText( tr("\nUploading: \n") );
    m_outPane->appendText( m_firmware );
    m_outPane->writeText( "\n\n" );
    
    if( McuComponent::self() ) McuComponent::self()->load( m_firmware );
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
    return 0;
}

#include "moc_basedebugger.cpp"

