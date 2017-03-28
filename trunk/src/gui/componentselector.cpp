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

#include <QDomDocument>

#include "componentselector.h"
#include "itemlibrary.h"

ComponentSelector::ComponentSelector( QWidget* parent )
    : QTreeWidget( parent )
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    //setAlternatingRowColors(true);
    setIndentation(12);
    setRootIsDecorated(true);
    setCursor(Qt::OpenHandCursor);
    headerItem()->setHidden( true );
    setIconSize( QSize( 32, 24));
    
    //QWhatsThis::add( this, QString::fromUtf8( gettext("Drag a Component and drop it into the Circuit." ) ) );

    LoadLibraryItems();

    LoadCompSet();
    
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect( this, SIGNAL(customContextMenuRequested(const QPoint&)),
             this, SLOT  (slotContextMenu(const QPoint&)));

    connect( this, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
             this, SLOT  (slotItemClicked(QTreeWidgetItem*, int)) );
}
ComponentSelector::~ComponentSelector() { }

void ComponentSelector::LoadLibraryItems()
{
    QList<LibraryItem*> itemList = itemLibrary()->items();
    
    foreach( LibraryItem* libItem, itemList )
    {
        //if( !m_loaded.contains(libItem->name()) )
        {
            QString category = libItem->category();
            if( category != "")
            {
                QString icon = ":/"+libItem->iconfile();
                addItem( libItem->name(), category, icon, libItem->type() );
            }
            //m_loaded << libItem->name();
        }
    }
}

void ComponentSelector::LoadCompSet()
{
    // Load Component sets, located at applicationDirPath/data/*.xml
    QDir compSetDir( qApp->applicationDirPath() );

    compSetDir.cd( "data" );
    compSetDir.setNameFilters( QStringList( "*.xml" ) );

    qDebug() << "\n    Loading Component sets at:\n"<<compSetDir.absolutePath()<<"\n";

    foreach( QString compSetName, compSetDir.entryList( QDir::Files ) )
    {
        //if( !m_loaded.contains(compSetName) )
        {
            loadXml( compSetDir.absoluteFilePath(compSetName) );
            //m_loaded << compSetName;
            qDebug() << "        Loaded Component set:  "<< compSetName;
        }
    }
    qDebug() << "\n";
}

void ComponentSelector::loadXml( const QString &setFile )
{
    QFile file(setFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
          QMessageBox::warning(0, tr("Application"), tr("Cannot read file %1:\n%2.").arg(setFile).arg(file.errorString()));
          return;
    }
    QDomDocument domDoc;
    if (!domDoc.setContent(&file))
    {
         QMessageBox::warning(0, tr("Application"), tr("Cannot set file %1\nto DomDocument").arg(setFile));
         file.close();
         return;
    }
    file.close();

    QDomElement root  = domDoc.documentElement();
    QDomNode    rNode = root.firstChild();

    while( !rNode.isNull() )
    {
        QDomElement element = rNode.toElement();
        QDomNode    node    = element.firstChild();

        QString category = element.attribute( "category");
        QString type     = element.attribute( "type");

        while( !node.isNull() )
        {
            element = node.toElement();

            QString icon = "";

            if( element.hasAttribute("icon") )
            {
                icon = QCoreApplication::applicationDirPath();
                icon.append("/data/images/");
                icon.append(element.attribute("icon"));
            }
            addItem( element.attribute( "name" ), category, icon, type );

            node = node.nextSibling();
        }
        rNode = rNode.nextSibling();
    }
}

