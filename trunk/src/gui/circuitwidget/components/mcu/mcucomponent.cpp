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
    : Component( parent, type, id )
{
    m_processor = 0l;
    m_symbolFile = "";

    m_labelx = -38;
    m_labely = -137;

    m_color = QColor( 50, 50, 70 );

    QSettings settings("PicLinux", "SimulIDE");
    m_lastFirmDir = settings.value("lastFirmDir").toString();
    if( m_lastFirmDir.isEmpty() )
        m_lastFirmDir = QCoreApplication::applicationDirPath()+"/examples/mega48_adc/mega48_adc.hex";
}
McuComponent::~McuComponent() { /*if( m_processor ) m_processor->terminate();*/ }

void McuComponent::initPackage()
{
    QString compName = m_id.split("-").first(); // for example: "atmega328-1" to: "atmega328"
    label->setText( compName );

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

    QDomElement root    = domDoc.documentElement();
    QDomElement element = root.toElement();
    QDomNode    node    = root.firstChild();

    QString type;

    while( !node.isNull() )         // Find the "type", for example 628A is type: 627A, Same pins
    {
        element = node.toElement();
        if( /*(element.tagName()=="mcu") &&*/ (element.attribute("name")==compName) )
            type = element.attribute( "type" );

        node = node.nextSibling();
    }
    node = root.firstChild();

    while( !node.isNull() )         // Find the node that contais pin description for "type"
    {
        element = node.toElement();
        if( /*(element.tagName()=="mcu") &&*/ (element.attribute("name")==type) )
        {
            m_numpins = element.attribute( "pins" ).toInt();
            m_device  = element.attribute( "device" );

            int width  = element.attribute( "width" ).toInt();  //64;
            int height = element.attribute( "height" ).toInt();  //16 + 8*m_numpins;

            m_processor->setDevice( m_device );

            m_area = QRect( -(width/2)*8, -(height/2)*8, 8*width, 8*height );
            label->setPos( m_area.x(), m_area.y()-20);

            node = node.firstChild();

            int chipPos = 0;

            while( !node.isNull() )
            {
                element = node.toElement();
                if( element.tagName() == "pin" )
                {
                    QString label = element.attribute( "label" );
                    QString type  = element.attribute( "type" );
                    QString id    = element.attribute( "id" );

                    int pos   = element.attribute( "pos" ).toInt();
                    int angle = element.attribute( "angle" ).toInt();

                    int xpos = 0;
                    int ypos = 0;

                    if( angle == 180 )      // Left
                    {
                        xpos = -(width/2)*8-8;
                        ypos = -(height/2)*8+8*pos;
                    }
                    else if( angle == 270 ) // Top
                    {
                        xpos = -(width/2)*8+8*pos;
                        ypos = -(height/2)*8-8;
                    }
                    else if( angle == 0 )   // Right
                    {
                        xpos =  (width/2)*8+8;
                        ypos = -(height/2)*8+8*pos;
                    }
                    else if( angle == 90 )  // Bottom
                    {
                        xpos = -(width/2)*8+8*pos;
                        ypos =  (height/2)*8+8;
                    }

                    chipPos++;
                    addPin( id, type, label, chipPos, xpos, ypos, angle );
                }
                node = node.nextSibling();
            }
        }
        node = node.nextSibling();
    }
}

/*void McuComponent::inStateChanged( int pin )
{
    //if( m_processor->getLoadStatus() ) m_pinList[pin-1]->changed();
}*/

void McuComponent::reset()
{
    //qDebug()<<"McuComponent::reset"<<m_id<<"loadStatus"<<m_processor->getLoadStatus();
    for ( int i = 0; i < m_pinList.size(); i++ ) // Reset pins states
        m_pinList[i]->resetOutput();
}

void McuComponent::terminate()
{
    qDebug() <<"McuComponent::terminate "<<m_id<<"\n";
    m_processor->terminate();
    for( int i = 0; i < m_numpins; i++ ) m_pinList[i]->terminate();
    reset();
    /*while( !m_pinList.isEmpty() ) delete m_pinList.takeFirst();
    initPackage();*/
}


void McuComponent::remove()
{
    foreach( McuComponentPin *mcupin, m_pinList )
    {
        Pin* pin = mcupin->pin(); //static_cast<Pin*>(mcupin->pin());
        if( pin->connector() ) pin->connector()->remove();
    }
    terminate();
    m_pinList.clear();

    Component::remove();
}

void McuComponent::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu *menu = new QMenu();
    QAction *loadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Load firmware") );
    connect( loadAction, SIGNAL(triggered()), this, SLOT(slotLoad()) );

    QAction *reloadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Reload firmware") );
    connect( reloadAction, SIGNAL(triggered()), this, SLOT(slotReload()) );
    
    /*QAction *ramAction = menu->addAction( QIcon(":/fileopen.png"),"View Ram" );
    connect( ramAction, SIGNAL(triggered()), this, SLOT(viewRam()) );*/

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

/*bool McuComponent::attachDevice()
{
    QString newDevice = m_processor->getDevice();

    if( newDevice != m_device ) // device type error
    {
        QMessageBox::warning( 0, tr("Device Type Error"),
        tr("ERROR: The file Is for %1.\nCannot load to %2.")
        .arg(newDevice).arg(m_device) );

        m_processor->terminate();
        return false;
    }
    attachPins();
    reset();
    connect( m_processor, SIGNAL( terminated()), this, SLOT( terminate()) );
    return true;
}*/

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
    if( m_processor->loadFirmware( fileName ) /*&& attachDevice()*/ )
    {
        attachPins();
        reset();

        m_symbolFile = fileName;

        QSettings settings( "PicLinux", "SimulIDE" );   //*********  !!!!!!!!!!!!!!!!!  ****************
        settings.setValue( "lastFirmDir", m_symbolFile );
        //MainWindow::self()->powerCircOff();
    }
}

void McuComponent::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    p->drawRoundedRect( boundingRect(), 1, 1);

    p->setPen( QColor( 170, 170, 150 ) );
    p->drawArc( -4, boundingRect().y()-4, 8, 8, 0, -2880 /* -16*180 */ );
}

#include "moc_mcucomponent.cpp"
