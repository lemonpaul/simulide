/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
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

#include <QtGui>
#include <QThread>
#include <QtAlgorithms>
#include <QRegExp>

#include "utils.h"
#include "codeeditor.h"
#include "gcbdebugger.h"
#include "inodebugger.h"
#include "avrasmdebugger.h"
#include "picasmdebugger.h"
#include "mcucomponent.h"
#include "mainwindow.h"
#include "simuapi_apppath.h"

bool  CodeEditor::m_showSpaces = false;
bool  CodeEditor::m_spaceTabs  = false;
int   CodeEditor::m_fontSize = 9;
int   CodeEditor::m_tabSize = 4;
QFont CodeEditor::m_font;

CodeEditor::CodeEditor( QWidget* parent, OutPanelText *outPane, RamTable *ramTable ) 
          : QPlainTextEdit( parent )
{
    setObjectName( "Editor" );
    
    m_outPane   = outPane;
    m_ramTable  = ramTable;
    m_lNumArea  = new LineNumberArea( this );
    m_hlighter  = new Highlighter( document() );
    //m_appPath   = QCoreApplication::applicationDirPath();
    
    m_debugger = 0l;
    m_debugLine = 0;
    m_brkAction = 0;
    m_running   = false;
    m_isCompiled= false;
    m_debugging = false;
    
    m_font.setFamily("Monospace");
    m_font.setFixedPitch( true );
    m_font.setPointSize( m_fontSize );
    setFont( m_font );
    
    QSettings* settings = MainWindow::self()->settings();
    
    if( settings->contains( "Editor_show_spaces" ) )
        setShowSpaces( settings->value( "Editor_show_spaces" ).toBool() );

    if( settings->contains( "Editor_spaces_tabs" ) )
        setSpaceTabs( settings->value( "Editor_spaces_tabs" ).toBool() );

    if( settings->contains( "Editor_tab_size" ) )
        setTabSize( settings->value( "Editor_tab_size" ).toInt() );

    if( settings->contains( "Editor_font_size" ) )
        setFontSize( settings->value( "Editor_font_size" ).toInt() );

    
    QPalette p = palette();
    p.setColor( QPalette::Base, QColor( 255, 255, 245) );
    p.setColor( QPalette::Text, QColor( 0, 0, 0) );
    setPalette( p );

    connect( this, SIGNAL( blockCountChanged( int )),   this, SLOT( updateLineNumberAreaWidth( int )));
    connect( this, SIGNAL( updateRequest( QRect,int )), this, SLOT( updateLineNumberArea( QRect,int )));
    connect( this, SIGNAL( cursorPositionChanged()),    this, SLOT( highlightCurrentLine()));

    setLineWrapMode(QPlainTextEdit::NoWrap);
    updateLineNumberAreaWidth( 0 );
    highlightCurrentLine();
}
CodeEditor::~CodeEditor()
{}

void CodeEditor::setFile( const QString& filePath )
{
    if( m_debugger ) return;
    
    m_outPane->appendText( "-------------------------------------------------------\n" );
    m_outPane->appendText( " File: " );
    m_outPane->appendText( filePath );
    m_outPane->writeText( "\n\n" );

    m_file  = filePath;
    m_fileDir  = filePath;
    m_fileName = filePath.split("/").last();
    m_fileDir.remove( m_fileDir.lastIndexOf( m_fileName ), m_fileName.size() );
    m_fileExt  = "."+m_fileName.split(".").last();
    m_fileName = m_fileName.remove( m_fileExt );
    
    QDir::setCurrent(m_file);
   
    m_isCompiled= false;

    QString sintaxPath = SIMUAPI_AppPath::self()->availableDataFilePath("codeeditor/sintax/");

    if( m_file.endsWith(".gcb") )
    {
        //m_appPath+"/data/codeeditor/gcbasic.sintax");
        QString path = sintaxPath + "gcbasic.sintax";
         m_hlighter->readSintaxFile( path );

        m_debugger = new GcbDebugger( this, m_outPane, filePath );
    }
    else if( m_file.endsWith(".cpp")|| m_file.endsWith(".ino") || m_file.endsWith(".h") )
    {
        //m_appPath+"/data/codeeditor/cpp.sintax"
        QString path = sintaxPath + "cpp.sintax";
        m_hlighter->readSintaxFile( path );

        if( m_file.endsWith(".ino") )
            m_debugger = new InoDebugger( this, m_outPane, filePath );
    }
    else if( m_file.endsWith(".asm") )
    {
        // We should identify if pic or avr asm
        QStringList picInstr = QString("addlw addwf andlw andwf bcf bov bsf btfsc btg btfss clrf clrw clrwdt comf decf decfsz goto incf incfsz iorlw iorwf movf movlw movwf reset retfie retlw return rlf rrfsublw subwf swapf xorlw xorwf")
                .split(" ");
        QStringList avrInstr = QString("add adc adiw sub subi sbc sbci sbiw andi ori eor com neg sbr cbr dec tst clr ser mul rjmp ijmp jmp rcall icall ret reti cpse cp cpc cpi sbrc sbrs sbic sbis brbs brbc breq brne brcs brcc brsh brlo brmi brpl brge brlt brhs brhc brts brtc brvs brvc brie brid mov movw ldi lds ld ldd sts st std lpm in out push pop lsl lsr rol ror asr swap bset bclr sbi cbi bst bld sec clc sen cln sez clz sei cli ses cls sev clv set clt seh clh wdr")
                .split(" ");

        int isPic = getSintaxCoincidences( m_file, picInstr );
        int isAvr = getSintaxCoincidences( m_file, avrInstr );

        m_outPane->writeText( tr("File recognized as: ") );

        if( isPic > isAvr )   // Is Pic
        {
            m_outPane->writeText( tr("Pic asm\n") );

            QString path = sintaxPath + "pic14asm.sintax";
            m_hlighter->readSintaxFile( path );

            m_debugger = new PicAsmDebugger( this, m_outPane, filePath );
        }
        else                 // Is Avr
        {
            m_outPane->writeText( tr("Avr asm\n") );

            QString path = sintaxPath + "avrasm.sintax";
            m_hlighter->readSintaxFile( path );

            m_debugger = new AvrAsmDebugger( this, m_outPane, filePath );
        }
    }
}

