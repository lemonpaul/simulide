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

BaseProcessor::BaseProcessor( QObject* parent )
     : QObject( parent )
{
    m_loadStatus = false;
    m_ramTable   = 0l;
    m_symbolFile = "";
    m_device     = "";
    //m_timerId   = 0;
}
BaseProcessor::~BaseProcessor() {}

/*void BaseProcessor::runMcu()
{
    step();
    //m_timerId = startTimer( 10 );
}

void BaseProcessor::stopMcu()
{
    if( m_timerId != 0 )
    {
        killTimer( m_timerId );
        m_timerId = 0;
    }
}*/

/*void BaseProcessor::timerEvent( QTimerEvent* e )
{
    e->accept();
    step();
}*/


void BaseProcessor::terminate()
{
    qDebug() <<"\nBaseProcessor::terminate "<<m_device<<m_symbolFile<<"\n";
    //emit terminated();

    if( m_ramTable )
    {
        MainWindow::self()->ramTabWidgetLayout->removeWidget( m_ramTable );
        delete m_ramTable;
    }
    m_loadStatus = false;
    m_ramTable   = 0l;
    m_symbolFile = "";
    //m_device     = "";
}

void BaseProcessor::initialized()
{
    qDebug() << "Loaded: " << m_symbolFile;

    //setRegisters();
    m_loadStatus = true;
    m_ramTable = new RamTable( this );
    MainWindow::self()->ramTabWidgetLayout->addWidget( m_ramTable );
}

QString BaseProcessor::getFileName() { return m_symbolFile; }

void BaseProcessor::setDevice( QString device ) { m_device = device;}

QString BaseProcessor::getDevice() { return m_device;}


void BaseProcessor::reset()
{
    //qDebug()<<"BaseProcessor::reset"<<m_device<<"m_loadStatus"<<m_loadStatus;
    //emit chipReset();
}

int BaseProcessor::getRegAddress( QString name ) { return m_regsTable[name]; }

QStringList BaseProcessor::getDefsList( QString fileName )
{
    return QStringList( getRegsTable( fileName ).uniqueKeys() );
}

void BaseProcessor::setRegisters()
{
    m_regsTable = getRegsTable( m_symbolFile );
}

#include "moc_baseprocessor.cpp"
