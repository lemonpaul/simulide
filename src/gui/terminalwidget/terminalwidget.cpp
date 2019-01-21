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

#include "terminalwidget.h"
#include "baseprocessor.h"

TerminalWidget* TerminalWidget::m_pSelf = 0l;

TerminalWidget::TerminalWidget( QWidget *parent )
    : QWidget( parent )
    ,m_verticalLayout(this)
    ,m_sendLayout()
    ,m_textLayout()
    ,m_sendText(this)
    ,m_sendValue(this)
    ,m_uartInPanel(this)
    ,m_uartOutPanel(this)
    ,m_ascciButton(this)
    ,m_valueButton(this)
{
    m_pSelf = this;
    this->setVisible( false );

    m_printASCII = true;
    
    setMinimumSize(QSize(200, 200));
    
    m_verticalLayout.setObjectName( "verticalLayout" );
    m_verticalLayout.setContentsMargins(0, 0, 0, 0);
    m_verticalLayout.setSpacing(4);

    QLabel* sendTextLabel = new QLabel(this);
    sendTextLabel->setText(tr("Send Text:"));
    m_sendText.setMaxLength( 50 );
    
    QLabel* sendValueLabel = new QLabel(this);
    sendValueLabel->setText(tr("    Send Value:"));
    m_sendValue.setMaxLength( 3 );
    m_sendValue.setMaximumWidth(40);
    m_sendValue.setValidator( new QIntValidator(0, 255, this) );

    QLabel* printLabel = new QLabel(this);
    printLabel->setText(tr("    Print:"));

    m_ascciButton.setCheckable(true);
    m_ascciButton.setForegroundRole( QPalette::BrightText );
    m_ascciButton.setFixedSize( 50, 20 );
    m_ascciButton.setText( " ASCII " );
    m_ascciButton.setChecked( m_printASCII );

    m_valueButton.setCheckable(true);
    m_valueButton.setForegroundRole( QPalette::BrightText );
    m_valueButton.setFixedSize( 50, 20 );
    m_valueButton.setText( tr(" Value ") );
    m_valueButton.setChecked( !m_printASCII );
    
    m_sendLayout.setSpacing(4);
    m_sendLayout.setContentsMargins(2, 2, 4, 2);
    m_sendLayout.addWidget( sendTextLabel );
    m_sendLayout.addWidget( &m_sendText );
    m_sendLayout.addWidget( sendValueLabel );
    m_sendLayout.addWidget( &m_sendValue );
    m_sendLayout.addWidget( printLabel );
    m_sendLayout.addWidget( &m_ascciButton );
    m_sendLayout.addWidget( &m_valueButton );
    m_verticalLayout.addLayout( &m_sendLayout );
    
    /*QFrame* myFrame = new QFrame();
    myFrame->setFrameShape(QFrame::HLine);
    m_verticalLayout.addWidget( myFrame );*/
    
    QHBoxLayout* textLabelsLayout = new QHBoxLayout();
    QLabel* sentLabel = new QLabel(this);
    sentLabel->setText(tr("Received From Micro:"));
    textLabelsLayout->addWidget( sentLabel );
    QLabel* recvLabel = new QLabel(this);
    recvLabel->setText(tr("Sent to Micro:"));
    textLabelsLayout->addWidget( recvLabel );
    m_verticalLayout.addLayout( textLabelsLayout );
    
    m_textLayout.addWidget( &m_uartOutPanel );
    m_textLayout.addWidget( &m_uartInPanel );
    m_verticalLayout.addLayout( &m_textLayout );

    
    connect( &m_sendText, SIGNAL( returnPressed() ),
                    this, SLOT( onTextChanged() ));
                    
    connect( &m_sendValue, SIGNAL( returnPressed() ),
                     this, SLOT( onValueChanged() ));

    connect( &m_ascciButton, SIGNAL( clicked()),
                       this, SLOT( ascciButtonClicked()) );

    connect( &m_valueButton, SIGNAL( clicked()),
                       this, SLOT( valueButtonClicked()) );
}
TerminalWidget::~TerminalWidget() { }

void TerminalWidget::onTextChanged()
{
    QString text = m_sendText.text();
    //qDebug() << text ;
    
    QByteArray array = text.toLatin1();
    
    for( int i=0; i<array.size(); i++ ) BaseProcessor::self()->uartIn( array.at(i) );
}

void TerminalWidget::onValueChanged()
{
    QString text = m_sendValue.text();

    BaseProcessor::self()->uartIn( text.toInt() );
}

void TerminalWidget::valueButtonClicked()
{
    m_printASCII = !m_valueButton.isChecked();
    m_ascciButton.setChecked( m_printASCII );
}

void TerminalWidget::ascciButtonClicked()
{
    m_printASCII = m_ascciButton.isChecked();
    m_valueButton.setChecked( !m_printASCII );
}

void TerminalWidget::step()
{
    m_uartInPanel.step();
    m_uartOutPanel.step();
}

void TerminalWidget::uartIn( uint32_t value ) // Receive one byte on Uart
{
    QString text = "";
    if( m_printASCII )
    {
        if( value == 0 ) return;
        text.append( value );
    }
    else text = QString::number( value )+" ";

    m_uartInPanel.appendText( text );
}

void TerminalWidget::uartOut( uint32_t value ) // Send value to OutPanelText
{
    QString text = "";
    if( m_printASCII )
    {
        if( value == 0 ) return;
        text.append( value );
    }
    else text = QString::number( value )+" ";

    m_uartOutPanel.appendText( text );
}


#include "moc_terminalwidget.cpp"
