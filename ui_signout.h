/********************************************************************************
** Form generated from reading UI file 'signout.ui'
**
** Created: Tue Apr 14 11:09:41 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNOUT_H
#define UI_SIGNOUT_H

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

class Ui_SignOut
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *signoutperson;
    QComboBox *signoutname;
    QHBoxLayout *horizontalLayout_2;
    QLabel *signouttime;
    QDateEdit *signoutTime_QDateEdit;
    QComboBox *signout_shift;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SignOut)
    {
        if (SignOut->objectName().isEmpty())
            SignOut->setObjectName(QString::fromUtf8("SignOut"));
        SignOut->resize(345, 233);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/signout.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignOut->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SignOut);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        signoutperson = new QLabel(SignOut);
        signoutperson->setObjectName(QString::fromUtf8("signoutperson"));
        signoutperson->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(signoutperson);

        signoutname = new QComboBox(SignOut);
        signoutname->setObjectName(QString::fromUtf8("signoutname"));

        horizontalLayout->addWidget(signoutname);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        signouttime = new QLabel(SignOut);
        signouttime->setObjectName(QString::fromUtf8("signouttime"));

        horizontalLayout_2->addWidget(signouttime);

        signoutTime_QDateEdit = new QDateEdit(SignOut);
        signoutTime_QDateEdit->setObjectName(QString::fromUtf8("signoutTime_QDateEdit"));

        horizontalLayout_2->addWidget(signoutTime_QDateEdit);

        signout_shift = new QComboBox(SignOut);
        signout_shift->setObjectName(QString::fromUtf8("signout_shift"));

        horizontalLayout_2->addWidget(signout_shift);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(spacerItem);

        okButton = new QPushButton(SignOut);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_3->addWidget(okButton);

        cancelButton = new QPushButton(SignOut);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(SignOut);
        QObject::connect(okButton, SIGNAL(clicked()), SignOut, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), SignOut, SLOT(reject()));

        QMetaObject::connectSlotsByName(SignOut);
    } // setupUi

    void retranslateUi(QDialog *SignOut)
    {
        SignOut->setWindowTitle(QApplication::translate("SignOut", "\344\272\244\347\217\255", 0, QApplication::UnicodeUTF8));
        signoutperson->setText(QApplication::translate("SignOut", "\344\272\244\347\217\255\344\272\272", 0, QApplication::UnicodeUTF8));
        signouttime->setText(QApplication::translate("SignOut", "\344\272\244\347\217\255\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("SignOut", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SignOut", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SignOut: public Ui_SignOut {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNOUT_H
