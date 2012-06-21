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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QtGui>
#include <QDomDocument>

#include "simulator.h"
#include "e-node.h"

//class Circuit;
class Component;
class Connector;
//class ConnectorLine;
class Pin;


class Circuit : public QGraphicsScene
{
    Q_OBJECT

    public:
        Circuit(qreal x, qreal y, qreal width, qreal height, QGraphicsView*  parent);
        ~Circuit();

        static Circuit*  self() { return m_pSelf; }

        void drawBackground(QPainter* painter, const QRectF &rect);

        void loadCircuit( QString &fileName );
        bool saveCircuit( QString &fileName );

        Component* createItem( QString name, QString id );

        QString newSceneId();

        void newconnector( Pin*  startpin );
        void closeconnector( Pin* endpin );
        Connector* getNewConnector() { return new_connector; }

        QList<Component*>* compList() { return &m_compList; }
        QList<Component*>* conList()  { return &m_conList; }
        //QList<eNode*>*   nodeList() { return &m_eNodeList;}


        /** a conector is been created*/
        void constarted( bool started) { m_con_started = started; }
        bool is_constarted() { return m_con_started ; }

        void removeItems();

        QGraphicsView* widget(){ return m_widget; }

    //public slots:
    //    void setChanged();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    private:
        void LoadProperties( QDomElement element, Component* Item );
        void listToDom( QList<Component*>* complist );

        static Circuit*  m_pSelf;

        QDomDocument m_domDoc;

        QRect          m_scenerect;
        QGraphicsView* m_widget;
        Connector*     new_connector;

        int  m_seqNumber;
        bool m_con_started;
        bool m_changed;

        QList<Component*> m_compList;   // Component list
        QList<Component*> m_conList;    // Connector list
        //QList<Component*> m_updateList;
        //QList<eNode*>     m_eNodeList;

        //QBasicTimer m_timer;

        Simulator simulator;
        //QThread simulatorThread;
};

#endif

