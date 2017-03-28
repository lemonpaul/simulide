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

#include <QtGui>
#include <QThread>

#include "utils.h"
#include "codeeditor.h"
#include "gcbdebugger.h"
#include "inodebugger.h"
#include "mcucomponent.h"
#include "simulator.h"

CodeEditor::CodeEditor( QWidget* parent, OutPanelText *outPane, RamTable *ramTable ) 
        : QPlainTextEdit( parent )
{
    m_outPane   = outPane;
    m_ramTable  = ramTable;
    m_lNumArea  = new LineNumberArea( this );
    m_hlighter  = new Highlighter( document() );
    m_appPath   = QCoreApplication::applicationDirPath();
    
    m_debugger = 0l;
    m_debugLine = 0;
    m_brkAction = 0;
    m_running   = false;
    m_isCompiled= false;
    m_debugging = false;
    
    QStringList lines = fileToStringList( m_appPath+"/data/codeeditor/config", "CodeEditor::CodeEditor" );

    int fontSize = 9;
    foreach( QString line, lines )  // Get path to gcbasic executable
    {
        if( line.startsWith("#") ) continue;
        
        if( line.contains("font_Size:") )
        {
            fontSize = line.remove("font_Size:").remove(" ").toInt();
        }
    }
    QFont font;
    font.setFamily("Monospace");
    font.setFixedPitch(true);
    font.setPointSize( fontSize );
    setFont(font);
    
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor( 255, 255, 245) );
    p.setColor(QPalette::Text, QColor( 0, 0, 0));
    setPalette(p);

    setupDebugTimer();

    connect( this, SIGNAL( blockCountChanged( int )),   this, SLOT( updateLineNumberAreaWidth( int )));
    connect( this, SIGNAL( updateRequest( QRect,int )), this, SLOT( updateLineNumberArea( QRect,int )));
    connect( this, SIGNAL( cursorPositionChanged()),    this, SLOT( highlightCurrentLine()));

    setLineWrapMode(QPlainTextEdit::NoWrap);
    updateLineNumberAreaWidth( 0 );
    highlightCurrentLine();
}
CodeEditor::~CodeEditor()
{}

void CodeEditor::setFile( const QString &filePath )
{
    m_outPane->appendText( "-------------------------------------------------------\n" );
    m_outPane->appendText( " File: " );
    m_outPane->appendText( filePath );
    m_outPane->writeText( "\n\n" );

    m_file  = filePath;
    
    QDir::setCurrent(m_file);
   
    m_isCompiled= false;
    /*m_isAsm = false;
    if( m_fileExt.endsWith(".asm") )
    {
        m_isAsm = true;
        getProcType();
        if(  m_processorType == 1 )
            m_hlighter->readSintaxFile( m_appPath+"/data/codeeditor/pic14asm.sintax");
        else if(  m_processorType ==2 )
            m_hlighter->readSintaxFile( m_appPath+"/data/codeeditor/avrasm.sintax");
    }
    else */
    if( m_file.endsWith(".gcb") )
    {
        m_hlighter->readSintaxFile( m_appPath+"/data/codeeditor/gcbasic.sintax");
        m_debugger = new GcbDebugger( this, m_outPane, filePath );
    }
    else if( m_file.endsWith(".cpp")|| m_file.endsWith(".ino") )
    {
        m_hlighter->readSintaxFile( m_appPath+"/data/codeeditor/cpp.sintax");
        m_debugger = new InoDebugger( this, m_outPane, filePath );
    }
}

QString CodeEditor::getFilePath()
{ 
    return ( m_file ); 
}

void CodeEditor::compile()
{
    int error=-2;
    m_isCompiled = false;
    
    m_outPane->appendText( "-------------------------------------------------------\n" );
    m_outPane->appendText( "Exec: ");
    
    error = m_debugger->compile();
    
    if( error == 0)
    {
        m_outPane->writeText( tr("     SUCCESS!!! Compilation Ok\n") );
        if( m_debugging )
        {
            m_debugLine  = 1;
        }
        m_isCompiled = true;
    }
    else 
    {
        m_outPane->writeText( tr("     ERROR!!! Compilation Faliled\n") );
        if( error > 0 ) // goto error line number
        {
            setTextCursor(QTextCursor(document()->findBlockByLineNumber(error-1)));
        }
    }
    updateScreen();
}

