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

#include "circuitview.h"
#include "circuit.h"
#include "component.h"
#include "utils.h"

CircuitView::CircuitView( QWidget *parent )
    : QGraphicsView( parent )
{
    m_circuit     = 0l;
    m_enterItem   = 0l;
    m_scalefactor = 1;

    clear();

    viewport()->setFixedSize(1600,1200);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setViewportUpdateMode( QGraphicsView::MinimalViewportUpdate );
    setCacheMode( CacheBackground );
    setRenderHint( QPainter::Antialiasing );
    setTransformationAnchor( AnchorUnderMouse );
    setResizeAnchor( AnchorUnderMouse );
    setDragMode( QGraphicsView::RubberBandDrag );

    m_file = tr("document1.circuit");
    setWindowTitle(m_file + "[*]");

    setAcceptDrops(true);
}
CircuitView::~CircuitView() { }

void CircuitView::clear()
{
    if( m_circuit ) delete m_circuit;
    m_circuit = new Circuit( -800, -600, 1600, 1200, this );
    setScene( m_circuit );
    centerOn( 300, 200 );
}

void CircuitView::wheelEvent(QWheelEvent *event) { scaleView(pow( 2.0, event->delta() / 700.0)); }

void CircuitView::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 8, 8)).width();

    if( factor < 2 || factor > 100 ) return;

    m_scalefactor *=  factor;
    scale(scaleFactor, scaleFactor);
}

void CircuitView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    QString type = event->mimeData()->html();
    QString id = event->mimeData()->text();
    id.append( "-" );
    id.append( m_circuit->newSceneId() );
    m_enterItem = m_circuit->createItem( type, id );
    m_enterItem->setPos( mapToScene( event->pos() ) );
    m_circuit->addItem( m_enterItem );
}

void CircuitView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    if (m_enterItem ) m_enterItem->setPos( togrid( mapToScene( event->pos() ) ) );
}

void CircuitView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
    if ( m_enterItem )
    {
        m_enterItem->remove();
        m_enterItem = 0l;
    }
}

void CircuitView::resizeEvent( QResizeEvent *event )
{
    int width = event->size().width();
    int height = event->size().height();

    m_circuit->setSceneRect(-width/2+2, -height/2+2, width-4, height-4);

    QGraphicsView::resizeEvent(event);
}

void CircuitView::keyPressEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Shift )
        setDragMode( QGraphicsView::ScrollHandDrag );
}

void CircuitView::keyReleaseEvent( QKeyEvent *event )
{
    if( event->key() == Qt::Key_Shift )
        setDragMode( QGraphicsView::RubberBandDrag );
}

#include "moc_circuitview.cpp"
