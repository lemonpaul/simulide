/***************************************************************************
 *   Copyright (C) 2017 by santiago González                               *
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

#include "textcomponent.h"

Component* TextComponent::construct( QObject* parent, QString type, QString id )
{
    return new TextComponent( parent, type, id );
}

LibraryItem* TextComponent::libraryItem()
{
    return new LibraryItem(
        tr( "Text" ),
        tr( "Other" ),
        "text.png",
        "TextComponent",
    TextComponent::construct );
}

TextComponent::TextComponent( QObject* parent, QString type, QString id )
             : Component( parent, type, id )
{
    m_color = QColor( 255, 255, 220 );

    QFont sansFont( "Helvetica [Cronyx]", 10 );
    sansFont.setWeight( QFont::Medium );

    m_text = new QGraphicsTextItem( this );
    m_text->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_text->setTextWidth( 90 );
    m_text->setFont( sansFont );
    m_text->setPlainText("... TEXT ...");
    m_text->setPos( 0, 0 );
    m_text->setCursor( Qt::IBeamCursor );
    m_text->document()->setTextWidth(-1);
    m_text->setDefaultTextColor( Qt::darkBlue );
    m_area = QRect( -10, -10, m_text->boundingRect().width()+20, m_text->boundingRect().height()+20 );

    connect(m_text->document(), SIGNAL( contentsChange(int, int, int )),
                          this, SLOT( updateGeometry(int, int, int )));
}
TextComponent::~TextComponent(){}

void TextComponent::updateGeometry(int, int, int)
{
    m_text->document()->setTextWidth(-1);
    m_area = QRect( -10, -10, m_text->boundingRect().width()+20, m_text->boundingRect().height()+20 );
}

void TextComponent::paint( QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    Component::paint( p, option, widget );

    QPen pen( QColor( 0, 0, 0 ), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setPen( pen );

    p->drawRect( m_area );
}

#include "moc_textcomponent.cpp"
