/********************************************************************************
** Form generated from reading UI file 'confirm.ui'
**
** Created: Tue Jul 28 13:30:55 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRM_H
#define UI_CONFIRM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Confirm
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *signoutperson;
    QComboBox *signoutname;
    QSpacerItem *spacer_3;
    QLabel *signoutperson_2;
    QLineEdit *lineEdit;
    QLabel *signinperson;
    QComboBox *signinname;
    QSpacerItem *spacer_4;
    QLabel *signinperson_2;
    QLineEdit *lineEdit_2;
    QLabel *signinandoutTime;
    QDateEdit *signinandoutTime_QDateEdit;
    QComboBox *signinandout_shift;
    QSpacerItem *spacer_2;
    QLabel *signinandoutNote;
    QTextEdit *signinandoutNote_QTextEdit;
    QSpacerItem *spacer;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Confirm)
    {
        if (Confirm->objectName().isEmpty())
            Confirm->setObjectName(QString::fromUtf8("Confirm"));
        Confirm->resize(552, 400);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/confirm.png"), QSize(), QIcon::Normal, QIcon::Off);
        Confirm->setWindowIcon(icon);
        layoutWidget = new QWidget(Confirm);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(16, 3, 508, 384));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        signoutperson = new QLabel(layoutWidget);
        signoutperson->setObjectName(QString::fromUtf8("signoutperson"));

        gridLayout->addWidget(signoutperson, 0, 0, 1, 1);

        signoutname = new QComboBox(layoutWidget);
        signoutname->setObjectName(QString::fromUtf8("signoutname"));

        gridLayout->addWidget(signoutname, 0, 1, 1, 2);

        spacer_3 = new QSpacerItem(68, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer_3, 0, 3, 1, 2);

        signoutperson_2 = new QLabel(layoutWidget);
        signoutperson_2->setObjectName(QString::fromUtf8("signoutperson_2"));

        gridLayout->addWidget(signoutperson_2, 0, 5, 1, 2);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 7, 1, 2);

        signinperson = new QLabel(layoutWidget);
        signinperson->setObjectName(QString::fromUtf8("signinperson"));

        gridLayout->addWidget(signinperson, 1, 0, 1, 1);

        signinname = new QComboBox(layoutWidget);
        signinname->setObjectName(QString::fromUtf8("signinname"));

        gridLayout->addWidget(signinname, 1, 1, 1, 2);

        spacer_4 = new QSpacerItem(68, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer_4, 1, 3, 1, 2);

        signinperson_2 = new QLabel(layoutWidget);
        signinperson_2->setObjectName(QString::fromUtf8("signinperson_2"));

        gridLayout->addWidget(signinperson_2, 1, 5, 1, 2);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 1, 7, 1, 2);

        signinandoutTime = new QLabel(layoutWidget);
        signinandoutTime->setObjectName(QString::fromUtf8("signinandoutTime"));

        gridLayout->addWidget(signinandoutTime, 2, 0, 1, 2);

        signinandoutTime_QDateEdit = new QDateEdit(layoutWidget);
        signinandoutTime_QDateEdit->setObjectName(QString::fromUtf8("signinandoutTime_QDateEdit"));

        gridLayout->addWidget(signinandoutTime_QDateEdit, 2, 2, 1, 3);

        signinandout_shift = new QComboBox(layoutWidget);
        signinandout_shift->setObjectName(QString::fromUtf8("signinandout_shift"));

        gridLayout->addWidget(signinandout_shift, 2, 5, 1, 2);

        spacer_2 = new QSpacerItem(98, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer_2, 2, 7, 1, 2);

        signinandoutNote = new QLabel(layoutWidget);
        signinandoutNote->setObjectName(QString::fromUtf8("signinandoutNote"));

        gridLayout->addWidget(signinandoutNote, 3, 0, 1, 2);

        signinandoutNote_QTextEdit = new QTextEdit(layoutWidget);
        signinandoutNote_QTextEdit->setObjectName(QString::fromUtf8("signinandoutNote_QTextEdit"));

        gridLayout->addWidget(signinandoutNote_QTextEdit, 3, 2, 1, 6);

        spacer = new QSpacerItem(281, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer, 4, 4, 1, 5);

        spacerItem = new QSpacerItem(281, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 5, 0, 1, 6);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 5, 6, 1, 2);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 5, 8, 1, 1);


        retranslateUi(Confirm);
        QObject::connect(okButton, SIGNAL(clicked()), Confirm, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Confirm, SLOT(reject()));

        QMetaObject::connectSlotsByName(Confirm);
    } // setupUi

    void retranslateUi(QDialog *Confirm)
    {
        Confirm->setWindowTitle(QApplication::translate("Confirm", "\347\241\256\350\256\244\344\272\244\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
        signoutperson->setText(QApplication::translate("Confirm", "\344\272\244\347\217\255\344\272\272", 0, QApplication::UnicodeUTF8));
        signoutperson_2->setText(QApplication::translate("Confirm", "\344\272\244\347\217\255\344\272\272\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        signinperson->setText(QApplication::translate("Confirm", "\346\216\245\347\217\255\344\272\272", 0, QApplication::UnicodeUTF8));
        signinperson_2->setText(QApplication::translate("Confirm", "\346\216\245\347\217\255\344\272\272\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        signinandoutTime->setText(QApplication::translate("Confirm", "\344\272\244\346\216\245\347\217\255\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        signinandoutNote->setText(QApplication::translate("Confirm", "\344\272\244\346\216\245\345\244\207\346\263\250\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Confirm", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Confirm", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Confirm: public Ui_Confirm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRM_H
