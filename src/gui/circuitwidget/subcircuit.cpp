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

#include "subcircuit.h"
#include "pin.h"
#include "connector.h"
#include "mainwindow.h"
#include "itemlibrary.h"
#include "e-shiftreg.h"
#include "e-resistor.h"
#include "e-capacitor.h"
#include "e-gate_or.h"
#include "e-gate_xor.h"
#include "e-logic_device.h"
#include "ledsmd.h"

Component* SubCircuit::construct( QObject* parent, QString type, QString id )
{ return new SubCircuit( parent, type,  id ); }

LibraryItem* SubCircuit::libraryItem()
{
    return new LibraryItem(
        tr("Subcircuit"),
        tr(""),         // Not dispalyed
        "",
        "Subcircuit",
        SubCircuit::construct );
}

SubCircuit::SubCircuit( QObject* parent, QString type, QString id )
    : Package( parent, type, id )
{
    //m_labelx = -38;
    //m_labely = -137;

    //m_color = QColor( 50, 50, 70 );

    m_numItems = 0;

    m_dataFile = "data/subcircuits.xml";

    initPackage();
    initSubcircuit();
}
SubCircuit::~SubCircuit() {}

void SubCircuit::initPackage()
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

    QDomElement root  = domDoc.documentElement();
    QDomNode    rNode = root.firstChild();

    while( !rNode.isNull() )
    {
        QDomElement element = rNode.toElement();
        QDomNode    node    = element.firstChild();

        while( !node.isNull() )         // Find the "package", for example 628A is package: 627A, Same pins
        {
            QDomElement element = node.toElement();
            if( element.attribute("name")==compName )
            {
                QString package = element.attribute( "package" );
                m_dataFile = m_dataFile.replace( m_dataFile.split("/").last(), package.append(".package") );
                //qDebug() << m_dataFile;
                /*if( m_device != "" )*/ Package::initPackage();
                //else qDebug() << compName << "ERROR!! McuComponent::initPackage Package not Found: " << package

                QString subcirc = element.attribute( "subcircuit" );
                m_dataFile = m_dataFile.replace( m_dataFile.split("/").last(), subcirc.append(".subcircuit") );
                break;
            }
            node = node.nextSibling();
        }
        rNode = rNode.nextSibling();
    }
}

