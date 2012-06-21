/***************************************************************************
 *   Copyright ( C ) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   ( at your option ) any later version.                                   *
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

 #include <QtGui>

 #include "renderarea.h"

 RenderArea::RenderArea( QWidget *parent )
     : QWidget( parent )
 {
     antialiased = false;

     setBackgroundRole( QPalette::Base );
     setAutoFillBackground( true );

     m_width = 1000;
     m_height = 150;

     QFont font;
     font.setPointSize(7);
     pixmap = QPixmap( m_width, m_height );
     QPainter p( &pixmap );
     p.setFont( font );
     p.end();

     //drawBackground();

     for( int i=0; i<4; i++ )
     {
         m_data[i] = 0;
         m_dataP[i] = 0;
     }

     m_sec = 0;
 }

 /*void RenderArea::drawBackground()
 {
     pixmap.fill( QColor( 10, 15, 50 ) );

     QPainter p( &pixmap );

     if( antialiased )
     {
         p.setRenderHint( QPainter::Antialiasing, true );
         p.translate( +0.5, +0.5 );
     }
     p.setPen( QColor( 50, 50, 100 ) );

     for( int i=0; i<m_width; i+=20 )
     {
         p.drawLine( i, 20, i, m_height-21 );
     }
     p.end();
 }*/

 void RenderArea::drawVmark()
 {
     m_sec++;

     int last = m_width-1;
     QString sec = "";

     QPainter p( &pixmap );
     if( m_sec == 10 )
     {
         m_sec = 0;
         p.setPen( QColor( 90, 90, 150 ) );
     }
     else p.setPen( QColor( 50, 50, 100 ) );

     sec.setNum(m_sec);

     int base = m_height-20;

     p.drawLine( last, 5, last, base );
     p.drawText( last-10, base-5, 10, 20, Qt::AlignHCenter, sec );
     p.end();
 }

 QSize RenderArea::minimumSizeHint() const  {  return QSize( 100, 100 );  }

 QSize RenderArea::sizeHint() const  { return QSize( 400, 200 ); }

 void RenderArea::setPen( int channel, const QPen &pen )
 {
     this->m_pen[channel] = pen;
     //update();
 }

 void RenderArea::setBrush( const QBrush &brush )
 {
     this->brush = brush;
     update();
 }

 void RenderArea::setAntialiased( const bool antialiased )
 {
     this->antialiased = antialiased;
     update();
 }

 void RenderArea::setData( const int channel, int data )
 {
     data = data*(m_height-45)/500;
     data = m_height-35-data;
     m_dataP[channel] = m_data[channel];
     m_data[channel] = data;
 }

 void RenderArea::printData()
 {
     pixmap.scroll( -1, 0, pixmap.rect() );

     QPainter p( &pixmap );

     if( antialiased )
     {
         p.setRenderHint( QPainter::Antialiasing, true );
         p.translate( +0.5, +0.5 );
     }
     int last = m_width-1;

     //p.setPen( QColor( 50, 50, 100 ) );
     p.setPen( QColor( 10, 15, 50 ) );
     p.drawLine( last, 0, last, 149 ); // borra anterior

     p.setPen( QColor( 90, 90, 150 ) );
     p.drawLine( last-1, m_height-34, last, m_height-34 ); // Linea de base ( 0 )
     p.drawLine( last-1, m_height-33, last, m_height-33 );

     for( int i=0; i<4; i++ )
     {
         p.setPen( m_pen[i] );
         p.drawLine( last-1, m_dataP[i], last, m_data[i] );
         m_dataP[i] = m_data[i];
     }
     p.end();
 }

 void RenderArea::paintEvent( QPaintEvent * /* event */ )
 {
     QPainter painter( this );

     //painter.drawPixmap( 0, 0, pixmap );
     int origX = width()-m_width;
     //int origy = 0; //height(),
     painter.drawPixmap( origX, 0, /*m_width, height(),*/ pixmap );
     painter.end();
 }
