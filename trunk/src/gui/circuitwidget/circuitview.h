/***************************************************************************
 *   Copyright (C) 2010 by santiago González                               *
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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include <QtGui>

class Circuit;
class Component;

class CircuitView : public QGraphicsView
{
    Q_OBJECT

    public:
        CircuitView( QWidget *parent );
        ~CircuitView();

        void clear();

        qreal scaleFactor() { return m_scalefactor; }

    public slots:

    private:
        void wheelEvent(QWheelEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);

        void keyPressEvent( QKeyEvent *event );
        void keyReleaseEvent( QKeyEvent *event );

        void resizeEvent(QResizeEvent *event);
        void scaleView(qreal scaleFactor);

        qreal      m_scalefactor;
        Component *m_enterItem;
        Circuit   *m_circuit;
        QString    m_file;
};

#endif
