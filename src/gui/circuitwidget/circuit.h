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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QDomDocument>

#include "simulator.h"
#include "component.h"
#include "connector.h"
#include "pin.h"


class MAINMODULE_EXPORT Circuit : public QGraphicsScene
{
    Q_OBJECT
    
    Q_PROPERTY( int Speed     READ circSpeed WRITE setCircSpeed DESIGNABLE true USER true )
    Q_PROPERTY( int ReactStep READ reactStep WRITE setReactStep DESIGNABLE true USER true )
    Q_PROPERTY( int NoLinStep READ noLinStep WRITE setNoLinStep DESIGNABLE true USER true )
    Q_PROPERTY( int NoLinAcc  READ nlAcc     WRITE setNlAcc     DESIGNABLE true USER true )
    Q_PROPERTY( bool Draw_Grid  READ drawGrid  WRITE setDrawGrid     DESIGNABLE true USER true )

    public:
        Circuit(qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent);
        ~Circuit();

 static Circuit*  self() { return m_pSelf; }
        
        int   reactStep();
        void  setReactStep( int steps );

        int   noLinStep();
        void  setNoLinStep( int steps );
        
        int  circSpeed();
        void setCircSpeed( int rate );
        
        int  nlAcc();
        void setNlAcc( int ac );
        
        bool drawGrid();
        void setDrawGrid( bool draw );
        
        void removeItems();
        void removeComp( Component* comp );
        void remove();
        void saveState();

        void drawBackground(QPainter* painter, const QRectF &rect);

        Pin* findPin( int x, int y, QString id );

        void loadCircuit( QString &fileName );
        bool saveCircuit( QString &fileName );

        Component* createItem( QString name, QString id );

        QString newSceneId();

        void newconnector( Pin*  startpin );
        void closeconnector( Pin* endpin );
        Connector* getNewConnector() { return new_connector; }

        QList<Component*>* compList();
        QList<Component*>* conList();

        void constarted( bool started);
        bool is_constarted();

        bool  pasting();
        QPointF deltaMove();

        const QString getFileName() const { return m_fileName; }

    public slots:
        void createSubcircuit();
        void copy( QPointF eventpoint );
        void paste( QPointF eventpoint );
        void undo();
        void redo();
        void importCirc(  QPointF eventpoint  );

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
        void keyPressEvent ( QKeyEvent * event );

    private:
        void loadProperties( QDomElement element, Component* Item );
        void listToDom( QDomDocument* doc, QList<Component*>* complist );
        void loadDomDoc(QDomDocument* doc );
        void circuitToDom();

        QString getCompId( QString name );

 static Circuit*  m_pSelf;
 
        QDomDocument m_domDoc;
        QDomDocument m_copyDoc;
        QString      m_fileName;

        QRect          m_scenerect;
        QGraphicsView* m_graphicView;
        Connector*     new_connector;

        int  m_seqNumber;
        bool m_con_started;
        bool m_pasting;
        bool m_drawGrid;

        QPointF m_eventpoint;
        QPointF m_deltaMove;

        QList<Component*> m_compList;   // Component list
        QList<Component*> m_conList;    // Connector list

        QList<QDomDocument*> m_undoStack;
        QList<QDomDocument*> m_redoStack;

        Simulator simulator;
};

#endif