int CodeEditor::getSintaxCoincidences( QString& fileName, QStringList& instructions )
{
    QStringList lines = fileToStringList( fileName, "CodeEditor::getSintaxCoincidences" );

    int coincidences = 0;

    foreach( QString line, lines )
    {
        if( line.startsWith(";")) continue;
        if( line.startsWith("#")) continue;
        if( line.startsWith(".")) continue;
        foreach( QString instruction, instructions )
        {
            if( line.contains( QRegExp( "\\b"+instruction+"\\b" ) ))
            {
                coincidences++;
            }
        }
    }
    return coincidences;
}

QString CodeEditor::getFilePath()
{ 
    return ( m_file ); 
}

void CodeEditor::setCompilerPath()
{
    if( m_debugger ) m_debugger->getCompilerPath();
}

void CodeEditor::compile()
{
    m_debugLine  = -1;
    update();
    
    int error=-2;
    m_isCompiled = false;
    
    m_outPane->appendText( "-------------------------------------------------------\n" );
    m_outPane->appendText( "Exec: ");

    if( m_file.endsWith("Makefile") )                          // Is a Makefile, make it
    {
        m_outPane->writeText( "make "+m_file+"\n" );

        QProcess makeproc( 0l );
        makeproc.setWorkingDirectory( m_fileDir );
        makeproc.start( "make" );
        makeproc.waitForFinished(-1);

        QString p_stdout = makeproc.readAllStandardOutput();
        QString p_stderr = makeproc.readAllStandardError();
        m_outPane->appendText( p_stderr );
        m_outPane->appendText( "\n" );
        m_outPane->appendText( p_stdout );
        m_outPane->writeText( "\n\n" );

        if( p_stderr.toUpper().contains("ERROR") || p_stdout.toUpper().contains("ERROR") )
        {
            error = -1;
        }
        else error = 0;
    }
    else
    {
        if( !m_debugger )
        {
            m_outPane->writeText( "File type not supported\n" );
            return;
        }
        error = m_debugger->compile();
    }

    if( error == 0)
    {
        m_outPane->writeText( tr("     SUCCESS!!! Compilation Ok\n") );
        
        m_isCompiled = true;
    }
    else 
    {
        m_outPane->writeText( tr("     ERROR!!! Compilation Faliled\n") );
        
        if( error > 0 ) // goto error line number
        {
            m_debugLine = error; // Show arrow in error line
            updateScreen();
        }
    }
}

void CodeEditor::upload()
{
    if( m_file.endsWith(".hex") )     // is an .hex file, upload to proccessor
    {
        //m_outPane->writeText( "-------------------------------------------------------\n" );
        m_outPane->appendText( tr("\nUploading: \n") );
        m_outPane->appendText( m_file );
        m_outPane->writeText( "\n\n" );

        if( McuComponent::self() ) McuComponent::self()->load( m_file );
        return;
    }
    if( !m_isCompiled ) compile();
    if( !m_isCompiled ) return;
    if( m_debugger ) m_debugger->upload();
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
    if( m_running ) return;

    m_running = true;
    timerTick();
}

void CodeEditor::step()
{
    if( m_running ) return;
    
    runClockTick();
    updateScreen();

}

void CodeEditor::stepOver()
{
    if( m_running ) return;

    runClockTick( true );
    updateScreen();
}

