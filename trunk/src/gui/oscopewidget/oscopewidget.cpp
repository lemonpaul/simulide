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

#include "oscopewidget.h"

OscopeWidget* OscopeWidget::m_pSelf = 0l;

OscopeWidget::OscopeWidget(  QWidget *parent  )
    : QWidget( parent )
{
    m_pSelf = this;

    this->setVisible( false );

    setMinimumSize(QSize(200, 150));

    m_color[0] = QColor( 170, 170, 0 );
    m_color[1] = QColor( 255, 120, 50 );
    m_color[2] = QColor( 50, 120, 255 );
    m_color[3] = QColor( 30, 220, 150 );

    setupWidget();

    //m_oscope->setAntialiased(true);

    m_ticksPs = 20;
    m_counter = 0;
    m_numchan = 0;

    for( int i=0; i<4; i++ )
    {
        m_data[i] = 0;
        m_channel[i] = false;
    }
}
OscopeWidget::~OscopeWidget(){ }

int OscopeWidget::addChannel()
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

void OscopeWidget::remChannel( int channel )
{
    if( channel < 0 || channel > 3 || m_channel[channel] == false ) return; // Nothing to do

    m_numchan--;                                // Decrease used channel
    if( m_numchan == 0 ) setVisible( false );   // Hide this if no channel active
    m_channel[channel] = false;                 // Set channel to not busy
    m_chanLabel[channel]->setEnabled( false );   // Disable channel label
    m_data[channel] = 0;                        // reset data
    m_chanLabel[channel]->setText( "--.-- V" );
    m_oscope->setData( channel, 0 );
}

QColor OscopeWidget::getColor( int channel )
{
    return m_color[channel];
}

void OscopeWidget::step()
{
    if( m_numchan == 0 ) return; // No data to plot

    if( ++m_counter == m_ticksPs )
    {
        m_counter = 0;
        m_oscope->drawVmark();
    }
    m_oscope->printData();
    m_oscope->update();
}

void OscopeWidget::setData( int channel, int data )
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
    m_oscope->setData( channel, data );
}

void OscopeWidget::setTicksPs( int tps )
{
    m_ticksPs = tps;
}

void OscopeWidget::setupWidget()
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
        m_chanLabel[i]->setGeometry(QRect(0, 0, 80, 27));
        m_chanLabel[i]->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        m_chanLabel[i]->setMaxLength(7);
        m_chanLabel[i]->setFont(font);
        m_chanLabel[i]->setAcceptDrops(false);
        m_chanLabel[i]->setReadOnly(true);
        m_chanLabel[i]->setText("--.-- V");
        //m_chanLabel[i]->setFixedHeight(30);
        m_chanLabel[i]->setFixedWidth(75);
        m_chanLabel[i]->setVisible( true );
        m_chanLabel[i]->setEnabled( false );

        QPalette p = m_chanLabel[i]->palette();
        p.setColor( QPalette::Active, QPalette::Base, m_color[i] );
        //p.setColor( QPalette::Active, QPalette::Text, QColor( 10, 15, 50 ));
        m_chanLabel[i]->setPalette(p);
        m_verticalLayout->addWidget( m_chanLabel[i] );
    }
    m_horizontalLayout->addLayout( m_verticalLayout );

    //m_value.setObjectName(tr("value"));
    //m_value.setFixedSize( 30, 150 );
    //m_horizontalLayout.addWidget(&m_value);

    m_oscope = new RenderArea( this );
    m_oscope->setObjectName(tr("oscope"));
    m_oscope->setFixedHeight( 150 );
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_oscope->setSizePolicy(sizePolicy);

    QPen pen( m_color[0], 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

    for( int i=0; i<4; i++ )
    {
        pen.setColor( m_color[i] );
        m_oscope->setPen( i, pen );
    }

    m_horizontalLayout->addWidget(m_oscope);

    //QGridLayout* lo = new QGridLayout(this);
    //lo->addLayout( &m_horizontalLayout, 0, 0 );
}

