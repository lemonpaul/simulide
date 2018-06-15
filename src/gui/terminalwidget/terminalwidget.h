/***************************************************************************
 *   Copyright (C) 2016 by santiago González                               *
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

#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QtWidgets>

#include "outpaneltext.h"

class MAINMODULE_EXPORT TerminalWidget : public QWidget
{
    Q_OBJECT

    public:
        TerminalWidget( QWidget *parent = 0);
        ~TerminalWidget();
        
 static TerminalWidget* self() { return m_pSelf; }

        void uartIn( uint32_t value );
        void uartOut( uint32_t value );

        void step();
 
    private slots:
        void onTextChanged();
        void onValueChanged();
        void valueButtonClicked();
        void ascciButtonClicked();

    private:
 static TerminalWidget* m_pSelf;
 
        QVBoxLayout   m_verticalLayout;
        QHBoxLayout   m_sendLayout;
        QHBoxLayout   m_textLayout;
        QLineEdit     m_sendText;
        QLineEdit     m_sendValue;
        OutPanelText  m_uartInPanel;
        OutPanelText  m_uartOutPanel;
        QPushButton   m_ascciButton;
        QPushButton   m_valueButton;

        bool m_printASCII;
};

#endif
