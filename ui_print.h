/********************************************************************************
** Form generated from reading UI file 'print.ui'
**
** Created: Thu May 7 09:29:40 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINT_H
#define UI_PRINT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Print
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *from;
    QDateEdit *fromTime_QDateEdit;
    QComboBox *fromcomboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *to;
    QDateEdit *toTime_QDateEdit;
    QComboBox *tocomboBox;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Print)
    {
        if (Print->objectName().isEmpty())
            Print->setObjectName(QString::fromUtf8("Print"));
        Print->resize(345, 233);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        Print->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(Print);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        from = new QLabel(Print);
        from->setObjectName(QString::fromUtf8("from"));
        from->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(from);

        fromTime_QDateEdit = new QDateEdit(Print);
        fromTime_QDateEdit->setObjectName(QString::fromUtf8("fromTime_QDateEdit"));

        horizontalLayout->addWidget(fromTime_QDateEdit);

        fromcomboBox = new QComboBox(Print);
        fromcomboBox->setObjectName(QString::fromUtf8("fromcomboBox"));

        horizontalLayout->addWidget(fromcomboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        to = new QLabel(Print);
        to->setObjectName(QString::fromUtf8("to"));
        to->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(to);

        toTime_QDateEdit = new QDateEdit(Print);
        toTime_QDateEdit->setObjectName(QString::fromUtf8("toTime_QDateEdit"));

        horizontalLayout_2->addWidget(toTime_QDateEdit);

        tocomboBox = new QComboBox(Print);
        tocomboBox->setObjectName(QString::fromUtf8("tocomboBox"));

        horizontalLayout_2->addWidget(tocomboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(spacerItem);

        okButton = new QPushButton(Print);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_3->addWidget(okButton);

        cancelButton = new QPushButton(Print);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(Print);
        QObject::connect(okButton, SIGNAL(clicked()), Print, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Print, SLOT(reject()));

        QMetaObject::connectSlotsByName(Print);
    } // setupUi

    void retranslateUi(QDialog *Print)
    {
        Print->setWindowTitle(QApplication::translate("Print", "\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        from->setText(QApplication::translate("Print", "\344\273\216", 0, QApplication::UnicodeUTF8));
        to->setText(QApplication::translate("Print", "\345\210\260", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Print", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Print", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Print: public Ui_Print {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINT_H
