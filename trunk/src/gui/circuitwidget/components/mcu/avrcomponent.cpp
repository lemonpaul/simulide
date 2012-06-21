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

#include "avrcomponent.h"
#include "avrcomponentpin.h"
#include "avrprocessor.h"
#include "itemlibrary.h"

Component* AVRComponent::construct( QObject* parent, QString type, QString id )
{ return new AVRComponent( parent, type,  id ); }

LibraryItem* AVRComponent::libraryItem()
{	
	return new LibraryItem(
        tr("AVR"),
        tr("Micro"),
        "ic2.png",
        "AVR",
		AVRComponent::construct );
}

AVRComponent::AVRComponent( QObject* parent, QString type, QString id )
    : McuComponent( parent, type, id )
{
    m_dataFile = "data/avrs.xml";
    m_processor = AvrProcessor::self();

    if( m_id.startsWith("AVR") ) m_id.replace( "AVR", "atmega328" );

    /*connect( m_processor, SIGNAL(chipReset()),
             this,        SLOT  (reset()) );*/

    initPackage();
}
AVRComponent::~AVRComponent() { }

void AVRComponent::attachPins()
{
    AvrProcessor *ap = dynamic_cast<AvrProcessor*>( m_processor );
    avr_t *cpu = ap->getCpu();

    for( int i = 0; i < m_numpins; i++ )
    {
        AVRComponentPin *pin = dynamic_cast<AVRComponentPin*>( m_pinList[i] );
        pin->attach( cpu );
    }
}

void AVRComponent::addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
{
    m_pinList.append( new AVRComponentPin( this, id, type, label, pos, xpos, ypos, angle ) );
}

#include "moc_avrcomponent.cpp"
