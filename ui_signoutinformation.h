/********************************************************************************
** Form generated from reading UI file 'signoutinformation.ui'
**
** Created: Tue Jul 28 14:01:42 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNOUTINFORMATION_H
#define UI_SIGNOUTINFORMATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignOutInformation
{
public:
    QGridLayout *gridLayout_5;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QTableView *accidentinfo_tableview;
    QWidget *tab_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Add;
    QPushButton *Del;
    QTableView *maintenanceplan_tableview;
    QWidget *tab_3;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label;
    QTableView *DPCPoint_tableview;
    QWidget *tab_4;
    QGridLayout *gridLayout_2;
    QTableView *lineload_tableview;

    void setupUi(QDialog *SignOutInformation)
    {
        if (SignOutInformation->objectName().isEmpty())
            SignOutInformation->setObjectName(QString::fromUtf8("SignOutInformation"));
        SignOutInformation->resize(1266, 552);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/information.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignOutInformation->setWindowIcon(icon);
        gridLayout_5 = new QGridLayout(SignOutInformation);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(SignOutInformation);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(SignOutInformation);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        gridLayout_5->addLayout(hboxLayout, 1, 0, 1, 1);

        tabWidget = new QTabWidget(SignOutInformation);
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
        layoutWidget = new QWidget(tab_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(13, 14, 1220, 441));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        Add = new QPushButton(layoutWidget);
        Add->setObjectName(QString::fromUtf8("Add"));

        horizontalLayout_2->addWidget(Add);

        Del = new QPushButton(layoutWidget);
        Del->setObjectName(QString::fromUtf8("Del"));

        horizontalLayout_2->addWidget(Del);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        maintenanceplan_tableview = new QTableView(layoutWidget);
        maintenanceplan_tableview->setObjectName(QString::fromUtf8("maintenanceplan_tableview"));

        gridLayout->addWidget(maintenanceplan_tableview, 1, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        layoutWidget2 = new QWidget(tab_3);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(13, 14, 1220, 441));
        gridLayout_6 = new QGridLayout(layoutWidget2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        label_6 = new QLabel(layoutWidget2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        label_5 = new QLabel(layoutWidget2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);

        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        label = new QLabel(layoutWidget2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);


        gridLayout_6->addLayout(horizontalLayout, 0, 0, 1, 1);

        DPCPoint_tableview = new QTableView(layoutWidget2);
        DPCPoint_tableview->setObjectName(QString::fromUtf8("DPCPoint_tableview"));

        gridLayout_6->addWidget(DPCPoint_tableview, 1, 0, 1, 1);

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


        retranslateUi(SignOutInformation);
        QObject::connect(okButton, SIGNAL(clicked()), SignOutInformation, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), SignOutInformation, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SignOutInformation);
    } // setupUi

    void retranslateUi(QDialog *SignOutInformation)
    {
        SignOutInformation->setWindowTitle(QApplication::translate("SignOutInformation", "\344\272\244\347\217\255\344\277\241\346\201\257\351\241\265\351\235\242", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("SignOutInformation", "\347\241\256\350\256\244\345\275\225\345\205\245", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SignOutInformation", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SignOutInformation", "\344\272\213\346\225\205\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        Add->setText(QApplication::translate("SignOutInformation", "\346\226\260\345\242\236\344\270\200\350\241\214", 0, QApplication::UnicodeUTF8));
        Del->setText(QApplication::translate("SignOutInformation", "\345\210\240\351\231\244\344\270\200\350\241\214", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SignOutInformation", "\346\243\200\344\277\256\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SignOutInformation", "\346\210\220\345\212\237\346\254\241\346\225\260:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        label_5->setText(QApplication::translate("SignOutInformation", "\346\254\241", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SignOutInformation", "\345\244\261\350\264\245\346\254\241\346\225\260:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        label->setText(QApplication::translate("SignOutInformation", "\346\254\241", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("SignOutInformation", "\351\201\245\346\216\247\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("SignOutInformation", "\347\272\277\350\267\257\350\264\237\350\275\275", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SignOutInformation: public Ui_SignOutInformation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNOUTINFORMATION_H
