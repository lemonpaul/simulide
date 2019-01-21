/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "relay_base.h"
#include "simulator.h"
#include "circuit.h"

#include <math.h>

static const char* RelayBase_properties[] = {
    QT_TRANSLATE_NOOP("App::Property","Rcoil"),
    QT_TRANSLATE_NOOP("App::Property","Itrig"),
    QT_TRANSLATE_NOOP("App::Property","Poles"),
    QT_TRANSLATE_NOOP("App::Property","DT"),
    QT_TRANSLATE_NOOP("App::Property","Norm Close")
};

RelayBase::RelayBase( QObject* parent, QString type, QString id )
         : Component( parent, type, id )
         , eInductor( id.toStdString() )
{
    Q_UNUSED( RelayBase_properties );
    
    // This represents a coil, so is an Inductor in series with a Resistor: -Ind-Nod-Res-
    // We need to create the resistor, internal eNode and do the connections.

    m_numthrows = 0;
    m_numPoles = 0;
    m_nClose = false;
    m_closed = false;

    m_ePin.resize(4);
    m_pin.resize(2);

    QString nodid = m_id;            // External Left pin to inductor
    nodid.append(QString("-lPin"));
    QPoint nodpos = QPoint(-16-8,0);
    m_pin[0] = new Pin( 180, nodpos, nodid, 0, this);
    m_pin[0]->setLength(4.5);
    m_pin[0]->setPos(-16, 0 );
    m_ePin[0] = m_pin[0];

    nodid = m_id;                    // External Right pin to resistor
    nodid.append(QString("-rPin"));
    nodpos = QPoint(16+8,0);
    m_pin[1] = new Pin( 0, nodpos, nodid, 1, this);
    m_pin[1]->setLength(4.5);
    m_pin[1]->setPos( 16, 0 );
    m_ePin[3] = m_pin[1];

    QString reid = m_id;
    reid.append(QString("-resistor"));
    m_resistor = new eResistor( reid.toStdString() );

    nodid = reid;                  // Internal Left pin to inductor
    nodid.append(QString("-lIntPin"));
    m_ePin[1] = new ePin( reid.toStdString(), 1 );

    nodid = reid;                 // Internal right pin to resistor
    nodid.append(QString("-rIntPin"));
    m_ePin[2] = new ePin( reid.toStdString(), 2 );

    m_resistor->setEpin( 0, m_ePin[2] );
    m_resistor->setEpin( 1, m_ePin[3] );

    m_resistor->setRes( 100 );
    m_trigCurrent = 0.02;

    m_internalEnode = new eNode( m_id+"-internaleNode" );
    m_ePin[1]->setEnode( m_internalEnode );
    m_ePin[2]->setEnode( m_internalEnode );

    setInd( 0.1 );  // 100 mH
    //m_unit = "H";
    //setInduc( m_ind );
    setValLabelPos(-16, 6, 0);
    setShowVal( true );

    setLabelPos(-16,-24, 0);

}
RelayBase::~RelayBase()
{
}

void RelayBase::initialize()
{
    eInductor::initialize();

    for( int i=0; i<m_numPoles; i++ )
    {
        eNode* node = m_pin[2+i*(1+m_numthrows)]->getEnode();

        int epinN = 4+i*m_numthrows*2;
        m_ePin[ epinN ]->setEnode( node );

        if( m_numthrows > 1 )
            m_ePin[ epinN+2 ]->setEnode( node );
    }
    for( uint i=4; i<m_ePin.size(); i++ )
    {
        eNode* node = m_ePin[i]->getEnode();
        
        if( node ) node->setSwitched( true );
    }

    setSwitch( m_nClose );
}

void RelayBase::setVChanged()
{
    eInductor::setVChanged();

    bool closed = ( fabs(m_curSource) > m_trigCurrent );
    
    if( m_nClose ) closed = !closed;

    if( closed != m_closed ) setSwitch( closed );
}

