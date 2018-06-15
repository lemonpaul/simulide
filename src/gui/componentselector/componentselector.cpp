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

#include <QDomDocument>

#include "componentselector.h"
#include "mainwindow.h"
#include "simuapi_apppath.h"

ComponentSelector* ComponentSelector::m_pSelf = 0l;

ComponentSelector::ComponentSelector( QWidget* parent )
                 : QTreeWidget( parent )
                 , m_pluginsdDialog( this )
                 , m_itemLibrary()
{
    m_pSelf = this;

    m_pluginsdDialog.setVisible( false );

    setDragEnabled(true);
    setDragDropMode( QAbstractItemView::DragOnly );
    //setAlternatingRowColors(true);
    setIndentation( 12 );
    setRootIsDecorated( true );
    setCursor( Qt::OpenHandCursor );
    headerItem()->setHidden( true );
    setIconSize( QSize( 32, 24));
    
    //QWhatsThis::add( this, QString::fromUtf8( gettext("Drag a Component and drop it into the Circuit." ) ) );

    LoadLibraryItems();
    //LoadCompSet();

    setContextMenuPolicy( Qt::CustomContextMenu );

    connect( this, SIGNAL(customContextMenuRequested(const QPoint&)),
             this, SLOT  (slotContextMenu(const QPoint&)));

    connect( this, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
             this, SLOT  (slotItemClicked(QTreeWidgetItem*, int)) );
}
ComponentSelector::~ComponentSelector()
{
}

void ComponentSelector::LoadLibraryItems()
{
    QList<LibraryItem*> itemList = m_itemLibrary.items();
    
    foreach( LibraryItem* libItem, itemList )
    {
        this->addItem( libItem );
    }
}

/*void ComponentSelector::LoadCompSet()
{
    QDir compSetDir = SIMUAPI_AppPath::self()->RODataFolder();

    if( compSetDir.exists() ) LoadCompSetAt( compSetDir );
}*/

void ComponentSelector::LoadCompSetAt( QDir compSetDir )
{
    compSetDir.setNameFilters( QStringList( "*.xml" ) );

    QStringList xmlList = compSetDir.entryList( QDir::Files );

    if( xmlList.isEmpty() ) return;                  // No comp sets to load

    qDebug() << "\n    Loading Component sets at:\n"<<compSetDir.absolutePath()<<"\n";

    foreach( QString compSetName, xmlList )
    {
        QString compSetFilePath = compSetDir.absoluteFilePath( compSetName );

        if( !compSetFilePath.isEmpty( ))  loadXml( compSetFilePath );
    }
    qDebug() << "\n";
}

void ComponentSelector::loadXml( const QString &setFile )
{
    QFile file( setFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          QMessageBox::warning(0, tr("Application"), tr("Cannot read file %1:\n%2.").arg(setFile).arg(file.errorString()));
          return;
    }
    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
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

        //LibraryItem* parent = m_itemLibrary.itemByName(category);

        LibraryItem* parent = m_itemLibrary.libraryItem(type);

        if( parent )
        {
            while( !node.isNull() )
            {
                element = node.toElement();

                QString icon = "";

                if( element.hasAttribute("icon") )
                {
                    QDir compSetDir( qApp->applicationDirPath() );
                    compSetDir.cd( "../share/simulide/data/images" );
                    //icon = QCoreApplication::applicationDirPath();
                    //icon.append("/data/images/");
                    //icon.append(element.attribute("icon"));
                    icon = compSetDir.absoluteFilePath(element.attribute("icon"));
                }
                QString name = element.attribute( "name" );

                addItem( name, category, icon, type );

                m_xmlFileList[ name ] = setFile;   // Save xml File used to create this item

                node = node.nextSibling();
            }
        }
        rNode = rNode.nextSibling();
    }
    QString compSetName = setFile.split( "/").last();

    qDebug() << "        Loaded Component set:           "<< compSetName;

    m_compSetUnique.append( compSetName );
}

QString ComponentSelector::getXmlFile( QString compName )
{
    return m_xmlFileList[ compName ];
}

void ComponentSelector::addLibraryItem( LibraryItem* libItem ) // Used By Plugins
{
    m_itemLibrary.addItem( libItem );
    this->addItem( libItem );
}

void ComponentSelector::addItem( LibraryItem* libItem )
{
    QString category = libItem->category();
    if( category != "")
    {
        QString icon = ":/"+libItem->iconfile();
        addItem( libItem->name(), category, icon, libItem->type() );
    }
}

