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

#include "switch.h"
#include "itemlibrary.h"
#include "pin.h"
#include "e-node.h"


Component* Switch::construct( QObject* parent, QString type, QString id )
{ return new Switch( parent, type, id ); }

LibraryItem* Switch::libraryItem()
{
    return new LibraryItem(
            tr( "Switch (all)" ),
            tr( "Switches" ),
            "switch.png",
            "Switch",
            Switch::construct);
}

Switch::Switch( QObject* parent, QString type, QString id )
      : SwitchBase( parent, type, id )
{
    m_area =  QRectF( -11, -9, 22, 11 );
    
    //m_pin.resize(2);

    /*QString pinid = m_id;
    pinid.append(QString("-lnod"));
    QPoint pinpos = QPoint(-8-8,0);
    m_pin[0] = new Pin( 180, pinpos, pinid, 0, this);
    m_ePin[0] = m_pin[0];

    pinid = m_id;
    pinid.append(QString("-rnod"));
    pinpos = QPoint(8+8,0);
    m_pin[1] = new Pin( 0, pinpos, pinid, 1, this);
    m_ePin[1] = m_pin[1];*/
    
    m_numthrows = 0;
    m_numPoles = 0;
    m_nClose = false;
    m_closed = false;
    
    SetupSwitches( 1, 1 );
    
    m_proxy->setPos( QPoint(-8, 4) );
    
    connect( m_button, SIGNAL( clicked() ),
             this,     SLOT  ( onbuttonclicked() ));
}
Switch::~Switch(){}

void Switch::onbuttonclicked()
{
    m_closed = false;
    if( m_button->isChecked() ) m_closed = true;
    if( m_nClose )              m_closed = !m_closed;
    m_changed = true;
    
    update();
}

void Switch::initialize()
{//qDebug() << "Switch::initialize:"<<m_numPoles<<;
    for( int i=0; i<m_numPoles; i++ )
    {
        eNode* node = m_pin[i*(1+m_numthrows)]->getEnode();

        int epinN = i*m_numthrows*2;
        m_ePin[ epinN ]->setEnode( node );

        if( m_numthrows > 1 )
            m_ePin[ epinN+2 ]->setEnode( node );
    }
    for( uint i=0; i<m_ePin.size(); i++ )
    {
        eNode* node = m_ePin[i]->getEnode();
        
        if( node ) node->setSwitched( true );
    }
    //m_closed = m_nClose;
    //setSwitch( m_nClose );
    onbuttonclicked();
}

void Switch::updateStep()
{
    if( m_changed )
    {
        setSwitch( m_closed );

        m_changed = false;
    }
}

void Switch::setSwitch( bool closed )
{
    for( int i=0; i<m_numPoles; i++ )
    {
        int switchN = i*m_numthrows;

        if( closed ) m_switches[ switchN ]->setAdmit( 1e3 );
        else         m_switches[ switchN ]->setAdmit( 0 );

        if( m_numthrows == 2 )
        {
            switchN++;

            if( !closed ) m_switches[ switchN ]->setAdmit( 1e3 );
            else          m_switches[ switchN ]->setAdmit( 0 );
        }
    }
    update();
}

