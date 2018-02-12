/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#include "oscopewidget.h"

OscopeWidget* OscopeWidget::m_pSelf = 0l;

OscopeWidget::OscopeWidget(  QWidget *parent  )
    : QWidget( parent )
{
    m_pSelf = this;

    this->setVisible( false );

    setMaximumSize(QSize(300, 200));
    setMinimumSize(QSize(300, 200));

    setupWidget();

    m_prevSpeed = 10;
    m_speed     = 10;
    m_prevOffset = 0;
    m_offset     = 0;
    
    m_ampli = 2.5;

    newReading = true;
    m_ticksPs = 20;
    m_counter = 0;
    m_tick    = 0;
    m_probe   = 0l;
    

}
OscopeWidget::~OscopeWidget(){ }

void OscopeWidget::setProbe( Probe* probe )
{
    m_probe = probe;
}

void OscopeWidget::clear()
{
    for( int i=0; i<140; i++ ) m_data[i] = 160;
    m_rArea->setData( m_data );
}

void OscopeWidget::step()
{
    if( m_counter < 140 ) 
    {
        //m_rArea->drawBackground();
        //m_rArea->update();
        return;
    }
    
    m_rArea->setTick( m_speed );
    m_rArea->setData( m_data );
    newReading = true;
    m_newReadCount = 0;
    m_counter = 0;
}

void OscopeWidget::setData()
{
    if( !m_probe ) return;
    if( m_counter == 140 ) return;

static double lastData;
static double max;
//static double mid;
static double min;
static bool   up;
static bool   down;
static int offset;

    //qDebug() << "OscopeWidget::setData" << m_speed << m_tick << m_counter << lastData << newReading;
    if( newReading ) // Starting a new reading set
    {
        double data = m_probe->getVolt();
        
        if( data > max ) max = data;
        if( data < min ) min = data;
        //qDebug() << "OscopeWidget::setData"<< lastData << data << max << min << up << down;

        if( (data-lastData)>0.2 )
        {
            if( up & down )
            {
                //mid = (max-min)/2;
                if( (data>=m_ampli) /*& ((data-m_ampli)<0.2)*/ )// Rising edge
                {
                    //qDebug() << "..."  << data << max << m_ampli << min;
                    m_tick = 0;
                    m_counter = 0;
                    up   = false;
                    down = false;
                    offset = 0;
                    max = 0;
                    min = 0;
                    //mid = 0;
                    newReading = false;
                }
                lastData = data;
                return;
            }
            up = true;
            lastData = data;
        }
        else if( (data-lastData) < -0.2 )
        {
            down = true;
            lastData = data;
        }

        if( ++m_newReadCount == 100000 )  // No reading: Clear Screen
        {
           m_newReadCount = 0;
           clear();
        }
        return;
    }
    if( offset < m_offset )
    {
        offset++;
    }
    else 
    {
        if( ++m_tick == m_speed )
        {
            double data = m_probe->getVolt();
      
            //data += mid; // Center data
            m_data[m_counter] = 160-data*28;
            m_counter++;
                
                //qDebug() << "m_tick == m_speed" << m_counter << data;

            m_tick = 0;
        }
    }
}

void OscopeWidget::setTicksPs( int tps )
{
    m_ticksPs = tps;
}

void OscopeWidget::setOscopeTick( int tickUs )
{
    m_rArea->setTick( tickUs );
}

void OscopeWidget::speedChanged( int speed )
{
    if( speed > m_prevSpeed ) 
    {
        m_speed++;
        if( m_speed > 2000 ) m_speed = 2000;
    }
    else
    {
        m_speed--;
        if( m_speed < 1 ) m_speed = 1;
    }
    
    m_prevSpeed = speed;

    m_tick = 0;
    m_counter = 0;
}
/*void OscopeWidget::ampliChanged( int ampli )
{
    m_ampli = ampli/10;
}*/

void OscopeWidget::centeChanged( int offset )
{
    if( offset < m_prevOffset ) 
    {
        m_offset += m_speed;
        if( m_offset > 200*m_speed ) m_offset = 200*m_speed;
    }
    else
    {
        m_offset -= m_speed;
        if( m_offset < 0 ) m_offset = 0;
    }
    
    m_prevOffset = offset;
}

void OscopeWidget::setupWidget()
{
    m_horizontalLayout = new QHBoxLayout( this );
    m_horizontalLayout->setObjectName( "horizontalLayout" );
    //m_horizontalLayout.setContentsMargins(0, 0, 0, 0);
    //m_horizontalLayout.setSpacing(0);
    
    m_verticalLayout = new QVBoxLayout();
    m_verticalLayout->setObjectName( "verticalLayout" );
    
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_verticalLayout->addWidget(line);
    
    m_speedDial = new QDial(this);
    m_speedDial->setObjectName( "speedDial" );
    m_speedDial->setNotchesVisible(true);
    m_speedDial->setWrapping(true);
    m_speedDial->setMinimum(1);
    m_speedDial->setMaximum(20);
    //m_speedDial->setValue(10);
    m_speedDial->setSingleStep(1);
    m_verticalLayout->addWidget( m_speedDial );
    
    QLabel *speedLabel = new QLabel( "Tick", this );
    speedLabel->setAlignment( Qt::AlignBottom | Qt::AlignHCenter );
    QFont font = speedLabel->font();
    font.setPointSize(9);
    font.setBold(true);
    speedLabel->setFont(font);
    m_verticalLayout->addWidget( speedLabel );
    
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_verticalLayout->addWidget(line);
    
    /*m_ampliDial = new QDial(this);
    m_ampliDial->setObjectName(QString::fromUtf8("ampliDial"));
    m_ampliDial->setNotchesVisible(true);
    m_ampliDial->setMinimum(1);
    m_ampliDial->setMaximum(100);
    m_ampliDial->setValue(25);
    m_ampliDial->setSingleStep(1);
    m_verticalLayout->addWidget( m_ampliDial );*/
    
    m_centeDial = new QDial(this);
    m_centeDial->setObjectName( "centeDial" );
    m_centeDial->setNotchesVisible(true);
    m_centeDial->setWrapping(true);
    m_centeDial->setMinimum(1);
    m_centeDial->setMaximum(20);
    m_centeDial->setSingleStep(1);
    m_verticalLayout->addWidget( m_centeDial );
    
    QLabel *centeLabel = new QLabel( "H-Center", this );
    centeLabel->setAlignment( Qt::AlignBottom | Qt::AlignHCenter );
    centeLabel->setFont(font);
    m_verticalLayout->addWidget( centeLabel );
    
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_verticalLayout->addWidget(line);

    m_horizontalLayout->addLayout( m_verticalLayout );

    m_rArea = new RenderOscope( 180, 180, this );
    m_rArea->setObjectName(tr("oscope"));

    m_horizontalLayout->addWidget(m_rArea);
    
    connect( m_speedDial, SIGNAL( valueChanged(int) ),
             this,        SLOT  ( speedChanged(int)) );
             
    /*connect( m_ampliDial, SIGNAL( valueChanged(int) ),
             this,        SLOT  ( ampliChanged(int)) );*/
             
    connect( m_centeDial, SIGNAL( valueChanged(int) ),
             this,        SLOT  ( centeChanged(int)) );
}

#include "moc_oscopewidget.cpp"
