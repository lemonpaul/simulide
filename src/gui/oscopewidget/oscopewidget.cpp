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

#include "oscopewidget.h"
#include "simulator.h"
#include "oscope.h"

#include <math.h>   // fabs(x,y)

OscopeWidget::OscopeWidget(  QWidget *parent  )
    : QWidget( parent ),
      eElement( "oscope" )
{
    this->setVisible( false );

    setMaximumSize(QSize(300, 200));
    setMinimumSize(QSize(300, 200));

    m_prevHscale = 10;
    m_Hscale     = 10;
    m_prevHpos = 0;
    m_Hpos     = 0;
    
    m_prevVscale = 1;
    m_Vscale     = 1;
    m_prevVpos = 0;
    m_Vpos     = 0;
    
    m_ampli = 0;
    m_filter = 0.3;

    newReading = true;
    m_auto = true;
    
    m_newReadCount = 0;
    m_stepCount = 0;
    m_updtCount = 0;
    m_counter = 0;
    m_tick    = 0;
    m_probe   = 0l;
    m_oscope  = 0l;
}
OscopeWidget::~OscopeWidget()
{ 
    Simulator::self()->remFromSimuClockList( this );
}

void OscopeWidget::initialize()
{
    clear();
}

void OscopeWidget::setProbe( Probe* probe )
{
    if( m_probe == probe ) return;
    m_probe = probe;
    if( probe == 0l ) Simulator::self()->remFromSimuClockList( this );
    else              Simulator::self()->addToSimuClockList( this );
}

void OscopeWidget::setOscope( Oscope* oscope )
{
    if( m_oscope == oscope ) return;
    m_oscope = oscope;
    if( oscope == 0l ) Simulator::self()->remFromSimuClockList( this );
    else              Simulator::self()->addToSimuClockList( this );
}

void OscopeWidget::clear()
{
    for( int i=0; i<140; i++ ) m_data[i] = 2.5*28;
    m_display->setData( m_data );
    
    newReading = true;
    
    m_newReadCount = 0;
    m_stepCount = 0;
    m_updtCount = 0;
    m_counter = 0;
    
    m_freqLabel->setText( "Freq: 000 Hz" );
    m_ampLabel->setText( "Amp: 0.00 V" );
}

void OscopeWidget::simuClockStep()
{
    if( ++m_newReadCount == 1000500 )
    {
        m_ampli = 0;
        m_newReadCount = 0;
        clear();
    }
    
    double data = 0;
    if     ( m_probe != 0l )  data = m_probe->getVolt();
    else if( m_oscope != 0l ) data = m_oscope->getVolt();

//qDebug() << "OscopeWidget::setData"<< lastData << data << max << min << up << down;
    
    if( data > max ) max = data;
    if( data < min ) min = data;
    
    if( (data-lastData)>m_filter )                       // Filter noise 
    {
        if( newReading )
        {
            mid = min + m_ampli/2;
            
            if( data>=mid )                               // Rising edge
            {
                int per = 1e6/m_freq;
                if( per > 1 )
                {
                    if( m_auto ) 
                    {
                        m_Vpos = mid;
                        m_Hpos = 0;
                        m_Vscale = 5/m_ampli;
                        if( m_Vscale > 1000 ) m_Vscale = 1000;
                        if( m_Vscale < 0.001 ) m_Vscale = 0.001;
                        
                        m_Hscale = (abs(per/70)+1);
                        if( m_Hscale > 10000 ) m_Hscale = 10000;
                        if( m_Hscale < 1 )    m_Hscale = 1;
                    }
//qDebug()<<"OscopeWidget::simuClockStep"<<max<<min <<m_ampli << m_Vscale;
//qDebug()<<"OscopeWidget::simuClockStep" <<m_sampleR <<per << m_freq;
                    
                    Hpos = 0;
                    m_tick = 0;
                    m_counter = 0;
                    m_newReadCount = 0;
                    
                    newReading = false;
                }
            }
        }
        if( down & !up )                                    // Min Found
        {
            m_ampli = max-min;
            m_display->setMaxMin( max, min );
            down = false;
            max = -1e12;
        }
        up = true;
        lastData = data;
    }
    else if( (data-lastData) < -m_filter )
    {
        if( up & !down )                                    // Max Found
        {
            m_numMax++;
            m_ampli = max-min;
            m_display->setMaxMin( max, min );
            up = false;
            min = 1e12;
        }
        down = true;
        lastData = data;
    }

    if( ++m_stepCount == 50000 )                          // 5 ms Update
    {
        m_stepCount = 0;
        
        double tick = 20*m_Hscale;
        double val = tick/1e6;
        QString unit = " S";
        
        if( val < 1 )
        {
            unit = " mS";
            val = tick/1e3;
            if( val < 1 )
            {
                unit = " uS";
                val = tick;
            }
        }
        m_tickLabel->setText( "Div:  "+QString::number( val,'f', 2)+unit );
        m_ampLabel->setText(  "Amp: " +QString::number( m_ampli,'f', 2)+" V" );

        if( ++m_updtCount >= 20 )                        // 1 Seg Update
        {
            m_freq = m_numMax; 
            m_updtCount = 0;
            m_numMax = 0;
            m_freqLabel->setText( "Freq: "+QString::number(m_freq)+" Hz" );
        }
        if( m_counter == 140 )              // Data set Ready to display
        {
            m_display->setData( m_data );
            newReading = true;
            m_counter = 0;
            return; 
        }
    }

    if( newReading == false )                         // Data Set saving
    {
        if( m_counter == 140 ) return;          // Done, Wait for update

        if( Hpos < m_Hpos ) Hpos++;                 // Wait for H offset
        else                                          // Actual Data Set 
        {
            if( ++m_tick == m_Hscale )
            {
                m_data[m_counter] = ((data-m_Vpos)*m_Vscale+2.5)*28;
                //qDebug() << "data"<<data;
                m_counter++;
                m_tick = 0;
                if( m_counter == 140 )
                {
                    down = false;
                    up = false;
                }
            }
        }
    }
}

