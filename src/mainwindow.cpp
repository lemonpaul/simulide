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
#include "avrprocessor.h"
#include "component.h"
#include "circuit.h"
#include "utils.h"

MainWindow *MainWindow::m_pSelf = 0l;

MainWindow::MainWindow()
{
    qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);

    m_pSelf   = this;
    m_circuit = 0l;

    //applyStile();

    createActions();
    createWidgets();
    //createMenus();
    createToolBars();

    //statusBar()->showMessage(tr("Ready"));

    readSettings();

    QString appPath = QCoreApplication::applicationDirPath();

    if( m_lastCircDir.isEmpty() )  m_lastCircDir = appPath + "/examples/mega48_adc/mega48_adc.simu";

    //Circuit::self()->loadCircuit( m_lastCircDir );
    m_curCirc = ""; //m_lastCircDir ;

    //readSettings();
    //stop();
}
MainWindow::~MainWindow(){ }

void MainWindow::closeEvent(QCloseEvent *event)
{
    //if (maybesaveDoc())
    {
        writeSettings();
        event->accept();
    }
    //else event->ignore();
}

void MainWindow::readSettings()
{
    QSettings settings( "PicLinux", "SimulIDE" );
    restoreGeometry( settings.value("geometry" ).toByteArray());
    restoreState( settings.value("windowState" ).toByteArray());
    splitter3->restoreState( settings.value("splitter3/geometry" ).toByteArray());
    splitter5->restoreState( settings.value("splitter5/geometry").toByteArray());
    m_docList    = settings.value( "lastDocs" ).toStringList();
    m_lastCircDir = settings.value( "lastCircDir" ).toString();
}

void MainWindow::writeSettings()
{
    // $HOME/.config/PicLinux/simulide.conf
    QSettings settings("PicLinux", "SimulIDE");
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );
    settings.setValue( "splitter3/geometry", splitter3->saveState() );
    settings.setValue( "splitter5/geometry", splitter5->saveState() );
    settings.setValue( "lastDocs", m_docList );
    settings.setValue( "lastCircDir", m_lastCircDir );
}

void MainWindow::newCircuit()
{
    m_circuit->clear();
    m_curCirc = "";
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
    }
}

bool MainWindow::saveCirc()
{
    if( m_curCirc.isEmpty() ) return saveCircAs();
    else                      return Circuit::self()->saveCircuit( m_curCirc );
}

bool MainWindow::saveCircAs()
{
    const QString dir = m_lastCircDir;
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save Circuit"), dir,
                                                     tr("Circuits (*.simu);;All files (*.*)"));
    if (fileName.isEmpty()) return false;

    m_curCirc = fileName;
    m_lastCircDir = fileName;

    return Circuit::self()->saveCircuit(fileName);
}

/*void MainWindow::run()
{
    pauseAct->setEnabled( true );
    stepAct->setEnabled( false );
    stepOverAct->setEnabled( false );
    runAct->setEnabled( false );
}*/
void MainWindow::pause()
{
    runAct->setEnabled( true );
    stepAct->setEnabled( true );
    //stepOverAct->setEnabled( true );
    pauseAct->setEnabled( false );
}
void MainWindow::reset()
{
    runAct->setEnabled( true );
    stepAct->setEnabled( true );
    //stepOverAct->setEnabled( true );
    pauseAct->setEnabled( false );
    stopAct->setEnabled( true );
}

/*void MainWindow::stop()
{

    runAct->setEnabled( false );
    stepAct->setEnabled( false );
    stepOverAct->setEnabled( false );
    pauseAct->setEnabled( false );
    stopAct->setEnabled( false );
    //powerCircOff();
}*/

void MainWindow::powerCirc()
{
    if( powerCircAct->iconText() == "Off" ) powerCircOn();
    else                                    powerCircOff();
}

void MainWindow::powerCircOn()
{
    powerCircAct->setIcon(QIcon(":/poweron.png"));
    powerCircAct->setIconText("On");
    //Debugger::self()->setPower( true );
    Simulator::self()->runContinuous();
}
void MainWindow::powerCircOff()
{
    powerCircAct->setIcon(QIcon(":/poweroff.png"));
    powerCircAct->setIconText("Off");
    //Debugger::self()->setPower( false );
    Simulator::self()->stopSim();
}

void MainWindow::simuRateChanged( int rate )
{
    Simulator::self()->simuRateChanged( rate );

    if      ( rate > 1000 ) rate = 100;
    else if ( rate > 500 )  rate = 50;
    else if ( rate > 100 )  rate = 10;
    else if ( rate > 50 )   rate = 5;
    else                    rate = 1;

    simuRate->setSingleStep( rate );
}



