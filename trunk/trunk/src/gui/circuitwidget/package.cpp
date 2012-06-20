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

//#include <qstringlist.h>

#include "package.h"
#include "pin.h"
#include "connector.h"
#include "mainwindow.h"

#include "e-resistor.h"

Package::Package( QObject* parent, QString type, QString id )
    : Component( parent, type, id ), eElement( id.toStdString() )
{
    //m_labelx = -38;
    //m_labely = -137;

    m_color = QColor( 50, 50, 70 );
}
Package::~Package() {}

void Package::initPackage()
{
    //qDebug() << "Package::initPackage datafile: " << m_dataFile;

    QFile file( QCoreApplication::applicationDirPath()+"/"+m_dataFile );
    if( !file.open(QFile::ReadOnly | QFile::Text) )
    {
          QMessageBox::warning(0, "Package::initPackage",
          tr("Cannot read file %1:\n%2.").arg(m_dataFile).arg(file.errorString()));
          return;
    }

    QDomDocument domDoc;
    if( !domDoc.setContent(&file) )
    {
         QMessageBox::warning(0, "Package::initPackage",
         tr("Cannot set file %1\nto DomDocument") .arg(m_dataFile));
         file.close();
         return;
    }
    file.close();

    QDomElement root  = domDoc.documentElement();

    if( root.tagName()!="package" )
    {
        qDebug() << " Package::initPackage Error reading Package file: " << m_dataFile;
        return;
    }

    int width  = root.attribute( "width" ).toInt();
    int height = root.attribute( "height" ).toInt();
    m_numpins  = root.attribute( "pins" ).toInt();
    m_ePin.resize( m_numpins );

    m_area = QRect( -(width/2)*8, -(height/2)*8, 8*width, 8*height );
    label->setPos( m_area.x(), m_area.y()-20);

    QDomNode node = root.firstChild();

    int chipPos = 0;

    while( !node.isNull() )
    {
        QDomElement element = node.toElement();
        if( element.tagName() == "pin" )
        {
            QString label = element.attribute( "label" );
            QString type  = element.attribute( "type" );
            QString id    = element.attribute( "id" );
            QString side  = element.attribute( "side" );
            int     pos   = element.attribute( "pos" ).toInt();

            int xpos = 0;
            int ypos = 0;
            int angle = 0;

            if( side=="left" )
            {
                xpos = -(width/2)*8-8;
                ypos = -(height/2)*8+8*pos;
                angle = 180;
            }
            else if( side=="top")
            {
                xpos = -(width/2)*8+8*pos;
                ypos = -(height/2)*8-8;
                angle = 270;
            }
            else if( side=="right" )
            {
                xpos =  (width/2)*8+8;
                ypos = -(height/2)*8+8*pos;
                angle = 0;
            }
            else if( side=="bottom" )
            {
                xpos = -(width/2)*8+8*pos;
                ypos =  (height/2)*8+8;
                angle = 90;
            }

            chipPos++;
            addPin( id, type, label, chipPos, xpos, ypos, angle );
        }
        node = node.nextSibling();
    }
}

void Package::addPin( QString id, QString /*type*/, QString label, int pos, int xpos, int ypos, int angle )
{
    Pin* pin = new Pin( angle, QPoint(xpos, ypos), id, pos-1, this ); // pos in package starts at 1
    pin->setLabelText( label );
    //pin->setFlag( QGraphicsItem::ItemStacksBehindParent, false );
    //m_pinList.append( new eSource( id.append("-eSource").toStdString(), pin) );
    m_ePin[pos-1] = pin;
}

void Package::remove()
{
    /*foreach( PackagePin *mcupin, m_pinList )
    {
        Pin* pin = mcupin->pin(); //static_cast<Pin*>(mcupin->pin());
        if( pin->isConnected() ) pin->connector()->remove();
    }
    m_pinList.clear();*/

    Component::remove();
}

void Package::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    event->accept();
    QMenu *menu = new QMenu();
    /*QAction *loadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Load firmware") );
    connect( loadAction, SIGNAL(triggered()), this, SLOT(slotLoad()) );

    QAction *reloadAction = menu->addAction( QIcon(":/fileopen.png"),tr("Reload firmware") );
    connect( reloadAction, SIGNAL(triggered()), this, SLOT(slotReload()) );*/
    
    /*QAction *ramAction = menu->addAction( QIcon(":/fileopen.png"),"View Ram" );
    connect( ramAction, SIGNAL(triggered()), this, SLOT(viewRam()) );*/

    menu->addSeparator();

    Component::contextMenu( event, menu );
    menu->deleteLater();
}

void Package::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    p->drawRoundedRect( boundingRect(), 1, 1);

    p->setPen( QColor( 170, 170, 150 ) );
    p->drawArc( -4, boundingRect().y()-4, 8, 8, 0, -2880 /* -16*180 */ );
}

#include "moc_package.cpp"
