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

#ifndef PICPLUGIN_H
#define PICPLUGIN_H

#include "itemlibrary.h"
#include "appiface.h"


class PicPlugin : public QObject, AppIface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.SimulIDE.Plugin.AppIface/1.0")
    Q_INTERFACES( AppIface )

    public:
            void initialize();
            void terminate();
};
#endif

