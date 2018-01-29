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
 
#include "itemlibrary.h"
#include "appiface.h"

//BEGIN Item includes
#include "amperimeter.h"
#include "adc.h"
#include "bcdtodec.h"
#include "bincounter.h"
#include "buffer.h"
#include "capacitor.h"
#include "clock.h"
#include "dac.h"
#include "dectobcd.h"
#include "demux.h"
#include "diode.h"
#include "flipflopd.h"
#include "flipflopjk.h"
#include "fulladder.h"
#include "gate_and.h"
#include "gate_or.h"
#include "gate_xor.h"
#include "ground.h"
#include "hd44780.h"
#include "i2c.h"
#include "ic74.h"
#include "inbus.h"
#include "inductor.h"
#include "ks0108.h"
#include "latchd.h"
#include "led.h"
#include "ledbar.h"
#include "logicinput.h"
#include "mosfet.h"
#include "mux.h"
#include "op_amp.h"
#include "outbus.h"
#include "pcd8544.h"
#include "probe.h"
#include "potentiometer.h"
#include "push.h"
#include "rail.h"
#include "relay-spst.h"
#include "resistor.h"
#include "resistordip.h"
#include "servo.h"
#include "sevensegment.h"
#include "sevensegment_bcd.h"
#include "shiftreg.h"
#include "stepper.h"
#include "subcircuit.h"
#include "switch.h"
#include "textcomponent.h"
#include "voltimeter.h"
#include "volt_reg.h"
#include "voltsource.h"
//END Item includes

#include "simuapi_apppath.h"

ItemLibrary* ItemLibrary::m_pSelf = 0l;

ItemLibrary::ItemLibrary()
{
    m_pSelf = this;

    loadItems();
    //loadPlugins();
}
ItemLibrary::~ItemLibrary(){}

void ItemLibrary::loadItems()
{
    m_items.clear();
    // Meters
    addItem( Probe::libraryItem() );
    addItem( Voltimeter::libraryItem() );
    addItem( Amperimeter::libraryItem() );
    // Sources
    addItem( LogicInput::libraryItem() );
    addItem( Clock::libraryItem() );
    addItem( VoltSource::libraryItem() );
    addItem( Rail::libraryItem() );
    addItem( Ground::libraryItem() );
    // Switches
    addItem( Switch::libraryItem() );
    addItem( Push::libraryItem() );
    addItem( RelaySPST::libraryItem() );
    // Passive
    addItem( Potentiometer::libraryItem() );
    addItem( Resistor::libraryItem() );
    addItem( ResistorDip::libraryItem() );
    addItem( Capacitor::libraryItem() );
    addItem( Inductor::libraryItem() );
    // Active
    addItem( Diode::libraryItem() );
    addItem( VoltReg::libraryItem() );
    addItem( OpAmp::libraryItem() );
    addItem( Mosfet::libraryItem() );
    // Outputs
    addItem( Led::libraryItem() );
    addItem( LedBar::libraryItem() );
    addItem( SevenSegment::libraryItem() );
    addItem( Hd44780::libraryItem() );
    addItem( Pcd8544::libraryItem() );
    addItem( Ks0108::libraryItem() );
    addItem( Stepper::libraryItem() );
    addItem( Servo::libraryItem() );
    // Logic
    //addItem( I2C::libraryItem() );
    addItem( SevenSegmentBCD::libraryItem() );
    addItem( Buffer::libraryItem() );
    addItem( AndGate::libraryItem() );
    addItem( OrGate::libraryItem() );
    addItem( XorGate::libraryItem() );
    addItem( FlipFlopD::libraryItem() );
    addItem( FlipFlopJK::libraryItem() );
    addItem( BinCounter::libraryItem() );
    addItem( FullAdder::libraryItem() );
    addItem( LatchD::libraryItem() );
    addItem( ShiftReg::libraryItem() );
    addItem( Mux::libraryItem() );
    addItem( Demux::libraryItem() );
    addItem( BcdToDec::libraryItem() );
    addItem( DecToBcd::libraryItem() );
    addItem( ADC::libraryItem() );
    addItem( DAC::libraryItem() );
    addItem( OutBus::libraryItem() );
    addItem( InBus::libraryItem() );
    // Subcircuits
    //addItem( Ic74::libraryItem() );
    addItem( SubCircuit::libraryItem() );
    // Other
    addItem( TextComponent::libraryItem() );
}

void ItemLibrary::addItem( LibraryItem* item )
{
    if (!item)
        return;
    m_items.append(item);
}

/*void ItemLibrary::loadPlugins()
{
    m_plugins.clear();
    QDir pluginsDir( qApp->applicationDirPath() );

    pluginsDir.cd( "data/plugins" );

    qDebug() << "\n    Loading App plugins at:\n"<<pluginsDir.absolutePath()<<"\n";

    QString pluginName = "*plugin.*";
    pluginsDir.setNameFilters( QStringList(pluginName) );

    foreach( pluginName, pluginsDir.entryList( QDir::Files ) )
    {
        QPluginLoader pluginLoader( pluginsDir.absoluteFilePath( pluginName ) );
        QObject *plugin = pluginLoader.instance();

        pluginName = pluginName.split(".").first().remove("lib").remove("plugin").toUpper();

        if( plugin )
        {
            AppIface* item = qobject_cast<AppIface*>( plugin );

            item->initialize();
            if( item && !(m_plugins.contains(pluginName)) )
            {
                m_plugins.append(pluginName);
                qDebug()<< "        Loaded plugin\t" << pluginName;
            }
        }
        else
        {
            QString errorMsg = pluginLoader.errorString();
            qDebug()<< "        " << pluginName << "\tplugin FAILED: " << errorMsg;

            if( errorMsg.contains( "libQt5SerialPort" ) )
                errorMsg = " Qt5SerialPort is not installed in your system\n\n    Mcu SerialPort will not work\n    Just Install libQt5SerialPort package\n    To have Mcu Serial Port Working";

            QMessageBox::warning( 0,"App Plugin Error:", errorMsg );
        }
    }
    qDebug() << "\n";
}*/


const QList<LibraryItem *> ItemLibrary::items() const
{
    return m_items;
}

LibraryItem *ItemLibrary::itemByName(const QString name) const
{
    foreach( LibraryItem* item, m_items )
    {
        if( item->name() == name ) return item;
    }
    return 0l;
}

LibraryItem* ItemLibrary::libraryItem(const QString type ) const
{
    foreach( LibraryItem* item, m_items )
    {
        if( item->type() == type ) return item;
    }
    return 0l;
}


// CLASS LIBRAYITEM *********************************************************

LibraryItem::LibraryItem( const QString &name,
                          const QString &category,
                          const QString &iconName,
                          const QString type,
                          createItemPtr _createItem )
{
    m_name      = name;
    m_category  = category;
    m_iconfile  = iconName;
    m_type      = type;
    createItem  = _createItem;
}
LibraryItem::~LibraryItem() { }

