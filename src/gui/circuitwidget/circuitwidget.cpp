/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include "circuitwidget.h"
#include "mainwindow.h"
#include "circuit.h"
#include "filebrowser.h"

CircuitWidget*  CircuitWidget::m_pSelf = 0l;

CircuitWidget::CircuitWidget( QWidget *parent  )
    : QWidget( parent )
    ,m_verticalLayout(this)
    ,m_horizontLayout()
    ,m_circView(this)
    ,m_terminal(this)
    ,m_oscope(this)
    ,m_plotter(this)
    ,m_serial(this)
    ,m_circToolBar(this)
    ,m_lastCircDir(MainWindow::self()->settings()->value("lastCircDir").toByteArray())
{
    m_pSelf = this;
    
    m_oscope.setupWidget( 180 );

    m_verticalLayout.setObjectName(tr("verticalLayout"));
    m_verticalLayout.setContentsMargins(0, 0, 0, 0);
    m_verticalLayout.setSpacing(0);

    m_verticalLayout.addWidget( &m_circToolBar );
    m_verticalLayout.addWidget( &m_circView );
    
    m_verticalLayout.addLayout( &m_horizontLayout );
    m_horizontLayout.addWidget( &m_oscope );
    m_horizontLayout.addWidget( &m_plotter );
    m_horizontLayout.addWidget( &m_terminal );
    m_horizontLayout.addWidget( &m_serial);
    
    connect( this,     &CircuitWidget::dataAvailable,
             &m_serial, &SerialPortWidget::slotWriteData );
    
    m_rateLabel = new QLabel( this );
    m_rateLabel->setText( "Real Speed: 0 %" );
    
    createActions();
    createToolBars();
    
    QString appPath = QCoreApplication::applicationDirPath();
    
    if( m_lastCircDir.isEmpty() )  m_lastCircDir = appPath + "/examples/Arduino/Voltimeter/voltimeter.simu";
    
    newCircuit();
}
CircuitWidget::~CircuitWidget() { }

OscopeWidget* CircuitWidget::oscope() { return &m_oscope; }

void CircuitWidget::clear()
{
    m_circView.clear();
    Simulator::self()->addToElementList( &m_oscope );
}

void CircuitWidget::createActions()
{
    newCircAct = new QAction(QIcon(":/newcirc.png"), tr("New C&ircuit\tCtrl+N"), this);
    newCircAct->setStatusTip(tr("Create a new Circuit"));
    connect( newCircAct, SIGNAL(triggered()), this, SLOT(newCircuit()));

    openCircAct = new QAction(QIcon(":/opencirc.png"), tr("&Open Circuit\tCtrl+O"), this);
    openCircAct->setStatusTip(tr("Open an existing Circuit"));
    connect(openCircAct, SIGNAL(triggered()), this, SLOT(openCirc()));

    saveCircAct = new QAction(QIcon(":/savecirc.png"), tr("&Save Circuit\tCtrl+S"), this);
    saveCircAct->setStatusTip(tr("Save the Circuit to disk"));
    connect(saveCircAct, SIGNAL(triggered()), this, SLOT(saveCirc()));

    saveCircAsAct = new QAction(QIcon(":/savecircas.png"),tr("Save Circuit &As...\tCtrl+Shift+S"), this);
    saveCircAsAct->setStatusTip(tr("Save the Circuit under a new name"));
    connect(saveCircAsAct, SIGNAL(triggered()), this, SLOT(saveCircAs()));

    powerCircAct = new QAction(QIcon(":/poweroff.png"),tr("Power Circuit"), this);
    powerCircAct->setStatusTip(tr("Power the Circuit"));
    powerCircAct->setIconText("Off");
    connect(powerCircAct, SIGNAL(triggered()), this, SLOT(powerCirc()));
    
    infoAct = new QAction(QIcon(":/help.png"),tr("Online Help"), this);
    infoAct->setStatusTip(tr("Online Help"));
    infoAct->setIconText("Off");
    connect(infoAct, SIGNAL(triggered()), this, SLOT(openInfo()));
}

void CircuitWidget::createToolBars()
{
    m_circToolBar.setObjectName("m_circToolBar");
    m_circToolBar.addAction(newCircAct);
    m_circToolBar.addAction(openCircAct);
    m_circToolBar.addAction(saveCircAct);
    m_circToolBar.addAction(saveCircAsAct);
    m_circToolBar.addSeparator();//..........................
    m_circToolBar.addAction(powerCircAct);
    m_circToolBar.addSeparator();//..........................
    m_circToolBar.addWidget( m_rateLabel );

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    m_circToolBar.addWidget(spacerWidget);
    m_circToolBar.addAction(infoAct);
    m_circToolBar.addSeparator();//..........................
}

void CircuitWidget::newCircuit()
{
    powerCircOff();
    
    clear();
    m_curCirc = "";

    MainWindow::self()->setTitle("New Circuit");
    MainWindow::self()->settings()->setValue( "lastCircDir", m_lastCircDir );
}

void CircuitWidget::openCirc()
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
        MainWindow::self()->setTitle(fileName.split("/").last());
        MainWindow::self()->settings()->setValue( "lastCircDir", m_lastCircDir );
        FileBrowser::self()->setPath(m_lastCircDir);
    }
}

void CircuitWidget::saveCirc()
{
    bool saved = false;
    if( m_curCirc.isEmpty() ) saved =  saveCircAs();
    else                      saved =  Circuit::self()->saveCircuit( m_curCirc );
    
    if( saved ) 
    {
        QString fileName = m_curCirc;
        MainWindow::self()->setTitle(fileName.split("/").last());
    }
}

bool CircuitWidget::saveCircAs()
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
        MainWindow::self()->setTitle(fileName.split("/").last());
        MainWindow::self()->settings()->setValue( "lastCircDir", m_lastCircDir );
        FileBrowser::self()->setPath(m_lastCircDir);
    }
    return saved;
}

void CircuitWidget::powerCirc()
{
    if( powerCircAct->iconText() == "Off" ) powerCircOn();
    else                                    powerCircOff();
}

void CircuitWidget::powerCircOn()
{
    powerCircAct->setIcon(QIcon(":/poweron.png"));
    powerCircAct->setIconText("On");
    Simulator::self()->runContinuous();
}
void CircuitWidget::powerCircOff()
{
        powerCircAct->setIcon(QIcon(":/poweroff.png"));
        powerCircAct->setIconText("Off");
        Simulator::self()->stopSim();
}

void CircuitWidget::openInfo()
{
    QDesktopServices::openUrl(QUrl("http://simulide.blogspot.com"));
}

void CircuitWidget::setRate( int rate )
{
    if( rate < 0 )
        m_rateLabel->setText( "Circuit ERROR!!!" );
    else 
        m_rateLabel->setText( "Real Speed: "+QString::number(rate) +" %" );
}

/*void CircuitWidget::setSerialPortWidget( QWidget* serialPortWidget )
{
    m_serialPortWidget = serialPortWidget;
    m_horizontLayout.addWidget( m_serialPortWidget );
}*/

void CircuitWidget::showSerialPortWidget( bool showIt )
{
    m_serial.setVisible( showIt );
}

void CircuitWidget::writeSerialPortWidget( const QByteArray &data )
{
    emit dataAvailable( data );
}

#include "moc_circuitwidget.cpp"
