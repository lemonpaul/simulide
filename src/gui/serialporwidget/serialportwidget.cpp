/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
* 
*  Modified 2017 by Santiago Gonzalez    santigoro@gmail.com
* 
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>

#include "serialportwidget.h"
#include "ui_serialportwidget.h"
#include "baseprocessor.h"

QT_USE_NAMESPACE

SerialPortWidget* SerialPortWidget::m_pSelf = 0l;

static const char blankString[] = QT_TRANSLATE_NOOP( "SerialPortWidget", "N/A" );

SerialPortWidget::SerialPortWidget( QWidget *parent )
                : QWidget(parent)
                , ui( new Ui::SerialPortWidget )
{
    ui->setupUi( this );

    m_pSelf = this;
    
    m_serial = new QSerialPort( this );

    intValidator = new QIntValidator( 0, 4000000, this );

    ui->baudRateBox->setInsertPolicy( QComboBox::NoInsert );

    connect( ui->openButton, &QPushButton::clicked,
                       this, &SerialPortWidget::open);
                       
    connect( ui->closeButton, &QPushButton::clicked,
                        this, &SerialPortWidget::close);
                        
    connect( ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                                  this, &SerialPortWidget::showPortInfo);
                                  
    connect( ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                        this, &SerialPortWidget::checkCustomBaudRatePolicy);
                        
    connect( ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                                  this, &SerialPortWidget::checkCustomDevicePathPolicy);

    connect( m_serial, &QSerialPort::readyRead, this, &SerialPortWidget::readData);
                 
    connect( this, &SerialPortWidget::getData, this, &SerialPortWidget::slotWriteData );

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
    
    this->setVisible( false );
}

SerialPortWidget::~SerialPortWidget()
{
    if( m_serial->isOpen() ) m_serial->close();
    delete ui;
}

void SerialPortWidget::setVisible( bool visible )
{
    QWidget::setVisible( visible );
    if( !visible && m_serial->isOpen() ) m_serial->close();
}

SerialPortWidget::Settings SerialPortWidget::settings() const
{
    return currentSettings;
}

QStringList SerialPortWidget::settingsProp()
{
    QStringList s;
    
    s << currentSettings.name
    << QString::number( ui->baudRateBox->currentIndex() )
    << QString::number( ui->dataBitsBox->currentIndex() )
    << QString::number( ui->parityBox->currentIndex() )
    << QString::number( ui->stopBitsBox->currentIndex() )
    << QString::number( ui->flowControlBox->currentIndex() ) ;
    
    return s;
}

void SerialPortWidget::setSettingsProp( QStringList s )
{
    ui->serialPortInfoListBox->setCurrentText( s.at(0) );

    int baud = s.at(1).toInt();
    if( baud == 5 ) ui->baudRateBox->setCurrentText( s.at(1) );
    else            ui->baudRateBox->setCurrentIndex( baud );

    ui->dataBitsBox->setCurrentIndex( s.at(2).toInt() );
    ui->parityBox->setCurrentIndex( s.at(3).toInt() );
    ui->stopBitsBox->setCurrentIndex( s.at(4).toInt() );
    ui->flowControlBox->setCurrentIndex( s.at(5).toInt() );
    
    updateSettings();
}

void SerialPortWidget::showPortInfo( int idx )
{
    if( idx == -1 ) return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText( tr( "Description: %1"  ).arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr( "Manufacturer: %1" ).arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr( "Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
}

void SerialPortWidget::open()
{
    updateSettings();

    SerialPortWidget::Settings p = settings();
    m_serial->setPortName( p.name );
    m_serial->setBaudRate( p.baudRate );
    m_serial->setDataBits( p.dataBits );
    m_serial->setParity(   p.parity );
    m_serial->setStopBits( p.stopBits );
    m_serial->setFlowControl( p.flowControl);

    if( m_serial->open(QIODevice::ReadWrite) )
    {
        ui->openButton->setEnabled( false );
        ui->closeButton->setEnabled( true );

        qDebug()<<(tr("Connected to %1 : %2, %3, %4, %5, %6")
                    .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                    .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    }
    else
    {
        QMessageBox::critical( this, tr("Error"), m_serial->errorString() );
    }
}

void SerialPortWidget::close()
{
    if( m_serial->isOpen() ) m_serial->close();
    ui->openButton->setEnabled( true );
    ui->closeButton->setEnabled( false );
}

void SerialPortWidget::readData()
{
    QByteArray data = m_serial->readAll();

    //qDebug()<<"SerialPortWidget::readData" << data;

    for( int i=0; i<data.size(); i++ ) BaseProcessor::self()->uartIn( data.at(i) );
}

void SerialPortWidget::writeData( const QByteArray &data )
{
    //qDebug() << "SerialPortWidget::writeData"<<data;
    emit getData( data );
}

void SerialPortWidget::slotWriteData( const QByteArray &data )
{
    if( m_serial->isOpen() )
    {
       m_serial->write( data );
        //qDebug() << "SerialPortWidget::slotWriteData"<<written<<data;
    }
}

void SerialPortWidget::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable( isCustomBaudRate );
    if( isCustomBaudRate )
    {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator( intValidator );
    }
}

void SerialPortWidget::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if( isCustomPath )
    {
        QString port = ui->serialPortInfoListBox->currentText();
        if( port == tr("Custom") ) port ="";
        ui->serialPortInfoListBox->clearEditText();
        ui->serialPortInfoListBox->setCurrentText( port );
    }
}

void SerialPortWidget::fillPortsParameters()
{
    ui->baudRateBox->addItem( QStringLiteral("9600"  ), QSerialPort::Baud9600);
    ui->baudRateBox->addItem( QStringLiteral("19200" ), QSerialPort::Baud19200);
    ui->baudRateBox->addItem( QStringLiteral("38400" ), QSerialPort::Baud38400);
    ui->baudRateBox->addItem( QStringLiteral("57600" ), QSerialPort::Baud57600);
    ui->baudRateBox->addItem( QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem( tr("Custom"));

    ui->dataBitsBox->addItem( QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem( QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem( QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem( QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr( "None"),  QSerialPort::NoParity);
    ui->parityBox->addItem(tr( "Even"),  QSerialPort::EvenParity);
    ui->parityBox->addItem(tr( "Odd" ),  QSerialPort::OddParity);
    ui->parityBox->addItem(tr( "Mark"),  QSerialPort::MarkParity);
    ui->parityBox->addItem(tr( "Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem( "1.5", QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr( "None"),     QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(    "RTS/CTS" ,  QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(    "XON/XOFF",  QSerialPort::SoftwareControl);
}

void SerialPortWidget::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;

    const QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    foreach( const QSerialPortInfo &info, infos )
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        #if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
        serialNumber = info.serialNumber();
        #else
        serialNumber = "";
        #endif
        list << info.portName()
             << (!description.isEmpty()  ? description  : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SerialPortWidget::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 5)
    {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();
}
#include "moc_serialportwidget.cpp"
