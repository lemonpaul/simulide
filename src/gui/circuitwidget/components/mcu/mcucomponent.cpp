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

#include <qstringlist.h>

#include "mcucomponent.h"
#include "mcucomponentpin.h"
#include "connector.h"
#include "mainwindow.h"

McuComponent::McuComponent( QObject* parent, QString type, QString id )
    : Package( parent, type, id )
{
    m_processor = 0l;
    m_symbolFile = "";
    m_device = "";

    m_labelx = -38;
    m_labely = -137;

    m_color = QColor( 50, 50, 70 );

    QSettings settings("PicLinux", "SimulIDE");
    m_lastFirmDir = settings.value("lastFirmDir").toString();
    
    if( m_lastFirmDir.isEmpty() )
        m_lastFirmDir = QCoreApplication::applicationDirPath()+"/examples/mega48_adc/mega48_adc.hex";
}
McuComponent::~McuComponent() {}

void McuComponent::initPackage()
{
    QString compName = m_id.split("-").first(); // for example: "atmega328-1" to: "atmega328"
    label->setText( compName );

    //qDebug() << "McuComponent::initPackage datafile: " << m_dataFile;

    QFile file( QCoreApplication::applicationDirPath()+"/"+m_dataFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          QMessageBox::warning(0, "McuComponent::initPackage",
          tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()));
          return;
    }

    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
         QMessageBox::warning(0, "McuComponent::initPackage",
         tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile));
         file.close();
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

        while( !node.isNull() )                    // Find the "package" 
        {
            QDomElement element = node.toElement();
            if( element.attribute("name")==compName )
            {
                package = element.attribute( "package" );
                m_dataFile = m_dataFile.replace( m_dataFile.split("/").last(), package.append(".package") );

                m_device = element.attribute( "device" );
                m_processor->setDevice( m_device );

                break;
            }
            node = node.nextSibling();
        }
        rNode = rNode.nextSibling();
    }
    if( m_device != "" ) Package::initPackage();
    else qDebug() << compName << "ERROR!! McuComponent::initPackage Package not Found: " << package;

}

void McuComponent::reset()
{
    for ( int i = 0; i < m_pinList.size(); i++ ) // Reset pins states
        m_pinList[i]->resetOutput();
}

void McuComponent::terminate()
{
    qDebug() <<"McuComponent::terminate "<<m_id<<"\n";
    m_processor->terminate();
    for( int i=0; i<m_numpins; i++ ) m_pinList[i]->terminate();
    reset();
}

void McuComponent::remove()
{
    foreach( McuComponentPin* mcupin, m_pinList )
    {
        Pin* pin = mcupin->pin(); //static_cast<Pin*>(mcupin->pin());
        if( pin->connector() ) pin->connector()->remove();
        //delete mcupin;
    }
    terminate();
    m_pinList.clear();

    Component::remove();
}

void McuComponent::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu* menu = new QMenu();
    QAction* loadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Load firmware") );
    connect( loadAction, SIGNAL(triggered()), this, SLOT(slotLoad()) );

    QAction* reloadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Reload firmware") );
    connect( reloadAction, SIGNAL(triggered()), this, SLOT(slotReload()) );

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

void McuComponent::slotLoad()
{
    const QString dir = m_lastFirmDir;
    QString fileName = QFileDialog::getOpenFileName( 0l, tr("Load Firmware"), dir,
                       tr("Hex Files (*.hex);;all files (*.*)"));

    load( fileName );
}

void McuComponent::slotReload()
{
    if( m_processor->getLoadStatus() ) load( m_symbolFile );
    else QMessageBox::warning( 0, tr("No File:"), tr("No File to reload ") );
}

void McuComponent::load( QString fileName )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim )  Simulator::self()->pauseSim();

    if( m_processor->loadFirmware( fileName ) )
    {
        attachPins();
        reset();

        m_symbolFile = fileName;

        QSettings settings( "PicLinux", "SimulIDE" );   //*********  !!!!!!!!!!!!!!!!!  ****************
        settings.setValue( "lastFirmDir", m_symbolFile );
    }
    if( pauseSim ) Simulator::self()->runContinuous();
}

void McuComponent::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Package::paint( p, option, widget );
}

#include "moc_mcucomponent.cpp"