/*void MainWindow::loadFile( QString &fileName )
{
    if( fileName.isEmpty() ) return;

    if( m_docList.contains(fileName) )
    {
        int count = docWidget->count();

        for( int index=0; index<count; index++ )
            if( docWidget->tabText(index) == strippedName(fileName) )
                docWidget->setCurrentIndex(index);
        return;
    }
    load( fileName );

    m_lastDocDir = fileName;
    m_docList.append(fileName);
}*/


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

    splitter5 = new QSplitter( this );
    splitter5->setObjectName("splitter5");
    splitter5->setOrientation( Qt::Horizontal );

    sidepanel = new QTabWidget( this );
    sidepanel->setObjectName("sidepanel");
    sidepanel->setTabPosition( QTabWidget::West );
    splitter5->addWidget( sidepanel );


    components = new ComponentSelector( sidepanel );
    components->setObjectName(QString::fromUtf8("components"));
    //components->setVisible(false);
    sidepanel->addTab( components, QString::fromUtf8("Components") );

    QWidget *ramTabWidget = new QWidget( this );
    ramTabWidget->setObjectName("ramTabWidget");
    ramTabWidgetLayout = new QGridLayout( ramTabWidget );
    ramTabWidgetLayout->setSpacing(0);
    ramTabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    ramTabWidgetLayout->setObjectName("ramTabWidgetLayout");
    sidepanel->addTab( ramTabWidget, tr("RamTable")  );

    itemprop = new QPropertyEditorWidget( this );
    itemprop->setObjectName(QString::fromUtf8("properties"));
    sidepanel->addTab( itemprop, QString::fromUtf8("Properties") );

    circToolBar = new QToolBar( this );
    m_circuit = new CircuitWidget( this, circToolBar );
    m_circuit->setObjectName(QString::fromUtf8("circuit"));
    splitter5->addWidget( m_circuit );

    splitter3 = new QSplitter( this );      //Editor doc+outmsg
    splitter3->setObjectName("splitter3");
    splitter3->setOrientation( Qt::Vertical );
    splitter5->addWidget( splitter3 );

    simuRate = new QSpinBox;
    simuRate->setMaximum( 20000000 );
    simuRate->setMinimum( 1 );
    simuRate->setValue( 16000000 );
    simuRate->setSuffix(" Hz");

    connect(simuRate, SIGNAL(valueChanged(int)), this, SLOT  (simuRateChanged(int)));

    /*circWidget = new QTabWidget( this );
    circWidget->setObjectName("circWidget");
    circWidget->setTabPosition( QTabWidget::North );
    circWidget->setTabsClosable ( true );
    circWidget->setMovable( true );
    splitter5->addWidget( circWidget );*/
    //connect( circWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(circCloseTab(int)));
    //connect( circWidget, SIGNAL(currentChanged(int)), this, SLOT(documentWasModified(int)));

    baseWidgetLayout->addWidget( splitter5, 0, 0 );

    QList<int> sizes;
    sizes << 150 << 350 << 500;
    splitter5->setSizes( sizes );

    sizes.clear();
    sizes << 700 << 150;
    splitter3->setSizes( sizes );

    this->showMaximized();
}

void MainWindow::createActions()
{
    /*exitAct = new QAction(QIcon::fromTheme("file-exit", QIcon(":/exit.png")),tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));*/

    /*runAct =  new QAction(QIcon(":/play.png"),tr("Run"), this);
    runAct->setStatusTip(tr("Run to next breakpoint"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));*/

    /*stepAct = new QAction(QIcon(":/step.png"),tr("Step"), this);
    stepAct->setStatusTip(tr("Step debugger"));
    stepAct->setEnabled(true);
    connect( stepAct, SIGNAL(triggered()), this, SLOT(step()) );*/


    /*pauseAct = new QAction(QIcon(":/pause.png"),tr("Pause"), this);
    pauseAct->setStatusTip(tr("Pause debugger"));
    pauseAct->setEnabled(false);
    connect( pauseAct, SIGNAL(triggered()), this, SLOT(pause()) );

    resetAct = new QAction(QIcon(":/reset.png"),tr("Reset"), this);
    resetAct->setStatusTip(tr("Reset debugger"));
    resetAct->setEnabled(true);
    connect( resetAct, SIGNAL(triggered()), this, SLOT(reset()) );

    stopAct = new QAction(QIcon(":/stop.png"),tr("Stop"), this);
    stopAct->setStatusTip(tr("Stop debugger"));
    stopAct->setEnabled(false);
    connect( stopAct, SIGNAL(triggered()), this, SLOT(stop()) );

    compileAct = new QAction(QIcon(":/compile.png"),tr("Compile"), this);
    compileAct->setStatusTip(tr("Compile Source"));
    compileAct->setEnabled(true);
    connect( compileAct, SIGNAL(triggered()), this, SLOT(compile()) );*/

    /*aboutAct = new QAction(QIcon(":/info.png"),tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));*/

    /*aboutQtAct = new QAction(QIcon(":/info.png"),tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));*/

    //connect(m_codeEditor, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    //connect(m_codeEditor, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

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

void MainWindow::createMenus()
{
    /*fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setObjectName("fileMenu");
    fileMenu->addAction(newCircAct);
    fileMenu->addSeparator();//..........................
    fileMenu->addAction(openCircAct);
    fileMenu->addAction(saveCircAct);
    fileMenu->addAction(saveCircAsAct);
    fileMenu->addSeparator();//..........................
    fileMenu->addAction(exitAct);*/

    /*editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();//..........................
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);*/

    //circMenu = menuBar()->addMenu(tr("Circuit"));

    //debugMenu->addSeparator();//..........................
    //circMenu->addAction(powerCircAct);

    /*menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);*/
}

void MainWindow::createToolBars()
{
    //fileToolBar = addToolBar(tr("File"));

    circToolBar->setObjectName("circToolBar");
    circToolBar->addAction(newCircAct);
    circToolBar->addAction(openCircAct);
    circToolBar->addAction(saveCircAct);
    circToolBar->addAction(saveCircAsAct);

    circToolBar->addSeparator();//..........................

    circToolBar->addWidget( simuRate );
    circToolBar->addSeparator();//..........................
    circToolBar->addAction(powerCircAct);


    /*editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("editToolBar");
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addSeparator();//..........................
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);*/

    //toolsToolBar->addWidget( simuRate );
}


void MainWindow::applyStile()
{
    QFile file(":/simulide.qss");
    file.open(QFile::ReadOnly);

    m_styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet( m_styleSheet );
}

#include  "moc_mainwindow.cpp"

