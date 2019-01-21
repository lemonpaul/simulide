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

#include "subcircuit.h"
#include "componentselector.h"
#include "circuit.h"
#include "utils.h"
#include "pin.h"
#include "connector.h"
#include "mainwindow.h"
#include "itemlibrary.h"
#include "simuapi_apppath.h"

#include "ledsmd.h"
#include "e-bcdto7s.h"
#include "e-bcdtodec.h"
#include "e-bincounter.h"
#include "e-bjt.h"
#include "e-capacitor.h"
#include "e-dectobcd.h"
#include "e-demux.h"
#include "e-diode.h"
#include "e-flipflopd.h"
#include "e-flipflopjk.h"
#include "e-fulladder.h"
#include "e-function.h"
#include "e-gate_or.h"
#include "e-gate_xor.h"
#include "e-gate_xor.h"
#include "e-inbus.h"
#include "e-latch_d.h"
#include "e-logic_device.h"
#include "e-mux.h"
#include "e-mosfet.h"
#include "e-op_amp.h"
#include "e-outbus.h"
#include "e-resistor.h"
#include "e-shiftreg.h"
#include "e-source.h"
#include "e-volt_reg.h"

Component* SubCircuit::construct( QObject* parent, QString type, QString id )
{ 
    SubCircuit* subCircuit = new SubCircuit( parent, type,  id ); 
    if( m_error > 0 )
    {
        Circuit::self()->compList()->removeOne( subCircuit );
        subCircuit->deleteLater();
        subCircuit = 0l;
        m_error = 0;
    }
    return subCircuit;
}

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
          : Chip( parent, type, id )
{
    m_numItems = 0;

    initChip();
}
SubCircuit::~SubCircuit()
{
}

void SubCircuit::initChip()
{
    QString compName = m_id.split("-").first(); // for example: "atmega328-1" to: "atmega328"

    m_dataFile = ComponentSelector::self()->getXmlFile( compName );

    QFile file( m_dataFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          MessageBoxNB( "SubCircuit::initChip",
                    tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()));
          m_error = 21;
          return;
    }
    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
         MessageBoxNB( "SubCircuit::initChip",
                   tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile));
         file.close();
         m_error = 22;
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
                QDir dataDir(  m_dataFile );
                dataDir.cdUp();             // Indeed it doesn't cd, just take out file name
                
                QString package =element.attribute( "package" );
                if( package == "" )
                {
                    m_error = 201;
                    return;
                }
                m_dataFile = dataDir.filePath( package );

                Chip::initChip();
                if( m_error != 0 ) return;

                m_dataFile = dataDir.filePath( element.attribute( "subcircuit" ) );
                break;
            }
            node = node.nextSibling();
        }
        rNode = rNode.nextSibling();
    }
    initSubcircuit();
    if( m_error != 0 ) return;
}

