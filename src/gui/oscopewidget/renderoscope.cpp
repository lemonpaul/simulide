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

 #include "renderoscope.h"

RenderOscope::RenderOscope( int width, int height, QWidget *parent )
            : QWidget( parent )
{
    //setBackgroundRole( QPalette::Base );
    //setAutoFillBackground( true );

    m_width  = width;
    m_height = height;

    m_pixmap = QPixmap( m_width, m_height );
    
    drawBackground();
}

void RenderOscope::drawBackground()
{
    m_pixmap.fill( QColor( 220, 220, 220 ) );
    QPainter p( &m_pixmap );
    p.setRenderHint( QPainter::Antialiasing, true );
    
    p.setBrush( QColor( 10, 15, 50 ) );
    p.drawRoundRect(0, 0, 180, 180 );

    QPen pen( QColor( 90, 90, 180 ), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
    p.setPen( pen );

    p.drawLine( 20, 90, 159, 90 );
    p.drawLine( 90, 20, 90, m_height-21 );
    
    p.setPen( QColor( 70, 70, 140 ) );
    
    for( int i=30; i<160; i+=20 )
    {
        p.drawLine( i, 20, i, m_height-21 );
    }
    for( int i=20; i<161; i+=28 )
    {
        p.drawLine( 20, i, 160, i );
    }
    p.setPen( QColor( 200, 200, 255 ) );
    for( int i=0; i<6; i++ )
    {
        int y = 15+i*28;
        p.drawText( 5  , y-3, 20 , y+3, Qt::AlignLeft, QString::number(5-i) );
        p.drawText( 166, y-3, 175, y+3, Qt::AlignLeft, QString::number(5-i) );
    }
    
    
}

QSize RenderOscope::minimumSizeHint() const  {  return QSize( 180, 180 );  }

QSize RenderOscope::sizeHint() const  { return QSize( 180, 180 ); }


void RenderOscope::setData( int data[] )
{
    drawBackground();
    
    QPainter p( &m_pixmap );

    p.setRenderHint( QPainter::Antialiasing, true );
    p.setRenderHint( QPainter::SmoothPixmapTransform, true );
    
    QPen pen( QColor( 240, 240, 100 ), 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
    
    p.setPen( pen );
    
    int lastPoint = data[0];
    for( int i=1; i<140; i++ )
    {
        int point = data[i];
        int x = i+20;
        p.drawLine( x-1, lastPoint, x, point );
        lastPoint = point;
    }
    QFont font;
    font.setPointSize(9);
    p.setFont( font );

    p.setPen( QColor( 200, 200, 255 ) );
    p.drawText( 10, 2, 160, 20, Qt::AlignHCenter, "Tick: "+m_tick );
    p.end();
    update();
}

void RenderOscope::setTick( int tickUs )
{
    double tick = 20*tickUs;
    
    QString unit = " S";
    
    double temp = tick/1e6;
    
    if( temp < 1 )
    {
        unit = " mS";
        temp = tick/1e3;
        if( temp < 1 )
        {
            unit = " uS";
            temp = tick;
        }
    }
    m_tick.setNum(temp);
    m_tick += unit;
}

void RenderOscope::paintEvent( QPaintEvent * /* event */ )
{
    QPainter painter( this );

    painter.drawPixmap( 0, 0, m_pixmap );
    
    painter.end();
}

#include "moc_renderoscope.cpp"
