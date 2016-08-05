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

#ifndef ComponentSelector_H
#define ComponentSelector_H

#include <QtWidgets>


class ComponentSelector : public QTreeWidget
{
    Q_OBJECT
    
    public:
        ComponentSelector( QWidget* parent );
        ~ComponentSelector();

        void addItem( const QString &caption,
                      const QString &category,
                      const QString &icon,
                      const QString &type );

        void loadXml( const QString &setFile );

        void mouseReleaseEvent(QMouseEvent*);

    private slots:
        void slotItemClicked( QTreeWidgetItem* item, int column );

    private:
    
        QStringList m_categories;
};

#endif
