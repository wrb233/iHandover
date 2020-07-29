/********************************************************************************
** Form generated from reading UI file 'signininformation.ui'
**
** Created: Tue Jul 28 13:30:55 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNININFORMATION_H
#define UI_SIGNININFORMATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignInInformation
{
public:
    QGridLayout *gridLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QTableView *accidentinfo_tableview;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QTableView *maintenanceplan_tableview;
    QWidget *tab_3;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout;
    QTableView *DPCPoint_tableview;
    QWidget *tab_4;
    QGridLayout *gridLayout_2;
    QTableView *lineload_tableview;

    void setupUi(QDialog *SignInInformation)
    {
        if (SignInInformation->objectName().isEmpty())
            SignInInformation->setObjectName(QString::fromUtf8("SignInInformation"));
        SignInInformation->resize(1328, 552);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/information.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignInInformation->setWindowIcon(icon);
        gridLayout_5 = new QGridLayout(SignInInformation);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        tabWidget = new QTabWidget(SignInInformation);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        accidentinfo_tableview = new QTableView(tab);
        accidentinfo_tableview->setObjectName(QString::fromUtf8("accidentinfo_tableview"));

        gridLayout_4->addWidget(accidentinfo_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        maintenanceplan_tableview = new QTableView(tab_2);
        maintenanceplan_tableview->setObjectName(QString::fromUtf8("maintenanceplan_tableview"));

        gridLayout_3->addWidget(maintenanceplan_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_6 = new QGridLayout(tab_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        gridLayout_7->addLayout(horizontalLayout, 0, 0, 1, 1);

        DPCPoint_tableview = new QTableView(tab_3);
        DPCPoint_tableview->setObjectName(QString::fromUtf8("DPCPoint_tableview"));

        gridLayout_7->addWidget(DPCPoint_tableview, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_7, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_2 = new QGridLayout(tab_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lineload_tableview = new QTableView(tab_4);
        lineload_tableview->setObjectName(QString::fromUtf8("lineload_tableview"));

        gridLayout_2->addWidget(lineload_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());

        gridLayout_5->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(SignInInformation);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SignInInformation);
    } // setupUi

    void retranslateUi(QDialog *SignInInformation)
    {
        SignInInformation->setWindowTitle(QApplication::translate("SignInInformation", "\346\216\245\347\217\255\344\277\241\346\201\257\351\241\265\351\235\242", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SignInInformation", "\344\272\213\346\225\205\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SignInInformation", "\346\243\200\344\277\256\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("SignInInformation", "\351\201\245\346\216\247\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("SignInInformation", "\347\272\277\350\267\257\350\264\237\350\275\275", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SignInInformation: public Ui_SignInInformation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNININFORMATION_H
