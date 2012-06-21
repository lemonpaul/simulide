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

#include "ramtable.h"
#include "baseprocessor.h"
#include "utils.h"

RamTable::RamTable( BaseProcessor *processor )
    : QTableWidget( 20, 3 )
{
    m_processor = processor;
    m_numRegs = 20;
    setColumnWidth(0, 50);
    setColumnWidth(1, 35);
    setColumnWidth(2, 75);

    QTableWidgetItem *it;

    for( int row=0; row<m_numRegs; row++ )
    {
        it = new QTableWidgetItem(0);
        it->setText( tr("---") );
        setVerticalHeaderItem( row, it );
        for( int col=0; col<4; col++ )
        {
            QTableWidgetItem *it = new QTableWidgetItem(0);
            if( col>0 ) it->setFlags( Qt::ItemIsEnabled );
            setItem( row, col, it );
        }
        item( row, 1 )->setText("---");
        item( row, 2 )->setText("---- ----");
    }

    it = new QTableWidgetItem(0);
    it->setText( tr("Reg.") );
    setHorizontalHeaderItem( 0, it );

    it = new QTableWidgetItem(0);
    it->setText( tr("Dec") );
    setHorizontalHeaderItem( 1, it );

    it = new QTableWidgetItem(0);
    it->setText( tr("Binary") );
    setHorizontalHeaderItem( 2, it );

    m_ramTimer = new QTimer(this);
    connect( m_ramTimer, SIGNAL(timeout()), this, SLOT(updateValues()) );

    connect( this, SIGNAL(itemChanged(QTableWidgetItem*)  ), this, SLOT(addToWatch(QTableWidgetItem*)) );

    m_ramTimer->start( 200 );

    show();
}
RamTable::~RamTable(){}

void RamTable::updateValues()
{
    foreach( int _row, watchList.keys() )
    {
        QString name = watchList[_row];

        int value = m_processor->getRamValue( name );

        if( value >= 0 )
        {
            item( _row, 1 )->setData( 0, value );
            item( _row, 2 )->setData( 0, decToBase(value, 2, 8) );
        }
    }
}

void RamTable::addToWatch( QTableWidgetItem *it )
{
    if( column(it) != 0 ) return;
    int _row = row(it);
    QString name = it->text().remove(" ").remove("\t").toLower();

    if( name.isEmpty() )
    {
        watchList.remove(_row);
        verticalHeaderItem( _row )->setText("---");

        item( _row, 1 )->setText("---");
        item( _row, 2 )->setText("---- ----");
    }
    else
    {
        watchList[_row] = name;
        verticalHeaderItem( _row )->setData( 0, m_processor->getRegAddress(name) );
    }
}

/*void RamTable::setProcessor( BaseProcessor *processor )
{
    m_processor = processor;
}*/

#include "moc_ramtable.cpp"