void ComponentSelector::addItem( const QString &name, const QString &_category, const QString &icon, const QString &type )
{
    bool hidden = MainWindow::self()->settings()->value( name+"/hidden" ).toBool();

    QTreeWidgetItem* titulo = 0l;
    
    QStringList catPath = _category.split( "/" );
    
    QString category = catPath.takeLast();

    if( !m_categories.contains(category, Qt::CaseSensitive) )  // Create new Category
    {
        bool c_hidden =  MainWindow::self()->settings()->value( category+"/hidden" ).toBool();
        bool expanded = !MainWindow::self()->settings()->value( category+"/collapsed" ).toBool();

        m_categories.append(category);
        titulo = new QTreeWidgetItem(0);
        titulo->setFlags( QFlag(32) );
        QFont font = titulo->font(0);
        font.setPointSize(11);
        font.setWeight(75);
        titulo->setIcon( 0, QIcon(":/null-0.png") );
        titulo->setFont( 0, font );
        titulo->setText( 0, category );
        titulo->setTextColor( 0, QColor( 110, 95, 50 )/*QColor(255, 230, 200)*/ );
        titulo->setBackground( 0, QBrush(QColor(240, 235, 245)) );
        titulo->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
        
        if( !catPath.isEmpty() )
        {
            QString topCat = catPath.takeLast();
            
            QList<QTreeWidgetItem*> list = findItems( topCat, Qt::MatchExactly );
            
            if( !list.isEmpty() ) 
            {
                QTreeWidgetItem* topItem = list.takeLast();
                topItem->addChild( titulo );
            }
        }
        else addTopLevelItem( titulo );
        
        titulo->setExpanded( expanded );
        titulo->setHidden( c_hidden );
    }
    else                                                                // Find Category
    {
        QList<QTreeWidgetItem*> list = findItems(category, Qt::MatchExactly );

        if( list.length() > 0 ) titulo = list.first();
        else
        {
            for( int i=0; i<topLevelItemCount(); i++ )
            {
                QTreeWidgetItem* it = topLevelItem(i);

                for( int j=0; j<it->childCount(); j++ )
                {
                    if( it->child(j)->text(0) == category )
                    {
                        titulo = it->child(j);
                        break;
                    }
                }
                if( titulo ) break;
            }
        }
    }
    if( !titulo ) return;

    if( !m_categories.contains(name, Qt::CaseSensitive) )
        m_categories.append(name);

    QTreeWidgetItem* item =  new QTreeWidgetItem(0);
    QFont font = item->font(0);
    font.setPointSize(9);
    font.setWeight( QFont::Bold );
    item->setFont( 0, font );
    item->setText( 0, name );
    item->setFlags( QFlag(32) );
    item->setIcon( 0, QIcon(icon) );
    item->setData( 0, Qt::UserRole, type );

    titulo->addChild( item );
    item->setHidden( hidden );
}

/*void ComponentSelector::removeLibItem( LibraryItem* libItem )
{
    QString category = libItem->category();
    QString     name = libItem->name();

    QTreeWidgetItem* cat = 0l;

    for( int i=0; i<topLevelItemCount(); i++ )
    {
        cat = topLevelItem( i );
        if( cat->text(0) == category ) break;
    }
    if( cat == 0l ) return;

    for( int i=0; i<cat->childCount(); i++ )
    {
        if( cat->child( i )->text( 0 ) == name )
        {
            cat->takeChild( i );
            delete libItem;
            break;
        }
    }

}*/

void ComponentSelector::mouseReleaseEvent( QMouseEvent* event )
{
    setCursor( Qt::OpenHandCursor );
}

void ComponentSelector::slotItemClicked( QTreeWidgetItem* item, int column)
{
    Q_UNUSED( column );
    
    if( !item ) return;
    
    QString type = item->data(0, Qt::UserRole).toString();
    //m_lastItemClicked = type;
    
    if( type == "" ) return;
    
    QMimeData *mimeData = new QMimeData;
    
    mimeData->setText( item->text(0) );
    mimeData->setHtml( type );              // esto hay que revisarlo
    
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::CopyAction );
}

void ComponentSelector::slotContextMenu( const QPoint& point )
{
    QMenu menu;
    
    QAction* managePluginAction = menu.addAction( QIcon(":/fileopen.png"),tr("Manage Components") );
    connect( managePluginAction, SIGNAL(triggered()), this, SLOT(slotManageComponents()) );
    
    //QAction* installAction = menu.addAction( QIcon(":/fileopen.png"),tr("Install New Component") );
    //connect( installAction, SIGNAL(triggered()), this, SLOT(slotIstallItem()) );

    //menu->addSeparator();
    
    menu.exec( mapToGlobal(point) );
}

void ComponentSelector::slotManageComponents()
{
    m_pluginsdDialog.setPluginList();
    m_pluginsdDialog.setVisible( true );
}


/*void ComponentSelector::unistallPlugin( QString item )
{
    qDebug() << "    Unloading Plugin:" << item;
    
    QFile fIn( "data/plugins/"+item+"uninstall" );
    if( fIn.open( QFile::ReadOnly | QFile::Text ) ) // Read unistall file
    {
        QTextStream sIn(&fIn);
        while (!sIn.atEnd())                    // Remove installed files
        {
            QString pluginPath = sIn.readLine();
            if( pluginPath.contains("plugin") )
            {
                QString pluginName = pluginPath.split("/").last().split(".").first().remove("lib").remove("plugin").toUpper();
                MainWindow::self()->unLoadPugin( pluginName );
            }
            QFile file( pluginPath );
            file.remove();
        }
        fIn.close();
        fIn.remove();
        //reLoadItems();
        qDebug() << "        Plugin Unloaded Successfully:\t" << item << "\n";
    } 
    else 
    { 
        qDebug() << "ComponentSelector::slotUnistallItem:\n Error Opening UnInstall File\n";
        qDebug()<<fIn.errorString();
    }
}*/

/*void ComponentSelector::istallPlugin( QString item )
{
    //qDebug() << "ComponentSelector::istallItem" << item;

    QString dir = "plugins/"+item+"plugin";

    QFile fOut("data/plugins/"+item+"uninstall");      // Create unistall File
    if( fOut.open( QFile::WriteOnly | QFile::Text ) )
    {
        QTextStream fileList(&fOut);
    
        QDirIterator it(dir, QDirIterator::Subdirectories);
        while(it.hasNext()) 
        {
            QString origFile = it.next();
            if( it.fileInfo().isFile() )           // Copy Plugin Files to data/
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
        MainWindow::self()->loadPlugins();
        //reLoadItems();
    }
    else { qDebug() << "ComponentSelector::slotIstallItem:\n Error Opening Output File\n"; }
}*/

/*void ComponentSelector::reLoadItems()
{
    clear();
    m_categories.clear();
    m_itemLibrary.loadItems();
    //m_itemLibrary.loadPlugins();
    LoadLibraryItems();
    LoadCompSet();
}*/

#include "moc_componentselector.cpp"
