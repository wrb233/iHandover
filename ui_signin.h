/********************************************************************************
** Form generated from reading UI file 'signin.ui'
**
** Created: Tue Jul 28 13:30:55 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNIN_H
#define UI_SIGNIN_H

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

class Ui_SignIn
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *signinperson;
    QComboBox *signinname;
    QHBoxLayout *horizontalLayout_2;
    QLabel *signintime;
    QDateEdit *signinTime_QDateEdit;
    QComboBox *signin_shift;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SignIn)
    {
        if (SignIn->objectName().isEmpty())
            SignIn->setObjectName(QString::fromUtf8("SignIn"));
        SignIn->resize(345, 233);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/signin.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignIn->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SignIn);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        signinperson = new QLabel(SignIn);
        signinperson->setObjectName(QString::fromUtf8("signinperson"));
        signinperson->setMaximumSize(QSize(50, 16777215));

        horizontalLayout->addWidget(signinperson);

        signinname = new QComboBox(SignIn);
        signinname->setObjectName(QString::fromUtf8("signinname"));

        horizontalLayout->addWidget(signinname);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        signintime = new QLabel(SignIn);
        signintime->setObjectName(QString::fromUtf8("signintime"));

        horizontalLayout_2->addWidget(signintime);

        signinTime_QDateEdit = new QDateEdit(SignIn);
        signinTime_QDateEdit->setObjectName(QString::fromUtf8("signinTime_QDateEdit"));

        horizontalLayout_2->addWidget(signinTime_QDateEdit);

        signin_shift = new QComboBox(SignIn);
        signin_shift->setObjectName(QString::fromUtf8("signin_shift"));

        horizontalLayout_2->addWidget(signin_shift);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        okButton = new QPushButton(SignIn);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_3->addWidget(okButton);

        cancelButton = new QPushButton(SignIn);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(SignIn);
        QObject::connect(okButton, SIGNAL(clicked()), SignIn, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), SignIn, SLOT(reject()));

        QMetaObject::connectSlotsByName(SignIn);
    } // setupUi

    void retranslateUi(QDialog *SignIn)
    {
        SignIn->setWindowTitle(QApplication::translate("SignIn", "\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
        signinperson->setText(QApplication::translate("SignIn", "\346\216\245\347\217\255\344\272\272", 0, QApplication::UnicodeUTF8));
        signintime->setText(QApplication::translate("SignIn", "\346\216\245\347\217\255\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("SignIn", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SignIn", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SignIn: public Ui_SignIn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNIN_H
