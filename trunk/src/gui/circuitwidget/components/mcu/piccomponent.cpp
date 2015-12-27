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

#ifndef NO_PIC

#include "piccomponent.h"
#include "piccomponentpin.h"
#include "gpsimprocessor.h"
#include "itemlibrary.h"


Component* PICComponent::construct( QObject* parent, QString type, QString id )
{ return new PICComponent( parent, type,  id ); }

LibraryItem* PICComponent::libraryItem()
{
    return new LibraryItem(
        tr("PIC"),
        tr("Micro"),
        "ic2.png",
        "PIC",
    PICComponent::construct );
}

PICComponent::PICComponent( QObject* parent, QString type, QString id )
    : McuComponent( parent, type, id )
{
    m_dataFile = "data/pics.xml";
    m_processor = GpsimProcessor::self();

    if( m_id.startsWith("PIC") ) m_id.replace( "PIC", "pic16f876" );

    initPackage();
}
PICComponent::~PICComponent() { }

void PICComponent::attachPins()
{
    GpsimProcessor* ap = dynamic_cast<GpsimProcessor*>( m_processor );
    pic_processor* cpu = ap->getCpu();

    for( int i=0; i < m_numpins; i++ )
    {
        PICComponentPin* pin = dynamic_cast<PICComponentPin*>( m_pinList[i] );
        pin->attach( cpu );
    }
}

void PICComponent::addPin( QString id, QString type, QString label, int pos, int xpos, int ypos, int angle )
{
    m_pinList.append( new PICComponentPin( this, id, type, label, pos, xpos, ypos, angle ) );
}

#include "moc_piccomponent.cpp"
#endif