void OscopeWidget::HscaleChanged( int Hscale )
{
    if( m_auto ) return;
    
    if( Hscale < m_prevHscale ) 
    {
        m_Hscale++;
        if( m_Hscale > 10000 ) m_Hscale = 10000;
    }
    else
    {
        m_Hscale--;
        if( m_Hscale < 1 ) m_Hscale = 1;
    }
    m_prevHscale = Hscale;
}
void OscopeWidget::VscaleChanged( int Vscale )
{
    if( m_auto ) return;
    
    double vscale = (double)Vscale;
    if( vscale > m_prevVscale ) 
    {
        m_Vscale *= 1.005;
        if( m_Vscale > 1000 ) m_Vscale = 1000;
    }
    else
    {
        m_Vscale /= 1.005;
        if( m_Vscale < 0.001 ) m_Vscale = 0.001;
    }
    m_prevVscale = vscale;
}

void OscopeWidget::HposChanged( int Hpos )
{
    if( m_auto ) return;
    
    if( Hpos < m_prevHpos ) 
    {
        m_Hpos += m_Hscale;
        if( m_Hpos > 200*m_Hscale ) m_Hpos = 200*m_Hscale;
    }
    else
    {
        m_Hpos -= m_Hscale;
        if( m_Hpos < 0 ) m_Hpos = 0;
    }
    m_prevHpos = Hpos;
}

void OscopeWidget::VposChanged( int Vpos )
{
    if( m_auto ) return;
    
    double vpos = (double)Vpos;
    
    if( vpos < m_prevVpos ) 
    {
        m_Vpos += 0.01*m_Vscale;
    }
    else
    {
        m_Vpos -= 0.01*m_Vscale;
    }
    m_prevVpos = vpos;
}

void OscopeWidget::autoChanged( int au )
{
    m_auto = au;
}