void RelayBase::setSwitch( bool closed )
{
    m_closed = closed;

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

void RelayBase::remove()
{
    Simulator::self()->remFromEnodeList( m_internalEnode, true );

    foreach( eResistor* res, m_switches ) delete res;
    delete m_resistor;

    Component::remove();
}

void RelayBase::SetupSwitches( int poles, int throws )
{
    m_area = QRectF( -13, -8-16*poles-4, 26, 8+16*poles+8+4 );

    for( uint i=0; i<m_switches.size(); i++ )
        delete m_switches[i];
    //qDebug() << "RelayBase::SetupSwitches Pins:"<<poles<<throws<<m_numPoles<<m_numthrows;
    for( int i=0; i<m_numPoles; i++ )
    {
        int epinN = 4+i*m_numthrows*2;
        //qDebug() << "RelayBase::SetupSwitches ePins:"<<epinN;
        delete m_ePin[ epinN ];
        //qDebug() << "RelayBase::SetupSwitches ePins:"<<epinN+2;
        if( m_numthrows > 1 ) delete m_ePin[ epinN+2 ];
    }

    for( uint i=2; i<m_pin.size(); i++ )
    {
        Pin* pin = m_pin[i];
        if( pin->isConnected() ) pin->connector()->remove();
        pin->reset();
        delete pin;
    }

    m_numPoles = poles;
    m_numthrows = throws;

    m_switches.resize( poles*throws );
    m_pin.resize( 2+poles+poles*throws);

    m_ePin.resize(4+2*poles*throws);
//qDebug() << "RelayBase::SetupSwitches" << poles+poles*throws;

    int cont = 0;
    for( int i=0; i<poles; i++ )              // Create Resistors
    {
        Pin* pin;
        int pinN = 2+cont;
        int ePinN = 4+cont;
        QString reid = m_id;

        QPoint pinpos = QPoint(-16,-16-16*i );
        pin = new Pin( 180, pinpos, reid+"-pinP"+QString::number(pinN), 0, this);
        m_pin[pinN] = pin;

        //qDebug() << "RelayBase::SetupSwitches Pin:" << pinN<<reid+"-pinP"+QString::number(pinN);

        for( int j=0; j<throws; j++ )
        {
            reid = m_id;

            cont++;
            int tN = i*throws+j;

            reid.append( QString( "-switch"+QString::number(tN)) );
            m_switches[ tN ] = new eResistor( reid.toStdString() );

            ePinN = 4+tN*2;
            QString pinp = reid+"pinP";
            m_ePin[ ePinN ] = new ePin( pinp.toStdString(), 1 );
            //qDebug() << "RelayBase::SetupSwitches ePin" <<tN<< ePinN<<pinp;

            pinpos = QPoint( 16,-16-16*i-8*j);
            pin = new Pin( 0, pinpos, reid+"pinN", 1, this);

            m_pin[ 2+cont ] = pin;
            m_ePin[ ePinN+1 ] = pin;

            //qDebug() << "RelayBase::SetupSwitches ePin"<<tN << ePinN+1<<reid+"pinN";
            //qDebug() << "RelayBase::SetupSwitches Pin:" << 2+cont<<reid+"-pinN";

            m_switches[ tN ]->setEpin( 0, m_ePin[ePinN] );
            m_switches[ tN ]->setEpin( 1, pin );

            //qDebug() << "RelayBase::SetupSwitches" << tN << pinN-2<< cont;
        }
        cont++;
    }

    foreach( Pin* pin, m_pin )
        pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false ); // draw Pins on top
}

double RelayBase::rCoil() const
{ return m_resistor->res(); }

void RelayBase::setRCoil(double res)
{ if (res > 0.0f) m_resistor->setResSafe(res); }

double RelayBase::iTrig() const
{ return m_trigCurrent; }

void RelayBase::setITrig( double current )
{ if (current > 0.0f) m_trigCurrent = current; }

int RelayBase::poles() const
{ return m_numPoles; }

void RelayBase::setPoles( int poles )
{
    if( poles < 1 ) poles = 1;

    if( poles != m_numPoles )
        SetupSwitches( poles, m_numthrows );
}

bool RelayBase::dt() const
{ return (m_numthrows>1); }

void RelayBase::setDt( bool dt )
{
    int throws = 1;
    if( dt ) throws = 2;

    if( throws != m_numthrows )
        SetupSwitches( m_numPoles, throws );
}

bool RelayBase::nClose() const
{
    return m_nClose;
}

void RelayBase::setNClose( bool nc )
{
    m_nClose = nc;
    setSwitch( m_nClose );
}


void RelayBase::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Component::paint( p, option, widget );

    p->setBrush(Qt::white);
    p->drawRect( m_area );

    QPen pen = p->pen();                                       // Draw Coil
    pen.setWidth(2.8);
    p->setPen(pen);

    QRectF rectangle(-12,-4.5, 10, 10 );
    int startAngle = -45 * 16;
    int spanAngle = 220 * 16;
    p->drawArc(rectangle, startAngle, spanAngle);

    QRectF rectangle2(-5,-4.5, 10, 10 );
    startAngle = 225 * 16;
    spanAngle = -270 * 16;
    p->drawArc(rectangle2, startAngle, spanAngle);

    QRectF rectangle3(2,-4.5, 10, 10 );
    startAngle = 225 * 16;
    spanAngle = -220 * 16;
    p->drawArc(rectangle3, startAngle, spanAngle);

    pen.setWidth(3);
    p->setPen(pen);

    for( int i=0; i<m_numPoles; i++ )                           // Draw Switches
    {
        int offset = 16*i;

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
        p->drawLine(-0, -12, 0, -12-16*m_numPoles+4 );
    }
}
