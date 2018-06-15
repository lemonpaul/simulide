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

#include "ramtable.h"
#include "baseprocessor.h"
#include "utils.h"

RamTable::RamTable( BaseProcessor *processor )
    : QTableWidget( 40, 3 )
{
    m_processor = processor;
    m_numRegs = 40;
    
    setColumnWidth(0, 100);
    setColumnWidth(1, 35);
    setColumnWidth(2, 90);
    
    int row_heigh = 23;

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
        QFont font = item( 0, 0 )->font();
        font.setBold(true);
        font.setPointSize( 10 );
        for( int col=0; col<3; col++ ) item( row, col )->setFont( font );
        
        item( row, 1 )->setText("---");
        item( row, 2 )->setText("---- ----");
        
        setRowHeight(row, row_heigh);
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
    
    QAction *loadVarSet = new QAction( QIcon(":/fileopen.png"),"loadVarSet", this);
    connect( loadVarSet, SIGNAL(triggered()), this, SLOT(loadVarSet()) );
    
    QAction *saveVarSet = new QAction( QIcon(":/remove.png"),"saveVarSet", this);
    connect( saveVarSet, SIGNAL(triggered()), this, SLOT(saveVarSet()) );
    
    horizontalHeader()->addAction( loadVarSet );
    horizontalHeader()->addAction( saveVarSet );
    horizontalHeader()->setContextMenuPolicy(Qt::ActionsContextMenu);

    m_ramTimer = new QTimer(this);
    connect( m_ramTimer, SIGNAL(timeout()), 
             this,       SLOT(updateValues()) );

    connect( this, SIGNAL(itemChanged(QTableWidgetItem*)  ), 
             this, SLOT(addToWatch(QTableWidgetItem*)) );

    m_ramTimer->start( 200 );

    show();
}
RamTable::~RamTable(){}

void RamTable::loadVarSet()
{
    const QString dir = m_processor->getFileName();
    //QCoreApplication::applicationDirPath()+"/data/varset";
    QString fileName = QFileDialog::getOpenFileName( this, tr("Load VarSet"), dir, tr("VarSets (*.vst);;All files (*.*)"));
    
    if( !fileName.isEmpty() )
    {
        QStringList lines = fileToStringList( fileName, "RamTable::loadVarSet" );
        int row = -1;
        foreach( QString line, lines )
        {
            line.remove( " " ).remove("\t");
            QStringList words = line.split( "=" );
            QString name = words.first();
            
            if( !words.last().isEmpty() )
            {
                int address = words.last().toInt();
                m_processor->addWatchVar( name, address, "uint8" );
            }
            if( row >= 0 ) item( row, 0 )->setText( name );
            row++;
            if( row >= m_numRegs ) break;
        }
    }
}
void RamTable::saveVarSet()
{
    const QString dir = m_processor->getFileName();
    //QCoreApplication::applicationDirPath()+"/data/varset";
    
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save VarSet"), dir,
                                                 tr("VarSets (*.vst);;All files (*.*)"));
    if( !fileName.isEmpty() )
    {
        if( !fileName.endsWith(".vst") ) fileName.append(".vst");

        QFile file( fileName );

        if( !file.open(QFile::WriteOnly | QFile::Text) )
        {
              QMessageBox::warning(0l, tr("RamTable::saveVarSet"),
              tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
              return;
        }

        QTextStream out(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        
        for( int row=0; row<m_numRegs; row++ )
        {
            QString name = item( row, 0 )->text();
            out << name << "\n";
        }
        QApplication::restoreOverrideCursor();
    }
}


void RamTable::updateValues()
{
    if( m_processor )
    {
        foreach( int _row, watchList.keys() )
        {
            m_currentRow = _row;
            QString name = watchList[_row];
            
            m_processor->updateRamValue( name );

            //int value = m_processor->getRamValue( name );

            /*if( value >= 0 )
            {
                item( _row, 1 )->setData( 0, value );
                item( _row, 2 )->setData( 0, decToBase(value, 2, 8) );
            }*/
        }
    }
}

void RamTable::setItemValue( int col, QString value  )
{
    item( m_currentRow, col )->setData( 0, value );
}

void RamTable::setItemValue( int col, float value  )
{
    item( m_currentRow, col )->setData( 0, value );
}

void RamTable::setItemValue( int col, int value  )
{
    item( m_currentRow, col )->setData( 0, value );
}

void RamTable::addToWatch( QTableWidgetItem* it )
{
    if( column(it) != 0 ) return;
    int _row = row(it);
    QString name = it->text().remove(" ").remove("\t").remove("*");//.toLower();

    if( name.isEmpty() )
    {
        watchList.remove(_row);
        verticalHeaderItem( _row )->setText("---");

        item( _row, 1 )->setText("---");
        item( _row, 2 )->setText("---- ----");
    }
    else
    {
        int value = m_processor->getRegAddress(name);
        if( value >= 0 )
        {
            watchList[_row] = name;
            verticalHeaderItem( _row )->setData( 0, value );
        }
    }
}

/*void RamTable::setProcessor( BaseProcessor *processor )
{
    m_processor = processor;
}*/

#include "moc_ramtable.cpp"

