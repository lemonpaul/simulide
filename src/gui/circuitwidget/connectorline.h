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
 
#ifndef CONNECTORLINE_H
#define CONNECTORLINE_H

#include <QtWidgets>

#include "connector.h"

class Connector;
 
 class MAINMODULE_EXPORT ConnectorLine : public QGraphicsObject
{
    Q_OBJECT

public:
    ConnectorLine( int x1, int y1, int x2, int y2, Connector*  connector );
    ~ConnectorLine();

    virtual QRectF boundingRect() const;

    void setConnector( Connector* con ) { m_pConnector = con; }

    void setP1( QPoint );
    void setP2( QPoint );

    QPoint p1() { return QPoint( m_p1X, m_p1Y ); }
    QPoint p2() { return QPoint( m_p2X, m_p2Y ); }

    int dx() { return (m_p2X - m_p1X);}
    int dy() { return (m_p2Y - m_p1Y);}

    Connector* connector() const { return m_pConnector; }

    void move( QPointF delta );

    void moveLine( QPoint delta );

    void updatePos();

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    //void wheelEvent(QGraphicsSceneWheelEvent* event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );

signals:
    void moved();
    void yourP1changed( QPoint );
    void yourP2changed( QPoint );

public slots:
    void sSetP1( QPoint );
    void sSetP2( QPoint );
    void remove();

private:
    int myIndex();
    int m_p1X;
    int m_p1Y;
    int m_p2X;
    int m_p2Y;

    Connector* m_pConnector;
};

#endif