void Switch::SetupSwitches( int poles, int throws )
{
    m_area = QRectF( -13,-16*poles, 26, 16*poles );

    for( uint i=0; i<m_switches.size(); i++ )
        delete m_switches[i];
        
    //qDebug() << "Switch::SetupSwitches Pins:"<<poles<<throws<<m_numPoles<<m_numthrows;
    for( int i=0; i<m_numPoles; i++ )
    {
        int epinN = i*m_numthrows*2;
        //qDebug() << "Switch::SetupSwitches ePins:"<<epinN;
        delete m_ePin[ epinN ];
        //qDebug() << "Switch::SetupSwitches ePins:"<<epinN+2;
        if( m_numthrows > 1 ) delete m_ePin[ epinN+2 ];
    }

    for( uint i=0; i<m_pin.size(); i++ )
    {
        Pin* pin = m_pin[i];
        if( pin->isConnected() ) pin->connector()->remove();
        pin->reset();
        delete pin;
    }

    m_numPoles = poles;
    m_numthrows = throws;

    m_switches.resize( poles*throws );
    m_pin.resize( poles+poles*throws);

    m_ePin.resize(2*poles*throws);
//qDebug() << "Switch::SetupSwitches" << poles+poles*throws;

    int cont = 0;
    for( int i=0; i<poles; i++ )              // Create Resistors
    {
        Pin* pin;
        int pinN = cont;
        int ePinN = cont;
        QString reid = m_id;

        QPoint pinpos = QPoint(-16,-16*i );
        pin = new Pin( 180, pinpos, reid+"-pinP"+QString::number(pinN), 0, this);
        pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false ); // draw Pins on top
        m_pin[pinN] = pin;

        //qDebug() << "Switch::SetupSwitches Pin:" << pinN<<reid+"-pinP"+QString::number(pinN);

        for( int j=0; j<throws; j++ )
        {
            reid = m_id;

            cont++;
            int tN = i*throws+j;

            reid.append( QString( "-switch"+QString::number(tN)) );
            m_switches[ tN ] = new eResistor( reid.toStdString() );

            ePinN = tN*2;
            QString pinp = reid+"pinP";
            m_ePin[ ePinN ] = new ePin( pinp.toStdString(), 1 );
            //qDebug() << "Switch::SetupSwitches ePin" <<tN<< ePinN<<pinp;

            pinpos = QPoint( 16,-16*i-8*j);
            pin = new Pin( 0, pinpos, reid+"pinN", 1, this);
            pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false ); // draw Pins on top

            m_pin[ cont ] = pin;
            m_ePin[ ePinN+1 ] = pin;

            //qDebug() << "Switch::SetupSwitches ePin"<<tN << ePinN+1<<reid+"pinN";
            //qDebug() << "Switch::SetupSwitches Pin:" << 2+cont<<reid+"-pinN";

            m_switches[ tN ]->setEpin( 0, m_ePin[ePinN] );
            m_switches[ tN ]->setEpin( 1, pin );

            //qDebug() << "Switch::SetupSwitches" << tN << pinN-2<< cont;
        }
        cont++;
    }

    //foreach( Pin* pin, m_pin )
    //    pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false ); // draw Pins on top
}

int Switch::poles() const
{ return m_numPoles; }

void Switch::setPoles( int poles )
{
    if( poles < 1 ) poles = 1;

    if( poles != m_numPoles )
        SetupSwitches( poles, m_numthrows );
}

bool Switch::dt() const
{ return (m_numthrows>1); }

void Switch::setDt( bool dt )
{
    int throws = 1;
    if( dt ) throws = 2;

    if( throws != m_numthrows )
        SetupSwitches( m_numPoles, throws );
}

bool Switch::nClose() const
{
    return m_nClose;
}

void Switch::setNClose( bool nc )
{
    m_nClose = nc;
    onbuttonclicked();
    //setSwitch( m_nClose );
}

void Switch::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );
    
    QPen pen = p->pen();
    pen.setWidth(3);
    p->setPen(pen);

    //if( m_closed ) p->drawLine(-10, 0, 10, -2 );
    //else           p->drawLine(-10.5, 0, 8, -8 );
    
    for( int i=0; i<m_numPoles; i++ )                           // Draw Switches
    {
        int offset = 16*i-16;

        if( m_closed )                                          // switch is closed
            p->drawLine(-10, -16-offset, 10, -18-offset );
        else                                                    // Switch is oppened
            p->drawLine(-10.5, -16-offset, 8, -24-offset );
    }
    if( m_numPoles > 1 )
    {
        pen.setStyle(Qt::DashLine);
        pen.setWidth(1);
        p->setPen(pen);
        p->drawLine(-0, 4, 0,-16*m_numPoles+4 );
    }
}

#include "moc_switch.cpp"