void CodeEditor::pause()
{
    m_running = false;
    updateScreen();
}

void CodeEditor::reset()
{
    if( m_running ) pause();
    
    McuComponent::self()->reset();
    m_debugLine = m_debugger->getProgramStart();
    
    updateScreen();
}

void CodeEditor::runClockTick( bool over )
{
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
    if( !m_running ) return;
    
    runClockTick();
    if( m_brkPoints.contains(m_debugLine) ) pause();

    if( m_running ) QTimer::singleShot( 500, this, SLOT( timerTick()) );
    
    updateScreen();
}

void CodeEditor::updateScreen()
{
    setTextCursor( QTextCursor(document()->findBlockByLineNumber(m_debugLine-1)));
    ensureCursorVisible();
    update();
}

bool CodeEditor::initDebbuger()
{
    m_outPane->writeText( "-------------------------------------------------------\n" );
    m_outPane->writeText( "Starting Debbuger...\n" );

    bool error = false;
    
    if( !McuComponent::self() )             // Must be an Mcu in Circuit
    {
        m_outPane->writeText( "\n    Error: No Mcu in Simulator... \n" );
        error = true;
    }
    else if( !m_debugger )             // No debugger for this file type
    {
        m_outPane->writeText( "\n    Error: No Debugger Suited for this File... \n" );
        error = true;
    }
    else if( m_file == "" )                                   // No File
    {
        m_outPane->writeText( "\n    Error: No File... \n" );
        error = true;
    }
    else if( !m_isCompiled ) 
    {
        compile();
        if( !m_isCompiled )                           // Error compiling
        { 
            m_outPane->writeText( "\n    Error Compiling... \n" );
            error = true; 
        }
    }
    m_outPane->writeText( "\n" );
    if( error )
    {
        stopDebbuger();
    }
    else                                          // OK: Start Debugging
    {
        if( !m_debugger->loadFirmware() )      // Error Loading Firmware
        {
            m_outPane->writeText( "\n    Error Loading Firmware... \n" );
            stopDebbuger();
        }        
        else 
        {
            m_debugging = true;
            reset();
            BaseProcessor::self()->setSteps( 0 );
            m_outPane->writeText( "Debbuger Started \n" );
        }
    }
    return m_debugging;
}

void CodeEditor::stopDebbuger()
{
    if( m_debugging )
    {
        m_debugger->stop();
        m_brkPoints.clear();
        m_debugLine = m_prevDebugLine = 0;
        McuComponent::self()->setFreq( McuComponent::self()->freq() );
        pause();
        m_debugging = false;
    }
    m_outPane->writeText( "Debbuger Stopped \n" );
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

void CodeEditor::focusInEvent( QFocusEvent* event)
{
    QPropertyEditorWidget::self()->setObject( this );
    if( m_debugger ) QPropertyEditorWidget::self()->addObject( m_debugger );
    QPlainTextEdit::focusInEvent( event );
}

void CodeEditor::writeSettings()
{
    MainWindow::self()->settings()->setValue( "Editor_font_size", QString::number(m_fontSize) );
    MainWindow::self()->settings()->setValue( "Editor_tab_size", QString::number(m_tabSize) );

    if( m_showSpaces )
        MainWindow::self()->settings()->setValue( "Editor_show_spaces", "true" );
    else
        MainWindow::self()->settings()->setValue( "Editor_show_spaces", "false" );
        
    if( m_spaceTabs )
        MainWindow::self()->settings()->setValue( "Editor_spaces_tabs", "true" );
    else
        MainWindow::self()->settings()->setValue( "Editor_spaces_tabs", "false" );
}
int CodeEditor::fontSize()
{
    return m_fontSize;
}
void CodeEditor::setFontSize( int size )
{
    m_fontSize = size;
    m_font.setPointSize( size );
    setFont( m_font );
    
    setTabSize( m_tabSize );
}

int CodeEditor::tabSize()
{
    return m_tabSize;
}
void CodeEditor::setTabSize( int size )
{
    m_tabSize = size;
    setTabStopWidth( m_tabSize*m_fontSize*2/3 );
    
    if( m_spaceTabs ) setSpaceTabs( true );
}

bool CodeEditor::showSpaces()
{
    return m_showSpaces;
}
void CodeEditor::setShowSpaces( bool on )
{
    m_showSpaces = on;
    
    QTextOption option =  document()->defaultTextOption();
    
    if( on ) option.setFlags(option.flags() | QTextOption::ShowTabsAndSpaces);

    else option.setFlags(option.flags() & ~QTextOption::ShowTabsAndSpaces);

    document()->setDefaultTextOption(option);
}

bool CodeEditor::spaceTabs()
{
    return m_spaceTabs;
}
void CodeEditor::setSpaceTabs( bool on )
{
    m_spaceTabs = on;

    if( on )
    {
        m_tab = "";
        for( int i=0; i<m_tabSize; i++) m_tab += " ";
    }
    else m_tab = "\t";
}

void CodeEditor::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_Plus && (event->modifiers() & Qt::ControlModifier) )
    {
        setFontSize( m_fontSize+1 );
    }
    else if( event->key() == Qt::Key_Minus && (event->modifiers() & Qt::ControlModifier) )
    {
        setFontSize( m_fontSize-1 );
    }
    else if( event->key() == Qt::Key_Tab )
    {
        if( textCursor().hasSelection() ) indentSelection( false );
        else                              insertPlainText( m_tab );
    }
    else if( event->key() == Qt::Key_Backtab )
    {
        if( textCursor().hasSelection() ) indentSelection( true );
        else
        {
            textCursor().movePosition( QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor , m_tab.size() );
        }
    }
    else 
    {
        int tabs = 0;
        if( event->key() == Qt::Key_Return )
        {
            int n0 = 0;
            int n = m_tab.size();
            QString line = textCursor().block().text();
            
            while(1)
            {
                QString part = line.mid( n0, n );
                if( part == m_tab )
                {
                    n0 += n;
                    tabs += 1;
                }
                else break;
            }
        } 
        QPlainTextEdit::keyPressEvent( event );
        
        if( event->key() == Qt::Key_Return )
        {
            for( int i=0; i<tabs; i++ ) insertPlainText( m_tab );
        }
    }
}

