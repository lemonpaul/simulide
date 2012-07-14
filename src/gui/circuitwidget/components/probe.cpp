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
    m_showVolt = true;
    m_labelx   = -32;
    m_labely   = -24;
    m_labelrot = 45;
    setLabelPos();


    m_voltIn   = cero_doub;
    m_voltTrig = 2.5;
    m_oscopLine = -1;
    m_oscopColor = QColor( 255, 255, 255 );

    // Create volts Label
    m_dispvolt = new Label( this );
    m_dispvolt->setPos( 14, 3 );
    m_dispvolt->setRotation( 45 );
    m_dispvolt->setText( "0 V" );
    m_dispvolt->setBrush( Qt::darkRed );

    // Create Input Pin
    m_ePin.resize(1);
    QString nodid = id;
    nodid.append(QString("inpin"));
    QPoint nodpos = QPoint(-22,0);
    inputpin = new Pin( 180, nodpos, nodid, 0, this);
    inputpin->setLength( 20 );
    inputpin->setBoundingRect( QRect(-2, -2, 4, 4) );

    setRotation( rotation() - 45 );

    //label->setRotation( label->rotation() + 45 );
    // Input Pin signal
    /*connect( inputpin, SIGNAL(wireState(int)),
             this,     SLOT  (inStateChanged(int)) );*/

    Simulator::self()->addToUpdateList( this );
}
Probe::~Probe() { Simulator::self()->remFromUpdateList( this );/*killTimer( m_timerId );*/ }

void Probe::updateStep()
{
    if( !Simulator::self()->isRunning() )
    {
        setVolt( 0.0 );
        return;
    }
    //if( ++m_timeStep == 10 ) m_timeStep = 0; // 100 mS
    //else return;

    if( inputpin->isConnected() )// Voltage from connected pin
    {
         setVolt( inputpin->getVolt() );
         return;
    }

    // Voltage from connector or Pin behind inputPin
    QList<QGraphicsItem*> list = inputpin->collidingItems();

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
    if( m_showVolt ) m_dispvolt->setText( QString("%1 V").arg(double(dispVolt)/100));
    else             m_dispvolt->setText("");

    if( m_oscopLine > -1 ) OscopeWidget::self()->setData(m_oscopLine, dispVolt );

    update();       // Repaint
}

/*void Probe::setChanged( bool changed )
{
    m_changed = changed;
    if( !m_changed )  inputpin->setChanged(false);
}*/

void Probe::remove()
{
    Simulator::self()->remFromUpdateList( this );
    if( inputpin->isConnected() )  inputpin->connector()->remove();
    Component::remove();
}

void Probe::slotOscopAdd()
{
    m_oscopLine = OscopeWidget::self()->addChannel();
    if( m_oscopLine < 0 ) return;

    OscopeWidget::self()->setData( m_oscopLine, int(m_voltIn*100) );
    m_oscopColor = OscopeWidget::self()->getColor( m_oscopLine );
    update();       // Repaint
}

void Probe::slotOscopRem()
{
    if( m_oscopLine < 0 ) return;

    OscopeWidget::self()->remChannel( m_oscopLine );
    m_oscopLine = -1;
    update();       // Repaint
}

void Probe::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
    QMenu *menu = new QMenu();

    QAction *oscopAddAction = menu->addAction(QIcon(":/fileopen.png"),"Add to Oscope");
    connect(oscopAddAction, SIGNAL(triggered()), this, SLOT(slotOscopAdd()));

    QAction *oscopRemAction = menu->addAction(QIcon(":/fileopen.png"),"Remove from Oscope");
    connect(oscopRemAction, SIGNAL(triggered()), this, SLOT(slotOscopRem()));

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

bool Probe::Show_volt()                { return  m_showVolt; }
void Probe::setShow_volt( bool show )  { m_showVolt = show; setVolt(m_voltIn); }

void Probe::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    if( m_oscopLine > -1 )            p->setBrush( m_oscopColor );
    else if ( m_voltIn > m_voltTrig)  p->setBrush( QColor( 255, 166, 0 ) );
    else                              p->setBrush( QColor( 230, 230, 255 ) );

    p->drawEllipse( -8, -8, 16, 16 );
}

#include "moc_probe.cpp"

