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

#include <qstringlist.h>
#include <QDomDocument>
#include <QFileInfo>

#include "mainwindow.h"
#include "mcucomponent.h"
#include "mcucomponentpin.h"
#include "terminalwidget.h"
#include "connector.h"
#include "simulator.h"
#include "simuapi_apppath.h"

McuComponent* McuComponent::m_pSelf = 0l;
bool McuComponent::m_canCreate = true;

McuComponent::McuComponent( QObject* parent, QString type, QString id )
    : Package( parent, type, id )
{    
    qDebug() << "        Initializing"<<m_id<<"...";
    
    m_canCreate  = false;
    //m_serialTerm = false;
    m_attached   = false;
    m_processor  = 0l;
    m_symbolFile = "";
    m_device     = "";
    m_error      = 0;
    
    // Id Label Pos set in Package::initPackage
    m_color = QColor( 50, 50, 70 );

    QSettings* settings = MainWindow::self()->settings();
    m_lastFirmDir = settings->value("lastFirmDir").toString();
    
    if( m_lastFirmDir.isEmpty() )
        m_lastFirmDir = QCoreApplication::applicationDirPath();
}
McuComponent::~McuComponent() {}

void McuComponent::initPackage()
{
    QString compName = m_id.split("-").first(); // for example: "atmega328-1" to: "atmega328"

    m_dataFile = ComponentSelector::self()->getXmlFile( compName );
    
    //QString dfPath = SIMUAPI_AppPath::self()->availableDataFilePath(m_dataFile);
    //qDebug() << "McuComponent::initPackage datafile: " << compName << " <= " << dfPath;
    QFile file( m_dataFile );
    
    if(( m_dataFile == "" ) || ( !file.exists() ))
    {
        m_error = 1;
        return;
    }
    
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        QMessageBox* msgBox = new QMessageBox( MainWindow::self() );
        msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
        msgBox->setStandardButtons( QMessageBox::Ok );
        msgBox->setWindowTitle( tr("Error") );
        msgBox->setText( tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()) );
        msgBox->setModal( false ); 
        msgBox->open();
        m_error = 1;
        return;
    }

    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
        QMessageBox* msgBox = new QMessageBox( MainWindow::self() );
        msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
        msgBox->setStandardButtons( QMessageBox::Ok );
        msgBox->setWindowTitle( tr("Error") );
        msgBox->setText( tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile) );
        msgBox->setModal( false ); 
        msgBox->open();
        file.close();
        m_error = 1;
        return;
    }
    file.close();

    QDomElement root  = domDoc.documentElement();
    QDomNode    rNode = root.firstChild();
    QString package;

    while( !rNode.isNull() )
    {
        QDomElement element = rNode.toElement();
        QDomNode    node    = element.firstChild();

        while( !node.isNull() ) 
        {
            QDomElement element = node.toElement();
            if( element.attribute("name")==compName )
            {
                // Get package file
                QDir dataDir(  m_dataFile );
                dataDir.cdUp();             // Indeed it doesn't cd, just take out file name
                m_dataFile = dataDir.filePath( element.attribute( "package" ) )+".package";
                
                // Get device
                m_device = element.attribute( "device" );
                //qDebug()<<"McuComponent::initPackage" << "m_device" <<m_device;
                
                m_processor->setDevice( m_device );
                //else qDebug() << compName << "ERROR!! McuComponent::initPackage m_processor: " << m_processor;
                
                // Get data file
                QString dataFile = dataDir.filePath( element.attribute( "data" ) )+".data";
                //qDebug() <<m_device<< dataFile;
                m_processor->setDataFile( dataFile );
                if( element.hasAttribute( "icon" ) ) m_BackGround = ":/" + element.attribute( "icon" );

                break;
            }
            node = node.nextSibling();
        }
        rNode = rNode.nextSibling();
    }
    if( m_device != "" ) Package::initPackage();
    else 
    {
        m_error = 1;
        qDebug() << compName << "ERROR!! McuComponent::initPackage Package not Found: " << package;
    }
}

int McuComponent::freq()
{ 
    return m_freq; 
}
void McuComponent::setFreq( int freq )
{ 
    if     ( freq < 0  ) freq = 0;
    else if( freq > 100 ) freq = 100;
    
    Simulator::self()->setMcuClock( freq );
    m_freq = freq; 
}

void McuComponent::reset()
{
    for ( int i = 0; i < m_pinList.size(); i++ ) // Reset pins states
        m_pinList[i]->resetOutput();
    
    m_processor->reset();
}

void McuComponent::terminate()
{
    qDebug() <<"        Terminating"<<m_id<<"...";
    m_processor->terminate();
    for( int i=0; i<m_numpins; i++ ) m_pinList[i]->terminate();
    m_pSelf = 0l;
    //reset();
    qDebug() <<"     ..."<<m_id<<"Terminated\n";
}

