/********************************************************************************
** Form generated from reading UI file 'explain.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPLAIN_H
#define UI_EXPLAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Explain
{
public:

    void setupUi(QDialog *Explain)
    {
        if (Explain->objectName().isEmpty())
            Explain->setObjectName(QStringLiteral("Explain"));
        Explain->resize(600, 400);
        Explain->setStyleSheet(QStringLiteral("border-image: url(:/image/information.jpg);"));

        retranslateUi(Explain);

        QMetaObject::connectSlotsByName(Explain);
    } // setupUi

    void retranslateUi(QDialog *Explain)
    {
        Explain->setWindowTitle(QApplication::translate("Explain", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class Explain: public Ui_Explain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPLAIN_H
