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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

#include "highlighter.h"
#include "outpaneltext.h"
#include "ramtable.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class BaseDebugger;
class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

    public:
        CodeEditor( QWidget *parent, OutPanelText *outPane, RamTable *ramTable );
        ~CodeEditor();

        void setFile(const QString &filePath);
        QString getFilePath();

        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int  lineNumberAreaWidth();
        
        void setCompiled( bool compiled ) { m_isCompiled = compiled; }

    signals:
        void msg( QString text );

    public slots:
        void stopDebbuger();
        void slotAddBreak() { m_brkAction = 1; }
        void slotRemBreak() { m_brkAction = 2; }
        void timerTick();
        void compile();
        void upload();
        void step();
        void stepOver();
        void pause();
        void reset();
        void run();

    protected:
        void resizeEvent(QResizeEvent *event);

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void updateLineNumberArea( const QRect &, int );
        void highlightCurrentLine();
        void initDebbuger();

    private:
        void addBreakPoint( int line );
        void remBreakPoint( int line );
        void updateScreen();

        void runClockTick( bool over=false );
        void setupDebugTimer();
        
        BaseDebugger*  m_debugger;
        RamTable*      m_ramTable;
        OutPanelText*  m_outPane;

        LineNumberArea *m_lNumArea;
        Highlighter    *m_hlighter;

        QString m_appPath;
        QString m_file;
        QList<int>      m_brkPoints;

        int m_brkAction;    // 0 = no action, 1 = add brkpoint, 2 = rem brkpoint
        int m_debugLine;
        int m_prevDebugLine;

        bool m_isCompiled;
        bool m_debugging;
        bool m_running;
        
        QTimer m_timer;
        int m_timerTick;
        int m_stepsPT;
};


// ********************* CLASS LineNumberArea **********************************

class LineNumberArea : public QWidget
{
    public:
        LineNumberArea(CodeEditor *editor);
        ~LineNumberArea();

        QSize sizeHint() const { return QSize(m_codeEditor->lineNumberAreaWidth(), 0); }

        int lastPos;

    protected:
        void contextMenuEvent( QContextMenuEvent *event);
        void paintEvent(QPaintEvent *event) { m_codeEditor->lineNumberAreaPaintEvent(event); }

    private:
        CodeEditor *m_codeEditor;
};

#endif