void OscopeWidget::setupWidget(  int size  )
{
    m_horizontalLayout = new QHBoxLayout( this );
    m_horizontalLayout->setObjectName( "horizontalLayout" );
    m_horizontalLayout->setContentsMargins(2, 2, 2, 2);
    //m_horizontalLayout.setSpacing(0);
    
    m_verticalLayout = new QVBoxLayout();
    m_verticalLayout->setObjectName( "verticalLayout" );
    m_verticalLayout->setContentsMargins(0, 0, 0, 0);
    m_verticalLayout->setSpacing(0);
    
    m_freqLabel = new QLabel( "Freq: 0 Hz", this );
    
    QFont font = m_freqLabel->font();
    font.setPointSize(8);
    font.setBold(true);
    
    m_freqLabel->setFont(font);
    m_freqLabel->setFixedSize( 85, 14 );
    m_verticalLayout->addWidget(m_freqLabel);
    
    m_ampLabel  = new QLabel( "Amp: 0.00 V", this );
    m_ampLabel->setFont(font);
    m_ampLabel->setFixedSize( 85, 14 );
    m_verticalLayout->addWidget(m_ampLabel);
    
    m_tickLabel  = new QLabel( "Div:  0 S", this );
    m_tickLabel->setFont(font);
    m_tickLabel->setFixedSize( 85, 14 );
    m_verticalLayout->addWidget(m_tickLabel);
    
    QHBoxLayout* row2Layout = new QHBoxLayout();
    row2Layout->setObjectName( "row2Layout" );
    row2Layout->setContentsMargins(0, 0, 0, 0);
    row2Layout->setSpacing(0);
    
    m_autoCheck = new QCheckBox( "Auto", this );
    //m_autoCheck->setLayoutDirection(Qt::RightToLeft);
    m_autoCheck->setChecked( true );
    m_autoCheck->setFixedSize( 38, 16 );
    font.setPointSize(7);
    m_autoCheck->setFont( font );
    row2Layout->addWidget( m_autoCheck );
    
    QLabel* HLabel = new QLabel( "H", this );
    HLabel->setAlignment( Qt::AlignBottom | Qt::AlignHCenter );
    HLabel->setFixedSize( 14, 16 );
    font.setPointSize(8);
    HLabel->setFont( font );
    row2Layout->addWidget( HLabel );
    
    QLabel* VLabel = new QLabel( "V", this );
    VLabel->setAlignment( Qt::AlignBottom | Qt::AlignHCenter );
    VLabel->setFixedSize( 30, 16 );
    VLabel->setFont( font );
    row2Layout->addWidget( VLabel );
    
    m_verticalLayout->addLayout( row2Layout );
    
    /*QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_verticalLayout->addWidget(line);*/
    
    QHBoxLayout* row0Layout = new QHBoxLayout();
    row0Layout->setObjectName( "row0Layout" );
    row0Layout->setContentsMargins(0, 0, 0, 0);
    row0Layout->setSpacing(0);
    
    QLabel* scaleLabel = new QLabel( "Scale", this );
    scaleLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    scaleLabel->setFixedSize( 32, 29 );
    scaleLabel->setFont( font );
    row0Layout->addWidget( scaleLabel );
    
    m_HscaleDial = new QDial(this);
    m_HscaleDial->setObjectName( "HscaleDial" );
    m_HscaleDial->setFixedSize( 32, 29 );
    m_HscaleDial->setNotchesVisible( true );
    m_HscaleDial->setWrapping(true);
    m_HscaleDial->setMinimum( 1 );
    m_HscaleDial->setMaximum( 20 );
    //m_HscaleDial->setValue( 10 );
    m_HscaleDial->setSingleStep( 1 );
    row0Layout->addWidget(m_HscaleDial );
    
    m_VscaleDial = new QDial(this);
    m_VscaleDial->setObjectName( "VscaleDial" );
    m_VscaleDial->setFixedSize( 32, 29 );
    m_VscaleDial->setNotchesVisible( true );
    m_VscaleDial->setWrapping( true );
    m_VscaleDial->setMinimum( 1 );
    m_VscaleDial->setMaximum( 20 );
    //m_VscaleDial->setValue( 25 );
    m_VscaleDial->setSingleStep( 1 );
    row0Layout->addWidget( m_VscaleDial );    
    
    m_verticalLayout->addLayout( row0Layout );
    
    QHBoxLayout* row1Layout = new QHBoxLayout();
    row1Layout->setObjectName( "row1Layout" );
    row1Layout->setContentsMargins(0, 0, 0, 0);
    row1Layout->setSpacing(0);
    
    QLabel* posLabel = new QLabel( "Pos", this );
    posLabel->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    posLabel->setFixedSize( 32, 29 );
    posLabel->setFont(font);
    row1Layout->addWidget( posLabel );
    
    m_HposDial = new QDial(this);
    m_HposDial->setObjectName( "HposDial" );
    m_HposDial->setFixedSize( 32, 29 );
    m_HposDial->setNotchesVisible( true );
    m_HposDial->setWrapping( true );
    m_HposDial->setMinimum( 1 );
    m_HposDial->setMaximum( 20 );
    m_HposDial->setSingleStep( 1 );
    row1Layout->addWidget( m_HposDial );
    
    m_VposDial = new QDial(this);
    m_VposDial->setObjectName( "VposDial" );
    m_VposDial->setFixedSize( 32, 29 );
    m_VposDial->setNotchesVisible( true );
    m_VposDial->setWrapping( true );
    m_VposDial->setMinimum( 1 );
    m_VposDial->setMaximum( 20 );
    m_VposDial->setSingleStep( 1 );
    row1Layout->addWidget( m_VposDial );
    
    m_verticalLayout->addLayout( row1Layout );

    m_horizontalLayout->addLayout( m_verticalLayout );

    m_display = new RenderOscope( size, size, this );
    m_display->setObjectName( "oscope" );
    
    m_horizontalLayout->addWidget( m_display );
    m_horizontalLayout->setAlignment( m_display, Qt::AlignRight );
    
    
    connect(m_HscaleDial, SIGNAL( valueChanged(int) ),
             this,        SLOT  ( HscaleChanged(int)) );
             
    connect( m_VscaleDial, SIGNAL( valueChanged(int) ),
             this,         SLOT  ( VscaleChanged(int)) );
             
    connect( m_HposDial, SIGNAL( valueChanged(int) ),
             this,       SLOT  ( HposChanged(int)) );
             
    connect( m_VposDial, SIGNAL( valueChanged(int) ),
             this,       SLOT  ( VposChanged(int)) );
             
    connect( m_autoCheck, SIGNAL( stateChanged(int) ),
             this,        SLOT  ( autoChanged(int)) );
}

#include "moc_oscopewidget.cpp"
