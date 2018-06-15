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

#ifndef UI_SERIALPORTWIDGET_H
#define UI_SERIALPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialPortWidget
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *selectBox;
    QGridLayout *gridLayout;
    QComboBox *serialPortInfoListBox;
    QLabel *manufacturerLabel;
    QLabel *serialNumberLabel;
    QLabel *descriptionLabel;
    QPushButton *openButton;
    QPushButton *closeButton;
    QGroupBox *parametersBox;
    QGridLayout *gridLayout_2;
    QLabel *parityLabel;
    QLabel *flowControlLabel;
    QComboBox *baudRateBox;
    QComboBox *stopBitsBox;
    QComboBox *dataBitsBox;
    QComboBox *parityBox;
    QComboBox *flowControlBox;
    QLabel *stopBitsLabel;
    QLabel *baudRateLabel;
    QLabel *dataBitsLabel;

    void setupUi(QWidget *SerialPortWidget)
    {
        if (SerialPortWidget->objectName().isEmpty())
            SerialPortWidget->setObjectName(QStringLiteral("SerialPortWidget"));
        SerialPortWidget->resize(313, 141);
        SerialPortWidget->setMaximumSize(QSize(450, 170));
        gridLayout_3 = new QGridLayout(SerialPortWidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setVerticalSpacing(0);
        gridLayout_3->setContentsMargins(4, 0, 4, 6);
        selectBox = new QGroupBox(SerialPortWidget);
        selectBox->setObjectName(QStringLiteral("selectBox"));
        selectBox->setTitle(QStringLiteral(""));
        gridLayout = new QGridLayout(selectBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(2);
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        serialPortInfoListBox = new QComboBox(selectBox);
        serialPortInfoListBox->setObjectName(QStringLiteral("serialPortInfoListBox"));

        gridLayout->addWidget(serialPortInfoListBox, 0, 0, 1, 1);

        manufacturerLabel = new QLabel(selectBox);
        manufacturerLabel->setObjectName(QStringLiteral("manufacturerLabel"));

        gridLayout->addWidget(manufacturerLabel, 4, 0, 1, 1);

        serialNumberLabel = new QLabel(selectBox);
        serialNumberLabel->setObjectName(QStringLiteral("serialNumberLabel"));

        gridLayout->addWidget(serialNumberLabel, 5, 0, 1, 1);

        descriptionLabel = new QLabel(selectBox);
        descriptionLabel->setObjectName(QStringLiteral("descriptionLabel"));

        gridLayout->addWidget(descriptionLabel, 3, 0, 1, 1);

        openButton = new QPushButton(selectBox);
        openButton->setObjectName(QStringLiteral("openButton"));

        gridLayout->addWidget(openButton, 1, 0, 1, 1);

        closeButton = new QPushButton(selectBox);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setEnabled(false);

        gridLayout->addWidget(closeButton, 2, 0, 1, 1);


        gridLayout_3->addWidget(selectBox, 0, 0, 1, 1);

        parametersBox = new QGroupBox(SerialPortWidget);
        parametersBox->setObjectName(QStringLiteral("parametersBox"));
        parametersBox->setTitle(QStringLiteral(""));
        gridLayout_2 = new QGridLayout(parametersBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(2);
        gridLayout_2->setContentsMargins(-1, 0, -1, 0);
        parityLabel = new QLabel(parametersBox);
        parityLabel->setObjectName(QStringLiteral("parityLabel"));

        gridLayout_2->addWidget(parityLabel, 2, 0, 1, 1);

        flowControlLabel = new QLabel(parametersBox);
        flowControlLabel->setObjectName(QStringLiteral("flowControlLabel"));

        gridLayout_2->addWidget(flowControlLabel, 4, 0, 1, 1);

        baudRateBox = new QComboBox(parametersBox);
        baudRateBox->setObjectName(QStringLiteral("baudRateBox"));

        gridLayout_2->addWidget(baudRateBox, 0, 1, 1, 1);

        stopBitsBox = new QComboBox(parametersBox);
        stopBitsBox->setObjectName(QStringLiteral("stopBitsBox"));

        gridLayout_2->addWidget(stopBitsBox, 3, 1, 1, 1);

        dataBitsBox = new QComboBox(parametersBox);
        dataBitsBox->setObjectName(QStringLiteral("dataBitsBox"));

        gridLayout_2->addWidget(dataBitsBox, 1, 1, 1, 1);

        parityBox = new QComboBox(parametersBox);
        parityBox->setObjectName(QStringLiteral("parityBox"));

        gridLayout_2->addWidget(parityBox, 2, 1, 1, 1);

        flowControlBox = new QComboBox(parametersBox);
        flowControlBox->setObjectName(QStringLiteral("flowControlBox"));

        gridLayout_2->addWidget(flowControlBox, 4, 1, 1, 1);

        stopBitsLabel = new QLabel(parametersBox);
        stopBitsLabel->setObjectName(QStringLiteral("stopBitsLabel"));

        gridLayout_2->addWidget(stopBitsLabel, 3, 0, 1, 1);

        baudRateLabel = new QLabel(parametersBox);
        baudRateLabel->setObjectName(QStringLiteral("baudRateLabel"));

        gridLayout_2->addWidget(baudRateLabel, 0, 0, 1, 1);

        dataBitsLabel = new QLabel(parametersBox);
        dataBitsLabel->setObjectName(QStringLiteral("dataBitsLabel"));

        gridLayout_2->addWidget(dataBitsLabel, 1, 0, 1, 1);


        gridLayout_3->addWidget(parametersBox, 0, 1, 1, 1);


        retranslateUi(SerialPortWidget);

        QMetaObject::connectSlotsByName(SerialPortWidget);
    } // setupUi

    void retranslateUi(QWidget *SerialPortWidget)
    {
        SerialPortWidget->setWindowTitle(QApplication::translate("SerialPortWidget", "Settings", 0));
        manufacturerLabel->setText(QApplication::translate("SerialPortWidget", "Manufacturer:", 0));
        serialNumberLabel->setText(QApplication::translate("SerialPortWidget", "Serial number:", 0));
        descriptionLabel->setText(QApplication::translate("SerialPortWidget", "Description:", 0));
        openButton->setText(QApplication::translate("SerialPortWidget", "Open", 0));
        closeButton->setText(QApplication::translate("SerialPortWidget", "Close", 0));
        parityLabel->setText(QApplication::translate("SerialPortWidget", "Parity:", 0));
        flowControlLabel->setText(QApplication::translate("SerialPortWidget", "Flow control:", 0));
        stopBitsLabel->setText(QApplication::translate("SerialPortWidget", "Stop bits:", 0));
        baudRateLabel->setText(QApplication::translate("SerialPortWidget", "BaudRate:", 0));
        dataBitsLabel->setText(QApplication::translate("SerialPortWidget", "Data bits:", 0));
    } // retranslateUi

};

namespace Ui {
    class SerialPortWidget: public Ui_SerialPortWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALPORTWIDGET_H