void SubCircuit::initSubcircuit()
{
    QFile file( QCoreApplication::applicationDirPath()+"/"+m_dataFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          QMessageBox::warning(0, "SubCircuit::initSubcircuit",
          tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()));
          return;
    }
    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
         QMessageBox::warning(0, "SubCircuit::initSubcircuit",
         tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile));
         file.close();
         return;
    }
    file.close();

    QDomElement root  = domDoc.documentElement();
    QDomNode    rNode = root.firstChild();

    if( root.tagName()!="subcircuit" )
    {
        qDebug() << " SubCircuit::initSubcircuit Error reading Subcircuit file: " << m_dataFile;
        return;
    }
    if( root.hasAttribute("enodes") )                    // Create eNodes & add to enodList
    {
        int enodes = root.attribute( "enodes" ).toInt(); // Number of eNodes to be created
        for( int i=0; i<enodes; i++ )
        {
            QString eNodeid = m_id;
            eNodeid.append( "-eNode_" ).append( QString::number(i));
            m_internal_eNode.append( new eNode(eNodeid) );
        }
    }
    m_pinConections.resize( m_numpins );

    while( !rNode.isNull() )
    {
        QDomElement element = rNode.toElement();
        QString     tagName = element.tagName();

        if( tagName=="item" )
        {
            QString type = element.attribute( "itemtype"  );
            QString id = m_id;
            id.append("-").append(type).append("-").append( QString::number(m_numItems) );
            m_numItems++;

            eElement* ecomponent;

            if     ( type == "eShiftReg" )  ecomponent = new eShiftReg( id.toStdString() );
            else if( type == "eResistor" )  ecomponent = new eResistor( id.toStdString() );
            else if( type == "eCapacitor" ) ecomponent = new eCapacitor( id.toStdString() );
            else if( type == "eDiode" )     ecomponent = new eDiode( id.toStdString() );
            else if( type == "eBuffer"
                  || type == "eInverter"
                  || type == "eAndGate"
                  || type == "eNandGate" )
            {
                //qDebug() << "SubCircuit::initSubcircuit , type: " << type;
                int numInputs = 2;
                if( element.hasAttribute("numInputs") ) numInputs  = element.attribute( "numInputs" ).toInt();
                eGate* egate = new eGate( id.toStdString(), numInputs );
                ecomponent = egate;

                if( type == "eInverter" || type == "eNandGate" ) egate->setInverted( true );

            }
            else if( type == "LedSmd" )
            {
                int width = 8;
                int height = 8;
                if( element.hasAttribute("width") )  width  = element.attribute( "width" ).toDouble();
                if( element.hasAttribute("height") ) height = element.attribute( "height" ).toDouble();

                ecomponent = new LedSmd( this, "LEDSMD", id, QRectF( 0, 0, width, height )  );
            }
            ecomponent->initEpins();

            if( element.hasAttribute("maxcurrent") )
            {
                eLed* eled = static_cast<eLed*>(ecomponent);
                eled->setMaxCurrent( element.attribute( "maxcurrent" ).toDouble() );
            }
            if( element.hasAttribute("threshold") )
            {
                eDiode* ediode = static_cast<eDiode*>(ecomponent);
                ediode->setThreshold( element.attribute( "threshold" ).toDouble() );
            }
            if( element.hasAttribute("uf") )
            {
                eCapacitor* ecapacitor = static_cast<eCapacitor*>(ecomponent);
                ecapacitor->setuF( element.attribute( "uf" ).toDouble() );
            }
            if( element.hasAttribute("resistance") )
            {
                eResistor* eresistor = static_cast<eResistor*>(ecomponent);
                eresistor->setRes( element.attribute( "resistance" ).toDouble() );
            }
            if( element.hasAttribute("outHighV") )
            {
                eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                elogicdevice->setOutHighV( element.attribute( "outHighV" ).toDouble() );
            }
            if( element.hasAttribute("outLowV") )
            {
                eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                elogicdevice->setOutLowV( element.attribute( "outLowV" ).toDouble() );
            }
            if( element.hasAttribute("inputImp") )
            {
                eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                elogicdevice->setInputImp( element.attribute( "inputImp" ).toDouble() );
            }
            if( element.hasAttribute("outImp") )
            {
                eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                elogicdevice->setOutImp( element.attribute( "outImp" ).toDouble() );
            }

            // Get the connections list
            QStringList connectionList = element.attribute( "connections" ).split(" ");

            foreach( QString connection, connectionList )   // Get the connection points for each connection
            {
                QStringList pins = connection.split("-");
                int pin = pins.first().remove("ePin").toInt();
                connectEpin( ecomponent->getEpin(pin), pins.last() );   // Connect points (ePin to Pin or eNode)
                //qDebug() << pin << pins.last();
            }
        }
        rNode = rNode.nextSibling();
    }
}

void SubCircuit::connectEpin( ePin* epin, QString connetTo )
{

    if( connetTo.startsWith("eNode") )
    {
        //qDebug() << "SubCircuit::connectEpin" << connetTo;
        int eNodeNum = connetTo.remove("eNode").toInt();
        epin->setEnode( m_internal_eNode.at(eNodeNum) );
    }
    else if( connetTo.startsWith("Pin") )
    {
        int pinNum = connetTo.remove("Pin").toInt();
        m_pinConections[pinNum].append( epin );
        //qDebug() << "connecting to Pin" << connetTo << pinNum;
    }
}

/*void SubCircuit::setRes( eResistor* ecomponent )
{

}*/

void SubCircuit::initialize()
{
    for( int i=0; i<m_numpins; i++ )        // get eNode for each package pin
    {                                       // and assing to connected subcircuit ePins
        eNode* enod = m_ePin[i]->getEnode();

        foreach( ePin* epin, m_pinConections[i] ) epin->setEnode(enod);
    }
}

void SubCircuit::remove()
{
    /*foreach( SubCircuitPin *mcupin, m_pinList )
    {
        Pin* pin = mcupin->pin(); //static_cast<Pin*>(mcupin->pin());
        if( pin->isConnected() ) pin->connector()->remove();
    }
    m_pinList.clear();*/

    Component::remove();
}

void SubCircuit::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu *menu = new QMenu();
    /*QAction *loadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Load firmware") );
    connect( loadAction, SIGNAL(triggered()), this, SLOT(slotLoad()) );

    QAction *reloadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Reload firmware") );
    connect( reloadAction, SIGNAL(triggered()), this, SLOT(slotReload()) );*/
    
    /*QAction *ramAction = menu->addAction( QIcon(":/fileopen.png"),"View Ram" );
    connect( ramAction, SIGNAL(triggered()), this, SLOT(viewRam()) );*/

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

/*void SubCircuit::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    p->drawRoundedRect( boundingRect(), 1, 1);
}*/

#include "moc_subcircuit.cpp"
