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

//#include <dlfcn.h>

#include "itemlibrary.h"
//#include "compIface.h"
//#include "logic_comb_iface.h"

//BEGIN Item includes
//#include "buffer.h"
#include "capacitor.h"
#include "diode.h"
//#include "FlipFlopJK.h"
//#include "FlipFlopD.h"
//#include "gate_and.h"
//#include "gate_nor.h"
//#include "gate_or.h"
//#include "gate_xor.h"
#include "ground.h"
//#include "inverter.h"
#include "led.h"
#include "logicinput.h"
//#include "neuron.h"
//#include "socket.h"
#include "probe.h"
//#include "rampa.h"
#include "resistor.h"
#include "sevensegment.h"
#include "voltsource.h"
#include "avrcomponent.h"
#include "avrboard.h"
#include "subcircuit.h"
//END Item includes


ItemLibrary::ItemLibrary()
{
    // Sources
    m_items.append( LogicInput::libraryItem() );
    m_items.append( VoltSource::libraryItem() );
    m_items.append( Ground::libraryItem() );
    // Passive
    m_items.append( Resistor::libraryItem() );
    m_items.append( Capacitor::libraryItem() );
    m_items.append( Diode::libraryItem() );
    // Outputs
    m_items.append( Probe::libraryItem() );
    //m_items.append( Socket::libraryItem() );
    m_items.append( Led::libraryItem() );
    m_items.append( SevenSegment::libraryItem() );
    //Boards
    m_items.append( AVRBoard::libraryItem() );
    // Micro
    m_items.append( AVRComponent::libraryItem() );
    //Subcircuits
    m_items.append( SubCircuit::libraryItem() );

}
ItemLibrary::~ItemLibrary(){}

LibraryItem * ItemLibrary::libraryItem( QString type ) const
{
    foreach ( LibraryItem* item, m_items )
    {
        if ( item->type() == type )
            return item;
    }
    return 0l;
}


// CLASS LIBRAYITEM *********************************************************

LibraryItem::LibraryItem( const QString &name, const QString &category,
                          const QString &iconName, const QString type, createItemPtr _createItem )
{
    m_name      = name;
    m_category  = category;
    m_iconfile  = iconName;
    m_type      = type;
    createItem  = _createItem;
}
LibraryItem::~LibraryItem() { }

