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
#include "component.h"
#include "circuit.h"
#include "utils.h"

MainWindow *MainWindow::m_pSelf = 0l;

MainWindow::MainWindow()
{
    m_pSelf   = this;
    m_circuit = 0l;

    createActions();
    createWidgets();
    createToolBars();
    readSettings();

    QString appPath = QCoreApplication::applicationDirPath();

    if( m_lastCircDir.isEmpty() )  m_lastCircDir = appPath + "/examples/Arduino/Voltimeter/voltimeter.simu";

    //if( m_curCirc != "" ) Circuit::self()->loadCircuit( m_curCirc );
    this->setWindowTitle("SimulIDE 0.0.3");
}
MainWindow::~MainWindow(){ }

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings                settings( "PicLinux", "SimulIDE_0.0.3" );
    restoreGeometry(         settings.value("geometry" ).toByteArray());
    restoreState(            settings.value("windowState" ).toByteArray());
    splitter3->restoreState( settings.value("splitter3/geometry" ).toByteArray());
    splitter5->restoreState( settings.value("splitter5/geometry").toByteArray());
    m_docList    =           settings.value( "lastDocs" ).toStringList();
    m_lastCircDir =          settings.value( "lastCircDir" ).toString();
    //m_curCirc =              settings.value( "currentCircuit" ).toString();
}

void MainWindow::writeSettings()
{
    // $HOME/.config/PicLinux/simulide.conf
    QSettings settings("PicLinux", "SimulIDE_0.0.3");
    settings.setValue( "geometry", saveGeometry() );
    settings.setValue( "windowState", saveState() );
    settings.setValue( "splitter3/geometry", splitter3->saveState() );
    settings.setValue( "splitter5/geometry", splitter5->saveState() );
    settings.setValue( "lastDocs", m_docList );
    settings.setValue( "lastCircDir", m_lastCircDir );
    //settings.setValue( "currentCircuit", m_curCirc );
}

void MainWindow::newCircuit()
{
    powerCircOff();
    
    m_circuit->clear();
    m_curCirc = "";
    
    this->setWindowTitle("SimulIDE 0.0.3  -  New Circuit");
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
        this->setWindowTitle("SimulIDE 0.0.3  -  "+fileName.split("/").last());
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
        this->setWindowTitle("SimulIDE 0.0.3  -  "+fileName.split("/").last());
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
        this->setWindowTitle("SimulIDE 0.0.3  -  "+fileName.split("/").last());
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
    if( Simulator::self()->isRunning() )
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

    splitter5 = new QSplitter( this );
    splitter5->setObjectName("splitter5");
    splitter5->setOrientation( Qt::Horizontal );

    sidepanel = new QTabWidget( this );
    sidepanel->setObjectName("sidepanel");
    sidepanel->setTabPosition( QTabWidget::West );
    splitter5->addWidget( sidepanel );

    components = new ComponentSelector( sidepanel );
    components->setObjectName(QString::fromUtf8("components"));
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

    m_rateLabel = new QLabel( this );
    m_rateLabel->setText( "Real Speed: 0 Hz" );

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
    circToolBar->setObjectName("circToolBar");
    circToolBar->addAction(newCircAct);
    circToolBar->addAction(openCircAct);
    circToolBar->addAction(saveCircAct);
    circToolBar->addAction(saveCircAsAct);
    circToolBar->addSeparator();//..........................
    circToolBar->addAction(powerCircAct);
    circToolBar->addSeparator();//..........................
    circToolBar->addWidget( m_rateLabel );
}

void MainWindow::applyStile()
{
    QFile file(":/simulide.qss");
    file.open(QFile::ReadOnly);

    m_styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet( m_styleSheet );
}

#include  "moc_mainwindow.cpp"

