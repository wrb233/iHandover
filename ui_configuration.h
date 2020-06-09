/********************************************************************************
** Form generated from reading UI file 'configuration.ui'
**
** Created: Wed Jun 10 01:02:01 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURATION_H
#define UI_CONFIGURATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Configuration
{
public:
    QGridLayout *gridLayout;
    QPushButton *Add;
    QPushButton *Del;
    QPushButton *okButton;
    QTableWidget *tableWidgetConfiguration;

    void setupUi(QDialog *Configuration)
    {
        if (Configuration->objectName().isEmpty())
            Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Configuration->resize(598, 286);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/configuration.png"), QSize(), QIcon::Normal, QIcon::Off);
        Configuration->setWindowIcon(icon);
        gridLayout = new QGridLayout(Configuration);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Add = new QPushButton(Configuration);
        Add->setObjectName(QString::fromUtf8("Add"));

        gridLayout->addWidget(Add, 1, 0, 1, 1);

        Del = new QPushButton(Configuration);
        Del->setObjectName(QString::fromUtf8("Del"));

        gridLayout->addWidget(Del, 1, 1, 1, 1);

        okButton = new QPushButton(Configuration);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 1, 2, 1, 1);

        tableWidgetConfiguration = new QTableWidget(Configuration);
        if (tableWidgetConfiguration->columnCount() < 3)
            tableWidgetConfiguration->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetConfiguration->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetConfiguration->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetConfiguration->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidgetConfiguration->setObjectName(QString::fromUtf8("tableWidgetConfiguration"));

        gridLayout->addWidget(tableWidgetConfiguration, 0, 0, 1, 3);


        retranslateUi(Configuration);

        QMetaObject::connectSlotsByName(Configuration);
    } // setupUi

    void retranslateUi(QDialog *Configuration)
    {
        Configuration->setWindowTitle(QApplication::translate("Configuration", "\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        Add->setText(QApplication::translate("Configuration", "\346\226\260\345\242\236", 0, QApplication::UnicodeUTF8));
        Del->setText(QApplication::translate("Configuration", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Configuration", "\347\241\256\350\256\244\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configuration: public Ui_Configuration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURATION_H
