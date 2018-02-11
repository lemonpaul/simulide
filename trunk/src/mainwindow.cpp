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

#include "mainwindow.h"
#include "appiface.h"
#include "circuit.h"
#include "utils.h"
#include "simuapi_apppath.h"


MainWindow* MainWindow::m_pSelf = 0l;

MainWindow::MainWindow()
          : QMainWindow()
          , m_settings( "SimulIDE", "SimulIDE-"+QString(APP_VERSION) )
{
    setWindowIcon( QIcon(":/simulide.png") );
    m_pSelf   = this;
    m_circuit = 0l;
    m_version = "SimulIDE-"+QString(APP_VERSION);
    

    createWidgets();
    readSettings();
    
    loadPlugins();

    QString appPath = QCoreApplication::applicationDirPath();

    if( m_lastCircDir.isEmpty() )  m_lastCircDir = appPath + "/examples/Arduino/Voltimeter/voltimeter.simu";

    //if( m_curCirc != "" ) Circuit::self()->loadCircuit( m_curCirc );
    
    this->setWindowTitle(m_version);
}
MainWindow::~MainWindow(){ }

void MainWindow::closeEvent(QCloseEvent *event)
{
    newCircuit();
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    restoreGeometry(         m_settings.value("geometry" ).toByteArray());
    restoreState(            m_settings.value("windowState" ).toByteArray());
    m_Centralsplitter->restoreState( m_settings.value("Centralsplitter/geometry").toByteArray());
    m_docList    =           m_settings.value( "lastDocs" ).toStringList();
    m_lastCircDir =          m_settings.value( "lastCircDir" ).toString();
    //m_curCirc =              m_settings.value( "currentCircuit" ).toString();
}

void MainWindow::writeSettings()
{
    m_settings.setValue( "geometry", saveGeometry() );
    m_settings.setValue( "windowState", saveState() );
    m_settings.setValue( "Centralsplitter/geometry", m_Centralsplitter->saveState() );
    m_settings.setValue( "lastDocs", m_docList );
    m_settings.setValue( "lastCircDir", m_lastCircDir );
    //m_settings.setValue( "currentCircuit", m_curCirc );
    foreach( QTreeWidgetItem* item, components->findItems("",Qt::MatchStartsWith)  )
    {
        MainWindow::self()->settings()->setValue( item->text(0)+"/collapsed", !item->isExpanded() );
        for( int j=0; j<item->childCount(); j++ )
        {
            MainWindow::self()->settings()->setValue( item->child(j)->text(0)+"/collapsed", !item->child(j)->isExpanded() );
        }
    }
}

void MainWindow::newCircuit()
{
    m_circuit->powerCircOff();
    
    m_circuit->clear();
    m_curCirc = "";
    
    this->setWindowTitle(m_version+"  -  New Circuit");
}

void MainWindow::openCirc()
{
    const QString dir = m_lastCircDir;
    QString fileName = QFileDialog::getOpenFileName( 0l, tr("Load Circuit"), dir,
                                          tr("Circuits (*.simu);;All files (*.*)"));

    if( !fileName.isEmpty() && fileName.endsWith(".simu") )
    {
        newCircuit();
        Circuit::self()->loadCircuit( fileName );
   
        m_curCirc = fileName;
        m_lastCircDir = fileName;
        this->setWindowTitle(m_version+"  -  "+fileName.split("/").last());
    }
}

void MainWindow::saveCirc()
{
    bool saved = false;
    if( m_curCirc.isEmpty() ) saved =  saveCircAs();
    else                      saved =  Circuit::self()->saveCircuit( m_curCirc );
    
    if( saved ) 
    {
        QString fileName = m_curCirc;
        this->setWindowTitle(m_version+"  -  "+fileName.split("/").last());
    }
}

bool MainWindow::saveCircAs()
{
    const QString dir = m_lastCircDir;
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save Circuit"), dir,
                                                     tr("Circuits (*.simu);;All files (*.*)"));
    if (fileName.isEmpty()) return false;

    m_curCirc = fileName;
    m_lastCircDir = fileName;
    
    bool saved = Circuit::self()->saveCircuit(fileName);
    if( saved ) 
    {
        QString fileName = m_curCirc;
        this->setWindowTitle(m_version+"  -  "+fileName.split("/").last());
    }
    return saved;
}
void MainWindow::openInfo()
{
    QDesktopServices::openUrl(QUrl("http://simulide.blogspot.com"));
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("Circuit simulation"
               "and IDE for mcu development"));
}