void ComponentSelector::addItem( const QString &caption, const QString &_category, const QString &icon, const QString &type )
{
    QTreeWidgetItem *item =  new QTreeWidgetItem(0);
    QFont font = item->font(0);
    font.setPointSize(9);
    font.setWeight( QFont::Bold );
    item->setFont( 0, font );
    item->setText( 0, caption );
    item->setFlags( QFlag(32) );
    item->setIcon( 0, QIcon(icon) );
    item->setData( 0, Qt::UserRole, type );

    QTreeWidgetItem *titulo = 0l;

    if ( !m_categories.contains(_category, Qt::CaseSensitive) )
    {
        m_categories.append(_category);
        titulo = new QTreeWidgetItem(0);
        titulo->setFlags( QFlag(32) );
        QFont font = titulo->font(0);
        font.setPointSize(11);
        font.setWeight(75);
        titulo->setIcon( 0, QIcon(":/null-0.png") );
        titulo->setFont( 0, font );
        titulo->setText( 0, _category );
        titulo->setTextColor( 0, QColor( 110, 95, 50 )/*QColor(255, 230, 200)*/ );
        titulo->setBackground( 0, QBrush(QColor(240, 235, 245)) );
        titulo->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
        addTopLevelItem( titulo );
        titulo->setExpanded(true);
    }
    else
    {
        QList<QTreeWidgetItem*> list = findItems(_category, Qt::MatchExactly );

        if ( list.length() > 0 ) titulo = list.first();
        else
        {
            for( int i=0; i<topLevelItemCount(); i++ )
            {
                QTreeWidgetItem *it = topLevelItem(i);

                for( int j=0; j < it->childCount(); j++ )
                {
                    if( it->child(j)->text(0) == _category)
                       titulo = it->child(j);
                }
            }
        }
    }
    m_categories.append(caption);

    titulo->addChild( item );
}

void ComponentSelector::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor( Qt::OpenHandCursor );
}


void ComponentSelector::slotItemClicked( QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    
    if (!item) return;
    
    QString type = item->data(0, Qt::UserRole).toString();
    m_lastItemClicked = type;
    
    if( type == "" ) return;
    
    QMimeData *mimeData = new QMimeData;
    
    mimeData->setText( item->text(0) );
    mimeData->setHtml( type );              // esto hay que revisarlo
    
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void ComponentSelector::slotContextMenu(const QPoint& point)
{
    QMenu menu;
    
    QAction* uninstallAction = menu.addAction( QIcon(":/fileopen.png"),tr("Uninstall ")+m_lastItemClicked );
    connect( uninstallAction, SIGNAL(triggered()), this, SLOT(slotUnistallItem()) );
    
    QAction* installAction = menu.addAction( QIcon(":/fileopen.png"),tr("Install New Component") );
    connect( installAction, SIGNAL(triggered()), this, SLOT(slotIstallItem()) );

    //menu->addSeparator();
    
    menu.exec(mapToGlobal(point));
}

void ComponentSelector::slotUnistallItem()
{
    qDebug() << "slotUnistallItem"<<m_lastItemClicked;
    qDebug() << "data/plugins/"+m_lastItemClicked.toUpper()+"uninstall";
    
    QFile fIn( "data/plugins/"+m_lastItemClicked.toUpper()+"uninstall" );
    if( fIn.open( QFile::ReadOnly | QFile::Text ) ) 
    {
        QTextStream sIn(&fIn);
        while (!sIn.atEnd())
        {
            QFile file( sIn.readLine() );
            file.remove();
        }
        fIn.close();
        fIn.remove();
        clear();
        m_categories.clear();
        itemLibrary()->loadItems();
        itemLibrary()->loadPlugins();
        LoadLibraryItems();
        LoadCompSet();
    } 
    else 
    { 
        qDebug() << "ComponentSelector::slotUnistallItem:\n Error Opening Output File\n"; 
        qDebug()<<fIn.errorString();
    }
    
}

void ComponentSelector::slotIstallItem()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                             "plugins",
                                             QFileDialog::ShowDirsOnly
                                           | QFileDialog::DontResolveSymlinks
                                           | QFileDialog::HideNameFilterDetails );

    QString dir2 = dir;
    QFile fOut("data/plugins/"+dir2.remove("plugin").split("/").last().toUpper()+"uninstall");
    if( fOut.open( QFile::WriteOnly | QFile::Text ) ) 
    {
        QTextStream fileList(&fOut);
    
        QDirIterator it(dir, QDirIterator::Subdirectories);
        while(it.hasNext()) 
        {
            QString origFile = it.next();
            if( it.fileInfo().isFile() )
            {
                QString destFile = origFile;
                destFile = destFile.remove(dir+"/");
                
                QFileInfo fi( destFile );
                QDir destDir = fi.dir();

                destDir.mkpath( destDir.absolutePath() );
                
                QFile::copy( origFile, destFile );
                
                fileList << destFile << '\n';
                //qDebug() << destFile;
            }
        }
        fOut.close();
        clear();
        m_categories.clear();
        itemLibrary()->loadItems();
        itemLibrary()->loadPlugins();
        LoadLibraryItems();
        LoadCompSet();
    }
    else { qDebug() << "ComponentSelector::slotIstallItem:\n Error Opening Output File\n"; }
}

#include "moc_componentselector.cpp"