void McuComponent::remove()
{
    foreach( McuComponentPin* mcupin, m_pinList )
    {
        Pin* pin = mcupin->pin(); 
        if( pin->connector() ) pin->connector()->remove();
        //delete mcupin;
    }
    slotCloseTerm();
    slotCloseSerial();
    terminate();
    m_pinList.clear();
    
    m_canCreate = true;

    Component::remove();
}

void McuComponent::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu* menu = new QMenu();
    QAction* loadAction = menu->addAction( QIcon(":/load.png"),tr("Load firmware") );
    connect( loadAction, SIGNAL(triggered()), this, SLOT(slotLoad()) );

    QAction* reloadAction = menu->addAction( QIcon(":/reload.png"),tr("Reload firmware") );
    connect( reloadAction, SIGNAL(triggered()), this, SLOT(slotReload()) );
    
    QAction* openTerminal = menu->addAction( QIcon(":/terminal.png"),tr("Open Serial Monitor.") );
    connect( openTerminal, SIGNAL(triggered()), this, SLOT(slotOpenTerm()) );
    
    QAction* closeTerminal = menu->addAction( QIcon(":/closeterminal.png"),tr("Close Serial Monitor") );
    connect( closeTerminal, SIGNAL(triggered()), this, SLOT(slotCloseTerm()) );

    QAction* openSerial = menu->addAction( QIcon(":/terminal.png"),tr("Open Serial Port.") );
    connect( openSerial, SIGNAL(triggered()), this, SLOT(slotOpenSerial()) );

    QAction* closeSerial = menu->addAction( QIcon(":/closeterminal.png"),tr("Close Serial Port") );
    connect( closeSerial, SIGNAL(triggered()), this, SLOT(slotCloseSerial()) );

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}


void McuComponent::slotOpenSerial()
{
    //if( m_serialTerm ) return;
    CircuitWidget::self()->showSerialPortWidget( true );
    m_processor->setSerPort( true );
    //m_serialTerm = true;
}

void McuComponent::slotCloseSerial()
{
    CircuitWidget::self()->showSerialPortWidget( false );
    //SerialPortWidget::self()->close();
    m_processor->setSerPort( false );
    //m_serialTerm = false;
}

void McuComponent::slotOpenTerm()
{
    //if( m_serialTerm ) return;
    TerminalWidget::self()->setVisible( true );
    m_processor->setUsart( true );
    //m_serialTerm = true;
}

void McuComponent::slotCloseTerm()
{
    TerminalWidget::self()->setVisible( false );
    m_processor->setUsart( false );
    //m_serialTerm = false;
}

void McuComponent::slotLoad()
{
    const QString dir = m_lastFirmDir;
    QString fileName = QFileDialog::getOpenFileName( 0l, tr("Load Firmware"), dir,
                       tr("Hex Files (*.hex);;ELF Files (*.elf);;all files (*.*)"));
    if (fileName.isEmpty())
        return; // User cancels loading

    QDir circuitDir = QFileInfo(Circuit::self()->getFileName()).absoluteDir();
    load( circuitDir.relativeFilePath(fileName) );
}

void McuComponent::slotReload()
{
    if( m_processor->getLoadStatus() ) load( m_symbolFile );
    else QMessageBox::warning( 0, tr("No File:"), tr("No File to reload ") );
}

void McuComponent::load( QString fileName )
{
    /*bool haveTerm = m_serialTerm;
    if( m_serialTerm )
    {
        slotCloseTerm();
        slotCloseSerial();
    }*/

    QDir circuitDir = QFileInfo(Circuit::self()->getFileName()).absoluteDir();
    QString fileNameAbs = circuitDir.absoluteFilePath(fileName);
    QString cleanPathAbs = circuitDir.cleanPath(fileNameAbs);

    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim )  Simulator::self()->pauseSim();

    if( m_processor->loadFirmware( cleanPathAbs ) )
    {
        if( !m_attached ) attachPins();
        reset();
        
        m_symbolFile = fileName;
        m_lastFirmDir = cleanPathAbs;

        QSettings* settings = MainWindow::self()->settings();
        settings->setValue( "lastFirmDir", m_symbolFile );
    }
    else QMessageBox::warning( 0, tr("Error:"), tr("Could not load ")+ fileName );
    
    if( pauseSim ) Simulator::self()->runContinuous();
    //if( haveTerm ) slotOpenTerm();
}

void McuComponent::setProgram( QString pro )
{
    if( pro == "" ) return;
    m_symbolFile = pro;

    QDir circuitDir = QFileInfo(Circuit::self()->getFileName()).absoluteDir();
    QString fileNameAbs = circuitDir.absoluteFilePath(m_symbolFile);

    if( QFileInfo::exists( fileNameAbs )  // Load firmware at circuit load
     && !m_processor->getLoadStatus())
    {
        load( m_symbolFile );
    }
}

void McuComponent::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Package::paint( p, option, widget );
}

#include "moc_mcucomponent.cpp"
