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

#include "plotterwidget.h"

PlotterWidget* PlotterWidget::m_pSelf = 0l;

PlotterWidget::PlotterWidget(  QWidget *parent  )
    : QWidget( parent )
{
    m_pSelf = this;

    this->setVisible( false );

    setMinimumSize(QSize(200, 200));
    setMaximumSize(QSize(1000, 200));

    m_color[0] = QColor( 190, 190, 0 );
    m_color[1] = QColor( 240, 100, 10 );
    m_color[2] = QColor( 50, 120, 255 );
    m_color[3] = QColor( 30, 220, 150 );

    setupWidget();
    
    m_maxVolt = 500;
    m_minVolt = -500;
    m_offset  = 0;

    m_rArea->setAntialiased(true);

    m_ticksPs = 20;
    m_counter = 0;
    m_numchan = 0;

    for( int i=0; i<4; i++ )
    {
        m_data[i] = -1000;
        m_channel[i] = false;
    }
    m_numchan = 1;
    for( int i=0; i<1000; i++ ) step();
    m_numchan = 0;
}
PlotterWidget::~PlotterWidget(){ }

int PlotterWidget::addChannel()
{
    for( int i=0; i<4; i++ )                   // Find available channel
    {
        if( m_channel[i] == true ) continue;

        m_numchan++;                            // If channel available, inc used channels
        m_channel[i] = true;                    // Set channel to busy
        m_chanLabel[i]->setEnabled( true );      // Set channel label enabled
        m_chanLabel[i]->setText( " 0.00 V" );
        if( m_numchan > 0 ) setVisible( true ); // Set this visible if some channel active
        return i;                               // return channel number assigned
    }
    return -1;                                  // -1 = not channel available
}

void PlotterWidget::remChannel( int channel )
{
    if( channel < 0 || channel > 3 || m_channel[channel] == false ) return; // Nothing to do

    m_numchan--;                                // Decrease used channel
    if( m_numchan == 0 ) setVisible( false );   // Hide this if no channel active
    m_channel[channel] = false;                 // Set channel to not busy
    m_chanLabel[channel]->setEnabled( false );   // Disable channel label
    m_data[channel] = 0;                        // reset data
    m_chanLabel[channel]->setText( "--.-- V" );
    m_rArea->setData( channel, 0 );
}

QColor PlotterWidget::getColor( int channel )
{
    return m_color[channel];
}

void PlotterWidget::step()
{
    if( m_numchan == 0 ) return; // No data to plot

    if( ++m_counter == m_ticksPs )
    {
        m_counter = 0;
        m_rArea->drawVmark();
    }
    m_rArea->printData();
    m_rArea->update();
}

void PlotterWidget::setData( int channel, int data )
{
    if( data == m_data[channel] ) return;

    float vf = data;
    vf = vf/100;
    QString volt;
    volt.setNum( vf );
    if( !volt.contains(".") ) volt.append(".00");
    else if( volt.split(".").last().length() == 1 ) volt.append("0");
    volt.append( " V" );
    m_chanLabel[channel]->setText( volt );   // Update volt Label
    m_data[channel] = data;
    setRenderData( channel, data );
}

void PlotterWidget::setRenderData( int channel, int data )
{
    int renderData = data*1000/(m_maxVolt-m_minVolt)-m_offset;
    m_rArea->setData( channel, renderData );
}

void PlotterWidget::setTicksPs( int tps )
{
    m_ticksPs = tps;
}

void PlotterWidget::setPlotterTick( int tickUs )
{
    m_rArea->setTick( tickUs );
}

void PlotterWidget::maxChanged( double value )
{
    m_maxVolt = value*100;
    if( m_maxVolt <= m_minVolt ) 
    {
        m_maxVolt = m_minVolt+1;
        m_maxValue->setValue( m_maxVolt/100 );
    }
    setScale();
}

void PlotterWidget::minChanged( double value )
{
    m_minVolt = value*100;
    if( m_maxVolt <= m_minVolt ) 
    {
        m_minVolt = m_maxVolt-1;
        m_minValue->setValue( m_minVolt/100 );
    }
    setScale();
}

void PlotterWidget::setScale()
{
    m_offset = (m_maxVolt+m_minVolt)*500/(m_maxVolt-m_minVolt);
    m_rArea->setZero( -m_offset );
    
    for( int i=0; i<4; i++ ) 
        if( m_channel[i] ) setRenderData( i, m_data[i] );
}

void PlotterWidget::setupWidget()
{
    m_horizontalLayout = new QHBoxLayout( this );
    m_horizontalLayout->setObjectName(tr("horizontalLayout"));
    //m_horizontalLayout.setContentsMargins(0, 0, 0, 0);
    //m_horizontalLayout.setSpacing(0);
    m_verticalLayout = new QVBoxLayout();
    m_verticalLayout->setObjectName(tr("verticalLayout"));

    QFont font;
    font.setPointSize(12);

    for( int i=0; i<4; i++ )    // Create volt Labels
    {
        m_chanLabel[i] = new QLineEdit( this );
        m_chanLabel[i]->setObjectName(QString::fromUtf8("voltLabel"+i));
        m_chanLabel[i]->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        m_chanLabel[i]->setMaxLength(7);
        m_chanLabel[i]->setFont(font);
        m_chanLabel[i]->setAcceptDrops(false);
        m_chanLabel[i]->setReadOnly(true);
        m_chanLabel[i]->setText("--.-- V");
        m_chanLabel[i]->setFixedHeight(25);
        m_chanLabel[i]->setFixedWidth(85);
        m_chanLabel[i]->setVisible( true );
        m_chanLabel[i]->setEnabled( false );

        QPalette p = m_chanLabel[i]->palette();
        p.setColor( QPalette::Active, QPalette::Base, m_color[i] );
        m_chanLabel[i]->setPalette(p);
        m_verticalLayout->addWidget( m_chanLabel[i] );
    }
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_verticalLayout->addWidget(line);
    
    m_maxValue = new QDoubleSpinBox( this );
    m_maxValue->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    m_maxValue->setMaximum( 1000000 );
    m_maxValue->setMinimum(-1000000 );
    m_maxValue->setPrefix( "Max " );
    m_maxValue->setValue( 5 );
    m_verticalLayout->addWidget( m_maxValue );
    connect( m_maxValue, SIGNAL( valueChanged(double) ),
                   this, SLOT( maxChanged(double) ));
    
    m_minValue = new QDoubleSpinBox( this );
    m_minValue->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    m_minValue->setMaximum( 1000000 );
    m_minValue->setMinimum(-1000000 );
    m_minValue->setPrefix( "Min " );
    m_minValue->setValue( -5 );
    m_verticalLayout->addWidget( m_minValue );
    connect( m_minValue, SIGNAL( valueChanged(double) ),
                   this, SLOT( minChanged(double) ));
        
    m_horizontalLayout->addLayout( m_verticalLayout );

    m_rArea = new RenderArea( 1000, 180, this );
    m_rArea->setObjectName(tr("oscope"));

    QPen pen( m_color[0], 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

    for( int i=0; i<4; i++ )
    {
        pen.setColor( m_color[i] );
        m_rArea->setPen( i, pen );
    }
    m_horizontalLayout->addWidget(m_rArea);
}

#include "moc_plotterwidget.cpp"
