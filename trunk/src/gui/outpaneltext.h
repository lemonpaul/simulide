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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 ***************************************************************************/

#ifndef OUTPANELTEXT_H
#define OUTPANELTEXT_H

#include <QPlainTextEdit>
#include <QObject>

class OutHighlighter;

class OutPanelText : public QPlainTextEdit
{
    Q_OBJECT
    public:
        OutPanelText(QWidget *parent = 0);
        ~OutPanelText();
        
 static OutPanelText* self() { return m_pSelf; }

        void appendText(  const QString &text );
        
        void step();

    private:
 static OutPanelText* m_pSelf;
 
    QString m_text;
 
        OutHighlighter *m_highlighter;

};

// CLASS OutHighlighter ****************************************

class QSyntaxHighlighter;

class OutHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        OutHighlighter( QTextDocument *parent = 0 );
        ~OutHighlighter();

    protected:
        void highlightBlock( const QString &text );

    private:
        struct HighlightingRule
        {
            QRegExp pattern;
            QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        QTextCharFormat errorFormat;
        QTextCharFormat fileFormat;
        QTextCharFormat successFormat;
        QTextCharFormat warningFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat quotationFormat;
};

#endif
