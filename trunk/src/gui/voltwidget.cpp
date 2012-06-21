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

#include <QtCore/QVariant>
#include <QtGui>

#include "voltwidget.h"

VoltWidget::VoltWidget()
{
    setupWidget();
    setFixedSize( 60, 90 );
}
VoltWidget::~VoltWidget() {}

void VoltWidget::setupWidget()
{
    dial = new QDial(this);
    dial->setObjectName(QString::fromUtf8("dial"));
    dial->setNotchesVisible(true);
    dial->setMinimum(0);
    dial->setMaximum(500);
    dial->setValue(000);
    dial->setSingleStep(1);
    
    pushButton = new QPushButton(this);
    pushButton->setObjectName( QString::fromUtf8("pushButton") );
    pushButton->setCheckable(true);
    pushButton->setForegroundRole( QPalette::BrightText );
    pushButton->setFixedSize( 40, 17 );

    QFont sansFont("Helvetica [Cronyx]", 8 );
    pushButton->setFont( sansFont );

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 2, 0, 4);
    verticalLayout->addWidget(dial);
    verticalLayout->addWidget(pushButton);
    verticalLayout->setAlignment( pushButton, Qt::AlignHCenter );
}

