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
#include "circuit.h"
#include "utils.h"

MainWindow *MainWindow::m_pSelf = 0l;

MainWindow::MainWindow()
          : QMainWindow()
          , m_settings( "SimulIDE", "SimulIDE-0.0.4" )
{
    setWindowIcon( QIcon(":/simulide.png") );
    m_pSelf   = this;
    m_circuit = 0l;
    m_version = "SimulIDE-0.0.4";
    

    createActions();
    createWidgets();
    createToolBars();
    readSettings();

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
}

void MainWindow::newCircuit()
{
    powerCircOff();
    
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

void MainWindow::powerCirc()
{
    if( powerCircAct->iconText() == "Off" ) powerCircOn();
    else                                    powerCircOff();
}

void MainWindow::powerCircOn()
{
    powerCircAct->setIcon(QIcon(":/poweron.png"));
    powerCircAct->setIconText("On");
    Simulator::self()->runContinuous();
}
void MainWindow::powerCircOff()
{
    //if( Simulator::self()->isRunning() )
    {
        powerCircAct->setIcon(QIcon(":/poweroff.png"));
        powerCircAct->setIconText("Off");
        Simulator::self()->stopSim();
    }
}

void MainWindow::setRate( int rate )
{
    if( rate < 0 )
        m_rateLabel->setText( "Circuit ERROR!!!" );
    else 
        m_rateLabel->setText( "Real Speed: "+QString::number(rate) +" %" );
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

    m_circToolBar = new QToolBar( this );
    m_circuit = new CircuitWidget( this, m_circToolBar );
    m_circuit->setObjectName(QString::fromUtf8("circuit"));
    m_Centralsplitter->addWidget( m_circuit );

    //m_editorWindow = new EditorWindow( this );
    //m_Centralsplitter->addWidget( m_editorWindow );
    
    m_rateLabel = new QLabel( this );
    m_rateLabel->setText( "Real Speed: 0 %" );

    baseWidgetLayout->addWidget( m_Centralsplitter, 0, 0 );

    QList<int> sizes;
    sizes << 150 << 350 << 500;
    m_Centralsplitter->setSizes( sizes );

    sizes.clear();
    sizes << 700 << 150;
    //splitter3->setSizes( sizes );

    this->showMaximized();

}


void MainWindow::createActions()
{
    newCircAct = new QAction(QIcon(":/newcirc.png"), tr("New C&ircuit"), this);
    newCircAct->setShortcut(tr("Ctrl+I"));
    newCircAct->setStatusTip(tr("Create a new Circuit"));
    connect( newCircAct, SIGNAL(triggered()), this, SLOT(newCircuit()));

    openCircAct = new QAction(QIcon(":/opencirc.png"), tr("&Open Circuit"), this);
    openCircAct->setShortcut(tr("Ctrl+O"));
    openCircAct->setStatusTip(tr("Open an existing Circuit"));
    connect(openCircAct, SIGNAL(triggered()), this, SLOT(openCirc()));

    saveCircAct = new QAction(QIcon(":/savecirc.png"), tr("&Save Circuit"), this);
    saveCircAct->setShortcut(tr("Ctrl+S"));
    saveCircAct->setStatusTip(tr("Save the Circuit to disk"));
    connect(saveCircAct, SIGNAL(triggered()), this, SLOT(saveCirc()));

    saveCircAsAct = new QAction(QIcon(":/savecircas.png"),tr("Save Circuit &As..."), this);
    saveCircAsAct->setStatusTip(tr("Save the Circuit under a new name"));
    connect(saveCircAsAct, SIGNAL(triggered()), this, SLOT(saveCircAs()));

    powerCircAct = new QAction(QIcon(":/poweroff.png"),tr("Power Circuit"), this);
    powerCircAct->setStatusTip(tr("Power the Circuit"));
    powerCircAct->setIconText("Off");
    connect(powerCircAct, SIGNAL(triggered()), this, SLOT(powerCirc()));
}

void MainWindow::createToolBars()
{
    m_circToolBar->setObjectName("m_circToolBar");
    m_circToolBar->addAction(newCircAct);
    m_circToolBar->addAction(openCircAct);
    m_circToolBar->addAction(saveCircAct);
    m_circToolBar->addAction(saveCircAsAct);
    m_circToolBar->addSeparator();//..........................
    m_circToolBar->addAction(powerCircAct);
    m_circToolBar->addSeparator();//..........................
    m_circToolBar->addWidget( m_rateLabel );
}

void MainWindow::applyStile()
{
    QFile file(":/simulide.qss");
    file.open(QFile::ReadOnly);

    m_styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet( m_styleSheet );
}

#include  "moc_mainwindow.cpp"

