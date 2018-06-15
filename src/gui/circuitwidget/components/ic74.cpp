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

#include "ic74.h"
#include  "circuit.h"

LibraryItem* Ic74::libraryItem()
{
    return new LibraryItem(
        tr("IC 74"),
        tr("Logic"),
        "ic2.png",
        "Ic74",
        Ic74::construct );
}

Component* Ic74::construct( QObject* parent, QString type, QString id )
{ 
    Ic74* ic74 = new Ic74( parent, type,  id ); 
    if( m_error > 0 )
    {
        Circuit::self()->compList()->removeOne( ic74 );
        ic74->deleteLater();
        ic74 = 0l;
        m_error = 0;
    }
    return ic74;
}


Ic74::Ic74( QObject* parent, QString type, QString id )
    : SubCircuit( parent, type, id )
{
    m_dataFile = "ic74.xml";
    
    initPackage();
}
Ic74::~Ic74()
{
}
#include "moc_ic74.cpp"