void MainWindow::createWidgets()
{
    QWidget *centralWidget = new QWidget( this );
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    QGridLayout *baseWidgetLayout = new QGridLayout( centralWidget );
    baseWidgetLayout->setSpacing(0);
    baseWidgetLayout->setContentsMargins(0, 0, 0, 0);
    baseWidgetLayout->setObjectName("gridLayout");

    m_Centralsplitter = new QSplitter( this );
    m_Centralsplitter->setObjectName("Centralsplitter");
    m_Centralsplitter->setOrientation( Qt::Horizontal );

    m_sidepanel = new QTabWidget( this );
    m_sidepanel->setObjectName("sidepanel");
    m_sidepanel->setTabPosition( QTabWidget::West );
    m_Centralsplitter->addWidget( m_sidepanel );

    components = new ComponentSelector( m_sidepanel );
    components->setObjectName(QString::fromUtf8("components"));
    m_sidepanel->addTab( components, QString::fromUtf8("Components") );

    m_ramTabWidget = new QWidget( this );
    m_ramTabWidget->setObjectName("ramTabWidget");
    m_ramTabWidgetLayout = new QGridLayout( m_ramTabWidget );
    m_ramTabWidgetLayout->setSpacing(0);
    m_ramTabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_ramTabWidgetLayout->setObjectName("ramTabWidgetLayout");
    m_sidepanel->addTab( m_ramTabWidget, tr("RamTable")  );

    m_itemprop = new QPropertyEditorWidget( this );
    m_itemprop->setObjectName(QString::fromUtf8("properties"));
    m_sidepanel->addTab( m_itemprop, QString::fromUtf8("Properties") );

    m_circuit = new CircuitWidget( this );
    m_circuit->setObjectName(QString::fromUtf8("circuit"));
    m_Centralsplitter->addWidget( m_circuit );
    //m_editorWindow = new EditorWindow( this );
    //m_Centralsplitter->addWidget( m_editorWindow );

    baseWidgetLayout->addWidget( m_Centralsplitter, 0, 0 );

    QList<int> sizes;
    sizes << 150 << 350 << 500;
    m_Centralsplitter->setSizes( sizes );

    sizes.clear();
    sizes << 700 << 150;
    //splitter3->setSizes( sizes );

    this->showMaximized();
}


#define STRING(s) #s
#define STRING_MACRO(arg) STRING(arg)

void MainWindow::loadPlugins()
{
    //m_plugins.clear();
    QDir pluginsDir( qApp->applicationDirPath() );

    pluginsDir.cd( "../lib/simulide/plugins" );

    qDebug() << "\n    Loading Plugins at:\n"<<pluginsDir.absolutePath()<<"\n";

    QString pluginName = "*plugin";
    QString pluginNameSuff = "";
#ifndef Q_OS_UNIX
#ifdef      QT_DEBUG
    pluginNameSuff += "d";
#endif      //QT_DEBUG
#ifdef      APP_VERSION
    //VER_MAJ;
    #if QT_VERSION < QT_VERSION_CHECK(5, 7, 0)
    pluginNameSuff += "0"; // If APP_VERSION become >= 1 just patch it there
    #else
    pluginNameSuff += QString::number(QVersionNumber::fromString(APP_VERSION).majorVersion());
    #endif
#endif      //APP_VERSION
#endif  //Q_OS_UNIX
    pluginName += pluginNameSuff;
    QString exeExtention = STRING_MACRO(QT_EXTENSION_SHLIB);
#ifdef Q_OS_UNIX
    if (exeExtention.isEmpty())
        exeExtention = "so";
#endif
    if (!exeExtention.isEmpty())
    {
        pluginName += ".";
        pluginName += exeExtention;
    }

    pluginsDir.setNameFilters( QStringList(pluginName) );

    foreach( QString libName, pluginsDir.entryList( QDir::Files ) )
    {
        pluginName = libName.split(".").first().remove("lib").remove("plugin").toUpper();
        if (!pluginNameSuff.isEmpty())
            pluginName = pluginName.remove(pluginNameSuff.toUpper());
        if( m_plugins.contains(pluginName) ) continue;

        QPluginLoader* pluginLoader = new QPluginLoader( pluginsDir.absoluteFilePath( libName ) );
        QObject* plugin = pluginLoader->instance();

        if( plugin )
        {
            AppIface* item = qobject_cast<AppIface*>( plugin );

            if( item )
            {
                item->initialize();
                m_plugins[pluginName] = pluginLoader;
                qDebug()<< "        Plugin Loaded Successfully:\t" << pluginName;
            }
            else
            {
                pluginLoader->unload();
                delete pluginLoader;
            }
        }
        else
        {
            QString errorMsg = pluginLoader->errorString();
            qDebug()<< "        " << pluginName << "\tplugin FAILED: " << errorMsg;

            if( errorMsg.contains( "libQt5SerialPort" ) )
                errorMsg = " Qt5SerialPort is not installed in your system\n\n    Mcu SerialPort will not work\n    Just Install libQt5SerialPort package\n    To have Mcu Serial Port Working";

            QMessageBox::warning( 0,"App Plugin Error:", errorMsg );
        }
    }
    qDebug() << "\n";
}

void MainWindow::unLoadPugin( QString pluginName )
{
    if( m_plugins.contains( pluginName ) )
    {
        QPluginLoader* pluginLoader = m_plugins[pluginName];
        QObject* plugin = pluginLoader->instance();
        AppIface* item = qobject_cast<AppIface*>( plugin );
        item->terminate();
        pluginLoader->unload();
        m_plugins.remove( pluginName );
        delete pluginLoader;
    }
}

void MainWindow::applyStile()
{
    QFile file(":/simulide.qss");
    file.open(QFile::ReadOnly);

    m_styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet( m_styleSheet );
}

#include  "moc_mainwindow.cpp"

