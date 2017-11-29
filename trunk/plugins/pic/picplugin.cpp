/***************************************************************************
 *   Copyright (C) 2017 by santiago González                               *
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

#include "componentselector.h"
#include "picplugin.h"
#include "piccomponent.h"
#include "simuapi_apppath.h"

void PicPlugin::initialize()
{
    ComponentSelector::self()->addLibraryItem( PICComponent::libraryItem() );
    //QDir compSetDir( qApp->applicationDirPath() );
    //compSetDir.cd( "../share/simulide/data" );
    //ComponentSelector::self()->loadXml( compSetDir.absoluteFilePath("pics.xml") );
    ComponentSelector::self()->loadXml( SIMUAPI_AppPath::self()->availableDataFilePath("pics.xml") );
}

void PicPlugin::terminate()
{
    /* ComponentSelector::removeLibItem removed!
    ComponentSelector::self()->removeLibItem( PICComponent::libraryItem() );
    */
}

#include "moc_picplugin.cpp"