/*void CodeEditor::increaseSelectionIndent()
{
    QTextCursor curs = textCursor();

    // Get the first and count of lines to indent.

    int spos = curs.anchor();
    int epos = curs.position();

    if( spos > epos ) std::swap(spos, epos);

    curs.setPosition( spos, QTextCursor::MoveAnchor );
    int sblock = curs.block().blockNumber();

    curs.setPosition( epos, QTextCursor::MoveAnchor );
    int eblock = curs.block().blockNumber();

    // Do the indent.

    curs.setPosition( spos, QTextCursor::MoveAnchor );

    curs.beginEditBlock();

    for( int i = 0; i <= ( eblock-sblock); ++i )
    {
        curs.movePosition( QTextCursor::StartOfBlock, QTextCursor::MoveAnchor );

        curs.insertText( m_tab );

        curs.movePosition( QTextCursor::NextBlock, QTextCursor::MoveAnchor );
    }
    curs.endEditBlock();

    // Set our cursor's selection to span all of the involved lines.

    curs.setPosition(spos, QTextCursor::MoveAnchor);
    curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor );

    while( curs.block().blockNumber() < eblock )
    {
        curs.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor );
    }
    curs.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );

    setTextCursor( curs );
}*/

void CodeEditor::indentSelection( bool unIndent )
{
    QTextCursor cur = textCursor();
    int a = cur.anchor();
    int p = cur.position();
    
    cur.beginEditBlock();
     
    if( a > p ) std::swap( a, p );
    
    QString str = cur.selection().toPlainText();
    QString str2 = "";
    QStringList list = str.split("\n");
    
    int lines = list.count();
 
    for (int i = 0; i < lines; i++)
    {
        QString line = list[i];
        
        if( unIndent ) 
        {
            int n = m_tab.size();
            int n1 = n;
            int n2 = 0;
            
            while( n1 > 0 )
            {
                QString car = line.at(n2);
                
                if( car == " " ) 
                {
                    n1 -= 1;
                    n2 += 1;
                }
                else if( car == "\t" )
                {
                    n1 -= n;
                    if( n1 >= 0 ) n2 += 1;
                }
                else n1 = 0;
            }
            line.replace( 0, n2, "" );
        }
        else line.insert( 0, m_tab );
        
        if( i < lines-1 ) line += "\n";

        str2 += line;
    }
    cur.removeSelectedText();
    cur.insertText(str2);
    p = cur.position();

    cur.setPosition( a );
    cur.setPosition( p, QTextCursor::KeepAnchor );

    setTextCursor(cur);

    cur.endEditBlock();
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
    
    if( !m_codeEditor->debugStarted() ) return;
    
    QMenu menu;

    QAction *addm_brkAction = menu.addAction( QIcon(":/remove.png"),"Add BreakPoint" );
    connect( addm_brkAction, SIGNAL( triggered()), m_codeEditor, SLOT(slotAddBreak()) );

    QAction *remm_brkAction = menu.addAction( QIcon(":/remove.png"),"Remove BreakPoint" );
    connect( remm_brkAction, SIGNAL( triggered()), m_codeEditor, SLOT(slotRemBreak()) );

    if( menu.exec(event->globalPos()) != 0 ) lastPos = event->pos().y();
}

#include "moc_codeeditor.cpp"