void CodeEditor::upload()
{
    if( document()->isModified() )
    {
         QMessageBox::warning( 0, tr("Document has Changed!!!"),
         tr("Document has Changed but not Saved.\n You should save it before uploading") );
    }
    if( !m_isCompiled ) compile();
    m_debugger->upload();
}

void CodeEditor::addBreakPoint( int line )
{
    if( !m_debugging ) return;

    int validLine = m_debugger->getValidLine( line );

    if( !m_brkPoints.contains( line ) ) m_brkPoints.append( validLine );
}

void CodeEditor::remBreakPoint( int line ) { m_brkPoints.removeOne( line ); }

void CodeEditor::run()
{
    if( m_running )     return;
    if( !m_debugging ) initDebbuger();

    if( m_debugging )
    {
        if( !m_brkPoints.isEmpty() )
        {
            m_running = true;
            m_timer.start( m_timerTick );
        }
    }
}

void CodeEditor::step()
{
    if( m_running )     return;
    if( !m_debugging ) initDebbuger();

    if( m_debugging )
    {
        runClockTick();
        updateScreen();
    }
}

void CodeEditor::stepOver()
{
    if( m_running )     return;
    if( !m_debugging ) initDebbuger();

    if( m_debugging )
    {
        runClockTick( true );
        updateScreen();
    }
}

void CodeEditor::pause()
{
    m_timer.stop();
    m_running = false;
    updateScreen();
}

void CodeEditor::reset()
{
    if( m_debugging )
    {
        McuComponent::self()->reset();
        m_debugLine = 1;
        pause();
    }
}

void CodeEditor::runClockTick( bool over )
{
    if( document()->isModified() )
    {
         QMessageBox::warning( 0, tr("Document has Changed!!!"),
         tr("Document has Changed\n you should save it and Reset") );
         pause();
         return;
    }
    m_prevDebugLine = m_debugLine;

    while( m_debugLine == m_prevDebugLine ) // Run to next src line
    {
        if( over ) m_debugLine = m_debugger->stepOver();
        else       m_debugLine = m_debugger->step();
        //qDebug() <<"m_prevDebugLine "<<m_prevDebugLine<< "  m_debugLine "<<m_debugLine;

        if( m_debugLine <= 0 ) m_debugLine = m_prevDebugLine; // Baksels returns -1
    }
}

void CodeEditor::timerTick()
{
    for( int step=0; step<m_stepsPT; step++ )
    {
        runClockTick();
        if( m_brkPoints.contains(m_debugLine) ) { pause();  break; }
    }
}

void CodeEditor::updateScreen()
{
    setTextCursor(QTextCursor(document()->findBlockByLineNumber(m_debugLine-1)));
    ensureCursorVisible();
    update();
}

void CodeEditor::initDebbuger()
{
    if( !McuComponent::self() )    // Must be an Mcu in Circuit
    {
         QMessageBox::warning( 0, tr("No Mcu in Circuit!!!"),
         tr("To start Debugging add a proper Mcu to the Circuit") );
         return;
    }
    if( !m_debugger ) return;
    if( m_file == "" ) return;
    
    if( !m_isCompiled ) compile();
    if( !m_isCompiled ) { m_debugging = false; return; } // Error compiling
    
    if( m_debugger->loadFirmware() )
    {
        //m_hlighter->addRegisters( m_debugger->getDefsList() );
        
        reset();
        m_debugging = true;
        
        Simulator::self()->setMcuClock( 0 );
        
        m_outPane->writeText( "-------------------------------------------------------\n" );
        m_outPane->writeText( "Debbuger Started \n" );
    }
    else stopDebbuger();
}

void CodeEditor::stopDebbuger()
{
    if( m_debugging )
    {
        m_debugger->stop();
        m_brkPoints.clear();
        m_debugLine = m_prevDebugLine = 0;
        pause();
        m_debugging = false;
        Simulator::self()->setMcuClock( 16 );
        m_outPane->writeText( "Debbuger Stopped \n" );
    }
}

