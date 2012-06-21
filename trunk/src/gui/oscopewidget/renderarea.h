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

 #ifndef RENDERAREA_H
 #define RENDERAREA_H

 #include <QBrush>
 #include <QPen>
 #include <QPixmap>
 #include <QWidget>

 class RenderArea : public QWidget
 {
     Q_OBJECT

     public:
         RenderArea( QWidget *parent = 0 );

         QSize minimumSizeHint() const;
         QSize sizeHint() const;

         void setData( const int channel, int data );
         void printData();
         void drawVmark();

     public slots:
         void setPen( const int channel, const QPen &pen );
         void setBrush( const QBrush &brush );
         void setAntialiased( const bool antialiased );

     protected:
         void paintEvent( QPaintEvent *event );

     private:
         //void drawBackground();
         QPen m_pen[4];
         QBrush brush;
         bool antialiased;
         QPixmap pixmap;

         int m_data[4];
         int m_dataP[4];
         int m_width;
         int m_height;
         int m_sec;
 };

 #endif

