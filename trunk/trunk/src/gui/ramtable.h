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

#ifndef RAMTABLE_H
#define RAMTABLE_H

#include <QtGui>

class BaseProcessor;

class RamTable : public QTableWidget
{
    Q_OBJECT
    public:
        RamTable( BaseProcessor *processor );
        ~RamTable();

        //void setProcessor( BaseProcessor *processor );

    public slots:
        void updateValues();

    private slots:
        void addToWatch(QTableWidgetItem*);

    private:
        BaseProcessor *m_processor;

        QHash<int, QString> watchList;

        int m_numRegs;
        QTimer *m_ramTimer;
};

#endif // RAMTABLE_H
