/********************************************************************************
** Form generated from reading UI file 'findreplacedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDREPLACEDIALOG_H
#define UI_FINDREPLACEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <findreplaceform.h>

QT_BEGIN_NAMESPACE

class Ui_FindReplaceDialog
{
public:
    QGridLayout *gridLayout;
    FindReplaceForm *findReplaceForm;

    void setupUi(QDialog *FindReplaceDialog)
    {
        if (FindReplaceDialog->objectName().isEmpty())
            FindReplaceDialog->setObjectName(QStringLiteral("FindReplaceDialog"));
        FindReplaceDialog->resize(342, 140);
        gridLayout = new QGridLayout(FindReplaceDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        findReplaceForm = new FindReplaceForm(FindReplaceDialog);
        findReplaceForm->setObjectName(QStringLiteral("findReplaceForm"));

        gridLayout->addWidget(findReplaceForm, 0, 0, 1, 1);


        retranslateUi(FindReplaceDialog);

        QMetaObject::connectSlotsByName(FindReplaceDialog);
    } // setupUi

    void retranslateUi(QDialog *FindReplaceDialog)
    {
        FindReplaceDialog->setWindowTitle(QApplication::translate("FindReplaceDialog", "Find/Replace", 0));
    } // retranslateUi

};

namespace Ui {
    class FindReplaceDIALOG: public Ui_FindReplaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDREPLACEDIALOG_H
