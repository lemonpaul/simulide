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

#ifndef ITEMLIBRARY_H
#define ITEMLIBRARY_H

#include "component.h"

class QStringList;
class ItemLibrary;
class LibraryItem;

inline ItemLibrary* itemLibrary();

/**
While the program is running, only one instance of this class is created.
You can get it by calling itemLibrary()
@short Holds the list of Items
@author David Saxton
*/
class ItemLibrary
{
	public:
		~ItemLibrary();

		/**
		 * Returns a list of items in the library
		 */
        QList<LibraryItem*> items() { return m_items; }


		/**
		 * @return the LibraryItem for the item with the given type (id) const.
		 */
        LibraryItem*  libraryItem( QString type ) const;

	
    protected:
		ItemLibrary();
	
        QList<LibraryItem*> m_items;
		
        friend ItemLibrary*  itemLibrary();
};

inline ItemLibrary* itemLibrary()
{
	// are we really sure we aren't calling new over and over again? 
    static ItemLibrary* _itemLibrary = new ItemLibrary();
	return _itemLibrary;
}


class LibraryItem
{
    public:
        LibraryItem( const QString &name, const QString &category, const QString &iconName,
                     const QString type, createItemPtr createItem );
        
        ~LibraryItem();

        QString name()     const { return m_name; }
        QString category() const { return m_category; }
        QString iconfile() const { return m_iconfile; }
        QString type()     const { return m_type; }

        createItemPtr createItemFnPtr() const { return createItem; }

    private:
        QString m_name;
        QString m_category;
        QString m_iconfile;
        QString m_type;

        createItemPtr createItem;
};

#endif
