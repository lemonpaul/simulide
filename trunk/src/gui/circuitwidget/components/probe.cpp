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

#include "probe.h"
#include "connector.h"
#include "itemlibrary.h"
#include "oscopewidget.h"
#include "plotterwidget.h"

bool Probe::m_oscopeBusy = false;

Component* Probe::construct( QObject* parent, QString type, QString id )
{ return new Probe( parent, type, id ); }

LibraryItem* Probe::libraryItem()
{
    return new LibraryItem(
        tr( "Probe" ),
        tr( "Outputs" ),
        "probe.png",
        "Probe",
        Probe::construct );
}

Probe::Probe( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eElement( id.toStdString() )
{
    m_haveOscope = false;
    m_showVolt   = true;

    setLabelPos(-32,-24, 45 ); // x, y, rot 

    m_voltIn   = cero_doub;
    m_voltTrig = 2.5;
    m_plotterLine = -1;
    m_plotterColor = QColor( 255, 255, 255 );

    // Create volts Label
    m_dispvolt = new Label( this );
    m_dispvolt->setPos( 16, -4 );
    m_dispvolt->setRotation( 45 );
    m_dispvolt->setPlainText( "0 V" );
    m_dispvolt->setDefaultTextColor( Qt::darkRed );
    //m_dispvolt->setEnabled( false );
    //m_valLabel->setEnabled( false );
    //m_valLabel->setVisible( false );

    // Create Input Pin
    m_ePin.resize(1);
    QString nodid = id;
    nodid.append(QString("inpin"));
    QPoint nodpos = QPoint(-22,0);
    m_inputpin = new Pin( 180, nodpos, nodid, 0, this);
    m_inputpin->setLength( 20 );
    m_inputpin->setBoundingRect( QRect(-2, -2, 4, 4) );
    
    nodid.append( QString("-eSource") );
    eSource* m_inSource = new eSource( nodid.toStdString(), m_inputpin );
    m_inSource->setOut(false);
    m_inSource->setImp( 1e9 );

    setRotation( rotation() - 45 );

    Simulator::self()->addToUpdateList( this );
}
Probe::~Probe()
{
}

void Probe::updateStep()
{
    if( !Simulator::self()->isRunning() )
    {
        setVolt( 0.0 );
        return;
    }

    if( m_inputpin->isConnected() )// Voltage from connected pin
    {
         setVolt( m_inputpin->getVolt() );
         return;
    }

    // Voltage from connector or Pin behind inputPin
    QList<QGraphicsItem*> list = m_inputpin->collidingItems();

    if( list.isEmpty() )
    {
        setVolt( 0.0 );
        return;
    }

    foreach( QGraphicsItem *it, list )
    {
        if( it->type() == 65536 )                          // Component
        {
            ConnectorLine *line =  qgraphicsitem_cast<ConnectorLine *>( it );

            Connector *con = line->connector();

            if( con->objectName().startsWith("Connector") ) // Connector found
            {
                setVolt( con->getVolt() ); //startPin()->volt();
                break;
            }
        }
        else if( it->type() == 65536+3 )                    // Pin found
        {
            Pin *pin =  qgraphicsitem_cast<Pin *>( it );
            setVolt( pin->getVolt() );
            //qDebug() << " probe: Pin found" << volt;
            break;
        }
    }
    //qDebug() << " probe: " /*<< item->type()*/ << UserType;//con->objectName();
}

void Probe::setVolt( double volt )
{
    if( m_voltIn == volt ) return;

    m_voltIn = volt;
    int dispVolt = int(m_voltIn*100);
    if( m_showVolt ) m_dispvolt->setPlainText( QString("%1 V").arg(double(dispVolt)/100));
    else             m_dispvolt->setPlainText("");

    if( m_plotterLine > -1 ) PlotterWidget::self()->setData(m_plotterLine, dispVolt );

    update();       // Repaint
}

double Probe::getVolt()
{
    double volt = 0;
    if( m_inputpin->isConnected() ) volt = m_inputpin->getVolt();
    return volt;
}

void Probe::remove()
{
    Simulator::self()->remFromUpdateList( this );
    if( m_inputpin->isConnected() )
    {
        slotOscopRem();
        m_inputpin->connector()->remove();
    }
    slotPlotterRem();
    Component::remove();
}

void Probe::slotPlotterAdd()
{
    m_plotterLine = PlotterWidget::self()->addChannel();
    if( m_plotterLine < 0 ) return;

    PlotterWidget::self()->setData( m_plotterLine, int(m_voltIn*100) );
    m_plotterColor = PlotterWidget::self()->getColor( m_plotterLine );
    update();       // Repaint
}

void Probe::slotPlotterRem()
{
    //qDebug() << m_plotterLine;
    if( m_plotterLine < 0 ) return;

    PlotterWidget::self()->remChannel( m_plotterLine );
    m_plotterLine = -1;
    update();       // Repaint
}

void Probe::slotOscopAdd()
{
    if( m_oscopeBusy ) return; // Another probe is using oscope
    
    if( m_inputpin->isConnected() )
    {
        OscopeWidget::self()->setProbe( this );
        OscopeWidget::self()->setVisible( true );
        m_haveOscope = true;
        m_oscopeBusy = true;
    }
}

void Probe::slotOscopRem()
{
    if( !m_haveOscope ) return; // this probe is not using oscope
    
    //if( m_inputpin->isConnected() )
    {
        OscopeWidget::self()->setProbe( 0l );
        OscopeWidget::self()->setVisible( false );
        m_haveOscope = false;
        m_oscopeBusy = false;
    }
}

void Probe::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
    QMenu *menu = new QMenu();

    QAction *plotterAddAction = menu->addAction(QIcon(":/fileopen.png"),"Add to Plotter");
    connect(plotterAddAction, SIGNAL(triggered()), this, SLOT(slotPlotterAdd()));

    QAction *plotterRemAction = menu->addAction(QIcon(":/fileopen.png"),"Remove from Plotter");
    connect(plotterRemAction, SIGNAL(triggered()), this, SLOT(slotPlotterRem()));
    
    if( m_inputpin->isConnected() )
    {
        menu->addSeparator();
        
        QAction *oscopAddAction = menu->addAction(QIcon(":/fileopen.png"),"Add to Oscope");
        connect(oscopAddAction, SIGNAL(triggered()), this, SLOT(slotOscopAdd()));

        QAction *oscopRemAction = menu->addAction(QIcon(":/fileopen.png"),"Remove from Oscope");
        connect(oscopRemAction, SIGNAL(triggered()), this, SLOT(slotOscopRem()));
    }

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

bool Probe::Show_volt()                { return  m_showVolt; }
void Probe::setShow_volt( bool show )  { m_showVolt = show; setVolt(m_voltIn); }

void Probe::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    if( m_plotterLine > -1 )            p->setBrush( m_plotterColor );
    else if ( m_voltIn > m_voltTrig)  p->setBrush( QColor( 255, 166, 0 ) );
    else                              p->setBrush( QColor( 230, 230, 255 ) );

    p->drawEllipse( -8, -8, 16, 16 );
    if( m_haveOscope )
    {
        p->drawLine( -8, 0, 8, 0 );
        p->drawLine(  0,-8, 0, 8 );
    }
}

#include "moc_probe.cpp"