void SubCircuit::initSubcircuit()
{
    //QString dfPath = SIMUAPI_AppPath::self()->availableDataFilePath(m_dataFile);
    //qDebug() << "SubCircuit::initSubcircuit datafile: " << dfPath;
    QFile file( m_dataFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          MessageBoxNB( "SubCircuit::initSubcircuit",
                    tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()));
          m_error = 23;
          return;
    }
    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
         MessageBoxNB( "SubCircuit::initSubcircuit",
                   tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile));
         file.close();
         m_error = 24;
         return;
    }
    file.close();

    QDomElement root  = domDoc.documentElement();
    QDomNode    rNode = root.firstChild();

    if( root.tagName()!="subcircuit" )
    {
        MessageBoxNB( "SubCircuit::initSubcircuit",
                  tr("Error reading Subcircuit file: %1\n") .arg(m_dataFile));
        m_error = 25;
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
            QString id = m_id+"-"+type+"-"+QString::number(m_numItems);
            m_numItems++;

            //qDebug() << "\nSubCircuit::initSubcircuit" << id;

            eElement* ecomponent = 0l;

            if     ( type == "eResistor" )  ecomponent = new eResistor( id.toStdString() );
            else if( type == "eCapacitor" ) ecomponent = new eCapacitor( id.toStdString() );
            else if( type == "eDiode" )     
            {
                eDiode* ediode = new eDiode( id.toStdString() );
                if( element.hasAttribute("threshold") )
                {
                    ediode->setThreshold( element.attribute( "threshold" ).toDouble() );
                }
                ecomponent = ediode;
            }
            else if( type == "eAndGate" )
            {
                int numInputs = 2;
                if( element.hasAttribute("numInputs") ) numInputs  = element.attribute( "numInputs" ).toInt();
                eGate* egate = new eGate( id.toStdString(), numInputs );
                egate->createPins( numInputs, 1 );
                ecomponent = egate;
            }
            else if( type == "eBuffer" )
            {
                eGate* egate = new eGate( id.toStdString(), 1 );
                egate->createPins( 1, 1 );
                ecomponent = egate;
                
                if( element.attribute( "tristate" ) == "true" )
                    egate->setTristate( true );
            }
            else if( type == "eOrGate" )
            {
                int numInputs = 2;
                if( element.hasAttribute("numInputs") ) numInputs  = element.attribute( "numInputs" ).toInt();
                eOrGate* egate = new eOrGate( id.toStdString(), numInputs );
                egate->createPins( numInputs, 1 );
                ecomponent = egate;
            }
            else if( type == "eXorGate" )
            {
                int numInputs = 2;
                if( element.hasAttribute("numInputs") ) numInputs  = element.attribute( "numInputs" ).toInt();
                eXorGate* egate = new eXorGate( id.toStdString(), numInputs );
                egate->createPins( numInputs, 1 );
                ecomponent = egate;
            }
            else if( type == "eFunction" )
            {
                eFunction* efunction = new eFunction( id.toStdString() );
                ecomponent = efunction;
                
                int inputs  = 0;
                int outputs = 0;
                if( element.hasAttribute("numInputs") )  inputs  = element.attribute( "numInputs" ).toInt();
                if( element.hasAttribute("numOutputs") ) outputs = element.attribute( "numOutputs" ).toInt();
                efunction->createPins( inputs, outputs );
                
                if( element.hasAttribute("functions") ) efunction->setFunctions( element.attribute( "functions" ) );
            }
            else if( type.startsWith( "eLatchD" ) )
            {
                int channels = 1;
                if( element.hasAttribute("channels") ) channels = element.attribute( "channels" ).toInt();
                eLatchD* elatchd = new eLatchD( id.toStdString() );
                elatchd->setNumChannels( channels );
                //elatchd->createInEnablePin();
                ecomponent = elatchd;
            }
            else if( type == "eBinCounter" )
            {
                int maxValue = 1;
                if( element.hasAttribute("maxValue") ) maxValue  = element.attribute( "maxValue" ).toInt();
                eBinCounter* ecounter = new eBinCounter( id.toStdString() );
                ecounter->setTopValue( maxValue );
                ecounter->createPins();
                ecomponent = ecounter;
            }
            else if( type == "eFullAdder" )
            {
                eFullAdder* efulladder = new eFullAdder( id.toStdString() );
                efulladder->createPins();
                ecomponent = efulladder;
            }
            else if( type == "eFlipFlopD" )
            {
                eFlipFlopD* eFFD = new eFlipFlopD( id.toStdString() );
                eFFD->createPins();
                ecomponent = eFFD;
                
                bool srInv = true;
                if( element.hasAttribute("sRInverted" ) )
                {
                    if( element.attribute( "sRInverted" ) == "false" ) srInv = false;
                }
                eFFD->setSrInv( srInv );
            }
            else if( type == "eFlipFlopJK" )
            {
                eFlipFlopJK* eFFJK = new eFlipFlopJK( id.toStdString() );
                eFFJK->createPins();
                ecomponent = eFFJK;
                
                bool srInv = true;
                if( element.hasAttribute("sRInverted" ) )
                {
                    if( element.attribute( "sRInverted" ) == "false" ) srInv = false;
                }
                eFFJK->setSrInv( srInv );
            }
            else if( type == "eShiftReg" )  
            {
                int latchClk = 0;
                int serOut   = 0;
                if( element.hasAttribute("latchClock") ) latchClk = element.attribute( "latchClock" ).toInt();
                if( element.hasAttribute("serialOut") )  serOut   = element.attribute( "serialOut" ).toInt();
                ecomponent = new eShiftReg( id.toStdString(), latchClk, serOut );
            }
            else if( type == "eMux" )
            {
                eMux* emux = new eMux( id.toStdString() );
                emux->createPins();
                ecomponent = emux;
            }
            else if( type == "eDemux" )
            {
                eDemux* edemux = new eDemux( id.toStdString() );
                edemux->createPins();
                ecomponent = edemux;
            }
            else if( type == "eBcdTo7S" )
            {
                eBcdTo7S* ebcdto7s = new eBcdTo7S( id.toStdString() );
                ebcdto7s->createPins();
                ecomponent = ebcdto7s;
            }
            else if( type == "eBcdToDec" )
            {
                eBcdToDec* ebcdtodec = new eBcdToDec( id.toStdString() );
                ebcdtodec->createPins();
                ecomponent = ebcdtodec;
            }
            else if( type == "eDecToBcd" )
            {
                eDecToBcd* edectobcd = new eDecToBcd( id.toStdString() );
                edectobcd->createPins();
                ecomponent = edectobcd;
            }
            else if( type == "eInBus" )
            {
                int numbits = 8;
                if( element.hasAttribute("numBits") ) numbits = element.attribute( "numBits" ).toInt();
                double addr = pow( 2, numbits )-1;
                double vref = addr;
                if( element.hasAttribute("vref") ) vref = element.attribute( "vref" ).toDouble();
                eInBus* einbus = new eInBus( id.toStdString() );
                einbus->setMaxAddr( addr );
                einbus->setMaxVolt( vref );
                einbus->setNumOuts( numbits );
                einbus->setNumInps( 1 );
                ecomponent = einbus;
            }
            else if( type == "eOutBus" )
            {
                int numbits = 8;
                if( element.hasAttribute("numBits") ) numbits = element.attribute( "numBits" ).toInt();
                double addr = pow( 2, numbits )-1;
                double vref = addr;
                if( element.hasAttribute("vref") ) vref = element.attribute( "vref" ).toDouble();
                eOutBus* eoutbus = new eOutBus( id.toStdString() );
                eoutbus->setMaxAddr( addr );
                eoutbus->setMaxVolt( vref );
                eoutbus->setNumInps( numbits );
                eoutbus->setNumOuts( 1 );
                ecomponent = eoutbus;
            }
            else if( type == "eRail" )  
            {
                double volt = 5;
                if( element.hasAttribute("voltage") ) volt = element.attribute( "voltage" ).toDouble();
                eSource* esource = new eSource( id.toStdString(), 0l );
                esource->createPin();
                esource->setVoltHigh( volt );
                esource->setOut( true );
                ecomponent = esource;
            }
            else if( type == "eMosfet" )
            {
                double threshold = 3;
                double rDSon     = 1;
                if( element.hasAttribute("threshold") ) threshold = element.attribute( "threshold" ).toDouble();
                if( element.hasAttribute("rDSon") )     rDSon = element.attribute( "rDSon" ).toDouble();
                eMosfet* emosfet = new eMosfet( id.toStdString() );
                emosfet->setThreshold( threshold );
                emosfet->setRDSon( rDSon );
                if( element.hasAttribute("pChannel") )
                {
                    if( element.attribute( "pChannel" ) == "true" ) emosfet->setPchannel( true ); 
                }
                if( element.hasAttribute("Depletion") )
                {
                    if( element.attribute( "Depletion" ) == "true" ) emosfet->setDepletion( true );
                }
                ecomponent = emosfet;
            }
            else if( type == "eBJT" )
            {
                double threshold = 0.7;
                double gain     = 100;
                if( element.hasAttribute("threshold") ) threshold = element.attribute( "threshold" ).toDouble();
                if( element.hasAttribute("gain") )      gain      = element.attribute( "gain" ).toDouble();
                eBJT* ebjt = new eBJT( id.toStdString() );
                ebjt->setBEthr( threshold );
                ebjt->setGain( gain );
                if( element.hasAttribute("pNP") )
                {
                    if( element.attribute( "pNP" ) == "true" ) { ebjt->setPnp( true ); }
                }
                ecomponent = ebjt;
            }
            else if( type == "eVoltReg" )
            {
                double volts = 1.2;
                if( element.hasAttribute("Volts") ) volts = element.attribute( "Volts" ).toDouble();
                eVoltReg* evoltreg = new eVoltReg( id.toStdString() );
                evoltreg->setNumEpins(3);
                evoltreg->setVRef( volts );
                ecomponent = evoltreg;
            }
            else if( type == "eopAmp" )
            {
                double gain = 1000;
                if( element.hasAttribute("Gain") ) gain = element.attribute( "Gain" ).toDouble();
                bool powerPins = false;
                if( element.hasAttribute("Power_Pins" ) )
                {
                    if( element.attribute( "Power_Pins" ) == "true" ) powerPins = true;
                }
                eOpAmp* eopamp = new eOpAmp( id.toStdString() );
                eopamp->setGain( gain );
                eopamp->setPowerPins( powerPins );
                ecomponent = eopamp;
            }
            else if( type == "LedSmd" )
            {
                int width = 8;
                int height = 8;
                if( element.hasAttribute("width") )  width  = element.attribute( "width" ).toDouble();
                if( element.hasAttribute("height") ) height = element.attribute( "height" ).toDouble();
                ecomponent = new LedSmd( this, "LEDSMD", id, QRectF( 0, 0, width, height )  );
            }
            
            if( ecomponent )
            {
                m_elementList.append( ecomponent );
                ecomponent->initEpins();

                // Get properties
                if( element.hasAttribute("maxcurrent") )
                {
                    eLed* eled = static_cast<eLed*>(ecomponent);
                    eled->setMaxCurrent( element.attribute( "maxcurrent" ).toDouble() );
                }
                if( element.hasAttribute("capacitance") )
                {
                    eCapacitor* ecapacitor = static_cast<eCapacitor*>(ecomponent);
                    ecapacitor->setCap( element.attribute( "capacitance" ).toDouble() );
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
                if( element.hasAttribute("tristate") )
                {
                    if( element.attribute( "tristate" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->createOutEnablePin();
                    }
                }
                if( element.hasAttribute("open_Collector") )
                {
                    if( element.attribute( "open_Collector" ) == "true" )
                    {
                        eGate* egate = static_cast<eGate*>(ecomponent);
                        egate->setOpenCol( true );
                    }
                }
                if( element.hasAttribute("inputEnable") )
                {
                    if( element.attribute( "inputEnable" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->createInEnablePin();
                    }
                }
                if( element.hasAttribute("clocked") )
                {
                    if( element.attribute( "clocked" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->createClockPin();
                    }
                }
                if( element.hasAttribute("clockInverted") )
                {
                    if( element.attribute( "clockInverted" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->setClockInv( true );
                    }
                }
                if( element.hasAttribute("inverted") )
                {
                    if( element.attribute( "inverted" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->setInverted( true );
                    }
                }
                if( element.hasAttribute("invertInputs") )
                {
                    if( element.attribute( "invertInputs" ) == "true" )
                    {
                        eLogicDevice* elogicdevice = static_cast<eLogicDevice*>(ecomponent);
                        elogicdevice->setInvertInps( true );
                    }
                }

                // Get the connections list
                QStringList connectionList = element.attribute( "connections" ).split(" ");
                //qDebug() << "connectionList" << connectionList;

                foreach( QString connection, connectionList )   // Get the connection points for each connection
                {
                    if( !(connection.contains("-")) ) continue;
                    QStringList pins = connection.split("-");

                    QString pin = pins.first();

                    //qDebug() << "SubCircuit::initSubcircuit connecting:"<<element.attribute( "itemtype" ) << pins.first() << pins.last();
                    ePin* epin = 0l;

                    if( pin.startsWith("componentPin"))
                    {
                        int pinNum = pin.remove("componentPin").toInt();
                        epin = ecomponent->getEpin( pinNum );
                    }
                    else epin = ecomponent->getEpin( pin );

                    if( epin ) connectEpin( epin, pins.last() );   // Connect points (ePin to Pin or eNode)
                    else 
                    {
                        qDebug() << "SubCircuit::initSubcircuit Error connecting:" << pin << pins.last();
                        m_error = 31;
                        return;
                    }
                }
            }
            else 
            {
                qDebug() << "SubCircuit::initSubcircuit Error creating: " << id;
                m_error = 32;
                return;
            }
        }
        rNode = rNode.nextSibling();
    }
}

void SubCircuit::connectEpin( ePin* epin, QString connetTo )
{
    if( connetTo.startsWith("eNode") )
    {
        int eNodeNum = connetTo.remove("eNode").toInt();
        epin->setEnode( m_internal_eNode.at(eNodeNum) );
        //qDebug() << "SubCircuit::connectEpin to eNode " << connetTo << eNodeNum;
    }
    else if( connetTo.startsWith("packagePin") )
    {
        int pinNum = connetTo.remove("packagePin").toInt()-1;
        //qDebug() << "SubCircuit::connectEpin to Pin " << connetTo << pinNum;
        m_pinConections[pinNum].append( epin );
    }
}

void SubCircuit::initialize()
{
    for( int i=0; i<m_numpins; i++ )        // get eNode for each package pin
    {                                       // and assing to connected subcircuit ePins
        eNode* enod = m_ePin[i]->getEnode();
        
        if( !enod )
        {
            QList<ePin*> ePinList = m_pinConections[i];
            int size = ePinList.size();
            
            if( size > 1 )
            {
                enod = ePinList.first()->getEnode();
                if( !enod )
                {
                    QString eNodeid = m_id;
                    eNodeid.append( "-eNode_I_" ).append( QString::number(i));
                    enod = new eNode( eNodeid );
                }
            }
        }
        foreach( ePin* epin, m_pinConections[i] ) epin->setEnode(enod);
    }
}

void SubCircuit::remove()
{
    for( int i=0; i<m_numpins; i++ )
    {
        foreach( ePin* epin, m_pinConections[i] ) epin->setEnode(0l);
    }
    foreach( eNode* node, m_internal_eNode )
    {
        Simulator::self()->remFromEnodeList( node, true );
        //delete node;
    }
    foreach( eElement* el, m_elementList )
    {
        //qDebug() << "deleting" << QString::fromStdString( el->getId() );
        delete el;
    }

    Chip::remove();
}

void SubCircuit::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu *menu = new QMenu();

    //menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

#include "moc_subcircuit.cpp"
