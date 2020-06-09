/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Jun 10 01:02:01 2020
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConfiguration;
    QAction *actionSignOut;
    QAction *actionSignIn;
    QAction *actionConfirm;
    QAction *actionPrint;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QDateEdit *startTime;
    QLabel *period;
    QDateEdit *endTime;
    QPushButton *queryAtTime;
    QSpacerItem *horizontalSpacer;
    QTableView *SignInAndOut_tableview;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1415, 740);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/handover.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionConfiguration = new QAction(MainWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/configuration.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfiguration->setIcon(icon1);
        actionSignOut = new QAction(MainWindow);
        actionSignOut->setObjectName(QString::fromUtf8("actionSignOut"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/signout.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSignOut->setIcon(icon2);
        QFont font1;
        actionSignOut->setFont(font1);
        actionSignIn = new QAction(MainWindow);
        actionSignIn->setObjectName(QString::fromUtf8("actionSignIn"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/signin.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSignIn->setIcon(icon3);
        actionConfirm = new QAction(MainWindow);
        actionConfirm->setObjectName(QString::fromUtf8("actionConfirm"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/confirm.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfirm->setIcon(icon4);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        startTime = new QDateEdit(splitter);
        startTime->setObjectName(QString::fromUtf8("startTime"));
        splitter->addWidget(startTime);
        period = new QLabel(splitter);
        period->setObjectName(QString::fromUtf8("period"));
        splitter->addWidget(period);
        endTime = new QDateEdit(splitter);
        endTime->setObjectName(QString::fromUtf8("endTime"));
        splitter->addWidget(endTime);
        queryAtTime = new QPushButton(splitter);
        queryAtTime->setObjectName(QString::fromUtf8("queryAtTime"));
        splitter->addWidget(queryAtTime);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        SignInAndOut_tableview = new QTableView(centralWidget);
        SignInAndOut_tableview->setObjectName(QString::fromUtf8("SignInAndOut_tableview"));

        gridLayout->addWidget(SignInAndOut_tableview, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1415, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setIconSize(QSize(50, 50));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionConfiguration);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSignOut);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSignIn);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionConfirm);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPrint);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\231\272\350\203\275\344\272\244\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
        actionConfiguration->setText(QApplication::translate("MainWindow", "\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConfiguration->setToolTip(QApplication::translate("MainWindow", "\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSignOut->setText(QApplication::translate("MainWindow", "\344\272\244\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSignOut->setToolTip(QApplication::translate("MainWindow", "\344\272\244\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSignIn->setText(QApplication::translate("MainWindow", "\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSignIn->setToolTip(QApplication::translate("MainWindow", "\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionConfirm->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConfirm->setToolTip(QApplication::translate("MainWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrint->setText(QApplication::translate("MainWindow", "\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrint->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        period->setText(QApplication::translate("MainWindow", " \350\207\263", 0, QApplication::UnicodeUTF8));
        queryAtTime->setText(QApplication::translate("MainWindow", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
