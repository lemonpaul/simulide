/***************************************************************************
 *   Copyright (C) 2018 by Pavel Lamonov                                   *
 *   leamonpaul@yandex.ru                                                  *
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

#include "filebrowser.h"
#include "circuitwidget.h"
#include "circuit.h"
#include "mainwindow.h"

FileBrowser*  FileBrowser::m_pSelf = 0l;

FileBrowser::FileBrowser( QWidget *parent ) 
           : QTreeView( parent )
{
    m_pSelf = this;
    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setRootPath(QDir::rootPath());
    m_currentPath = m_fileSystemModel->rootPath();
    setModel(m_fileSystemModel);
    setRootIndex(m_fileSystemModel->index(QDir::rootPath()));
}

FileBrowser::~FileBrowser() { }

void FileBrowser::setRoot()
{
    QModelIndex currentDir = m_fileSystemModel->index(m_currentPath);
    setRootIndex(currentDir);
}

void FileBrowser::resetRoot()
{
    setRootIndex(m_fileSystemModel->index(QDir::rootPath()));
}

void FileBrowser::resetRootToHome()
{
    setRootIndex(m_fileSystemModel->index(QDir::homePath()));
}

void FileBrowser::openFile()
{
    emit openFileWithEditor(m_currentPath);
}

void FileBrowser::setPath(QString path)
{
    setCurrentIndex(m_fileSystemModel->index(path));
}

void FileBrowser::contextMenuEvent(QContextMenuEvent* event)
{
    QTreeView::contextMenuEvent( event );

    if( !event->isAccepted() )
    {
        QPoint eventPos = event->globalPos();
        QModelIndex eventIndex = indexAt( event->pos() );
        
        m_currentPath = m_fileSystemModel->filePath( eventIndex );

        QMenu menu;
        
        if (m_fileSystemModel->isDir(eventIndex))
        {
            QAction* changeRootAction = menu.addAction(QIcon(":/setroot.png"),"Set directory as root");
            connect( changeRootAction, SIGNAL( triggered()), this, SLOT(setRoot()) );
        }
        else
        {
            QAction* openWithEditor = menu.addAction(QIcon(":/open.png"),"Open in editor");
            connect( openWithEditor, SIGNAL( triggered()), this, SLOT(openFile()) );
            menu.addSeparator();
        }
        
        QAction* restoreRootAction = menu.addAction(QIcon(":/reset.png"),"Restore root as /");
        connect( restoreRootAction, SIGNAL( triggered()), this, SLOT(resetRoot()) );
        
        QAction* restoreRootAsHomeAction = menu.addAction(QIcon(":/reset.png"),"Restore root as ~");
        connect( restoreRootAsHomeAction, SIGNAL( triggered()), this, SLOT(resetRootToHome()) );

        menu.exec( eventPos );
    }
}

void FileBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_fileSystemModel->isDir(currentIndex()))
        QTreeView::mouseDoubleClickEvent( event );
    else
    {
        QString fileName = m_fileSystemModel->filePath(currentIndex());
        if( !fileName.isEmpty() && fileName.endsWith(".simu") )
        {
            CircuitWidget::self()->newCircuit();
            Circuit::self()->loadCircuit( fileName );
       
            MainWindow::self()->setTitle(fileName.split("/").last());
            MainWindow::self()->settings()->setValue( "lastCircDir", fileName );
        }
    }
}

void FileBrowser::keyPressEvent( QKeyEvent *event )
{
    bool isEnter = ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return));
    if (!m_fileSystemModel->isDir(currentIndex()) && isEnter)
    {
        QString fileName = m_fileSystemModel->filePath(currentIndex());
        if( !fileName.isEmpty() && fileName.endsWith(".simu") )
        {
            CircuitWidget::self()->newCircuit();
            Circuit::self()->loadCircuit( fileName );
       
            MainWindow::self()->setTitle(fileName.split("/").last());
            MainWindow::self()->settings()->setValue( "lastCircDir", fileName );
        }
    }
    else
        QTreeView::keyPressEvent( event );
}

#include  "moc_filebrowser.cpp"