void CodeEditor::setupDebugTimer()
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    int simuclock  = 4*1e6/4;        // steps/Sec
    int timerClock = simuclock/1000; // to reach 1000 steps per timer tick
    m_timerTick  = 10;

    m_stepsPT = m_timerTick*timerClock;

    qDebug() << "\nm_timerTick: " << m_timerTick << "mS \nSteps/tick: " << m_stepsPT;
    //m_outPane->writeText(tr("\nm_timerTick: %1 mS \nSteps/tick: %2" ).arg(m_timerTick).arg(m_stepsPT) );
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax( 1, blockCount() );
    while( max >= 10 ) { max /= 10; ++digits; }
    return  fontMetrics().height() + fontMetrics().width( QLatin1Char( '9' ) ) * digits;
}

void CodeEditor::updateLineNumberAreaWidth( int /* newBlockCount */ )
{
    setViewportMargins( lineNumberAreaWidth(), 0, 0, 0 );
}

void CodeEditor::updateLineNumberArea( const QRect &rect, int dy )
{
    if( dy ) m_lNumArea->scroll( 0, dy );
    else     m_lNumArea->update( 0, rect.y(), m_lNumArea->width(), rect.height() );
    if( rect.contains( viewport()->rect() ) ) updateLineNumberAreaWidth( 0 );
}

void CodeEditor::resizeEvent( QResizeEvent *e )
{
    QPlainTextEdit::resizeEvent( e );
    QRect cr = contentsRect();
    m_lNumArea->setGeometry( QRect( cr.left(), cr.top(), lineNumberAreaWidth(), cr.height() ) );
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if( !isReadOnly() )
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor( 250, 240, 220 );

        selection.format.setBackground( lineColor );
        selection.format.setProperty( QTextFormat::FullWidthSelection, true );
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append( selection );
    }
    setExtraSelections( extraSelections );
}

void CodeEditor::lineNumberAreaPaintEvent( QPaintEvent *event )
{
    QPainter painter( m_lNumArea );
    painter.fillRect( event->rect(), Qt::lightGray );

    QTextBlock block = firstVisibleBlock();

    int blockNumber = block.blockNumber();
    int top       = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int fontSize  = fontMetrics().height();

    while( block.isValid() && top <= event->rect().bottom() )
    {
        int blockSize = (int)blockBoundingRect( block ).height();
        int bottom = top + blockSize;

        if( block.isVisible() && bottom >= event->rect().top() )
        {
            int lineNumber = blockNumber + 1;
            // Check if there is a new breakpoint request from context menu
            int pos = m_lNumArea->lastPos;
            if( pos > top && pos < bottom)
            {
                if      ( m_brkAction == 1 ) addBreakPoint( lineNumber );
                else if ( m_brkAction == 2 ) remBreakPoint( lineNumber );
                m_brkAction = 0;
                m_lNumArea->lastPos = 0;
            }
            // Draw breakPoint icon
            if( m_brkPoints.contains(lineNumber) )
            {
                painter.setBrush( QColor(Qt::yellow) );
                painter.setPen( Qt::NoPen );
                painter.drawRect( 0, top, fontSize, fontSize );
            }
            // Draw debug line icon
            if( lineNumber == m_debugLine )
                painter.drawImage( QRectF(0, top, fontSize, fontSize), QImage(":/finish.png") );
            // Draw line number
            QString number = QString::number( lineNumber );
            painter.setPen( Qt::black );
            painter.drawText( 0, top, m_lNumArea->width(), fontSize, Qt::AlignRight, number );
        }
        block = block.next();
        top = bottom;
        ++blockNumber;
    }
}


// CLASS LineNumberArea ******************************************************
LineNumberArea::LineNumberArea( CodeEditor *editor ) : QWidget(editor)
{
    m_codeEditor = editor;
}
LineNumberArea::~LineNumberArea(){}

void LineNumberArea::contextMenuEvent( QContextMenuEvent *event)
{
    event->accept();
    QMenu menu;

    QAction *addm_brkAction = menu.addAction( QIcon(":/remove.png"),"Add BreakPoint" );
    connect( addm_brkAction, SIGNAL( triggered()), m_codeEditor, SLOT(slotAddBreak()) );

    QAction *remm_brkAction = menu.addAction( QIcon(":/remove.png"),"Remove BreakPoint" );
    connect( remm_brkAction, SIGNAL( triggered()), m_codeEditor, SLOT(slotRemBreak()) );

    if( menu.exec(event->globalPos()) != 0 ) lastPos = event->pos().y();
}

#include "moc_codeeditor.cpp"
