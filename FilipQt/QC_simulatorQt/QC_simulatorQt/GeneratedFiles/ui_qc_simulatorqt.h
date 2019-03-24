/********************************************************************************
** Form generated from reading UI file 'qc_simulatorqt.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QC_SIMULATORQT_H
#define UI_QC_SIMULATORQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QC_simulatorQtClass
{
public:

    void setupUi(QWidget *QC_simulatorQtClass)
    {
        if (QC_simulatorQtClass->objectName().isEmpty())
            QC_simulatorQtClass->setObjectName(QStringLiteral("QC_simulatorQtClass"));
        QC_simulatorQtClass->resize(600, 400);

        retranslateUi(QC_simulatorQtClass);

        QMetaObject::connectSlotsByName(QC_simulatorQtClass);
    } // setupUi

    void retranslateUi(QWidget *QC_simulatorQtClass)
    {
        QC_simulatorQtClass->setWindowTitle(QApplication::translate("QC_simulatorQtClass", "QC_simulatorQt", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QC_simulatorQtClass: public Ui_QC_simulatorQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QC_SIMULATORQT_H
