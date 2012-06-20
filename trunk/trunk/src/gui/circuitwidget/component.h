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


#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QPointer>

#include "simulator.h"
#include "circuit.h"
#include "QPropertyEditorWidget.h"

class Pin;
class Label;

class Component : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    Q_PROPERTY( QString  itemtype READ itemtype )
    Q_PROPERTY( QString  id       READ itemID   WRITE setId      DESIGNABLE true USER true )
    Q_PROPERTY( bool     Show_id  READ Show_id  WRITE setShow_id DESIGNABLE true USER true )
    Q_PROPERTY( qreal    rotation READ rotation WRITE setRotation )
    Q_PROPERTY( int      x        READ x        WRITE setX )
    Q_PROPERTY( int      y        READ y        WRITE setY )
    Q_PROPERTY( int      labelx   READ labelx   WRITE setLabelX )
    Q_PROPERTY( int      labely   READ labely   WRITE setLabelY )
    Q_PROPERTY( int      labelrot READ labelrot WRITE setLabelRot )

    public:
        QRectF boundingRect() const { return m_area; }

        Component( QObject* parent, QString type, QString id );
        ~Component();

        enum { Type = UserType + 1 };
        int type() const { return Type; }

        bool Show_id();
        void setShow_id( bool show );

        QString itemID();
        void setId( QString id );

        int labelx();
        void setLabelX( int x );

        int labely();
        void setLabelY( int y );

        int labelrot();
        void setLabelRot( int rot );

        void setLabelPos();

        QString itemtype();
        QString category();
        QIcon   icon();

        //bool isChanged();

        virtual void inStateChanged( int ){}

        //virtual void setChanged( bool changed ){ Q_UNUSED(changed); }

        virtual void move( QPointF delta );

        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    signals:
        void moved();

    public slots:
        void rotateCW();
        void rotateCCW();
        void rotateHalf();
        void slotremove();

        virtual void remove();
        //virtual void runStep() { /*setChanged( false);*/ }
        //virtual void saveState() { ; }
        //virtual void runTimStep() = 0;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
        void contextMenu( QGraphicsSceneContextMenuEvent* event, QMenu* menu );

        void setLabel();

        Label* label;

        QString m_id;
        QString m_type;
        QString m_category;
        QIcon   m_icon;
        QColor  m_color;
        QRectF  m_area;         // bounding rect

        int m_labelx;
        int m_labely;
        int m_labelrot;

        bool m_showId;
        //bool m_changed;
        bool m_changedPrev;
};

typedef Component* (*createItemPtr)( QObject* parent, QString type, QString id );


class Label : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
    public:
        Label( Component* parent );
        ~Label();

    public slots:
        void rotateCW();
        void rotateCCW();
        void rotate180();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    private:
        Component*  m_parentComp;
};
#endif

