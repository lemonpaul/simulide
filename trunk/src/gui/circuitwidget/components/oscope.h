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

#ifndef OSCOPE_H
#define OSCOPE_H

#include "component.h"
#include "e-element.h"
#include "topwidget.h"
#include "pin.h"

class LibraryItem;
class OscopeWidget;

class MAINMODULE_EXPORT Oscope : public Component, public eElement
{
    Q_OBJECT

    public:

        Oscope( QObject* parent, QString type, QString id );
        ~Oscope();

        static Component* construct( QObject* parent, QString type, QString id );
        static LibraryItem* libraryItem();

        void initialize();
        
        double getVolt();

        virtual void paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget );

    public slots:
        void remove();

    private:
        //bool m_changed;
        
        Pin m_pinP;
        Pin m_pinN;
        
        OscopeWidget* m_oscopeW;
        TopWidget    m_topW;
        QGraphicsProxyWidget* m_proxy;
};

#endif

