/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Mar 30 22:06:02 2020
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
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExport;
    QAction *actionSave;
    QAction *actionRecord;
    QAction *actionAddRecord;
    QAction *actionQuery;
    QAction *actionPreviousQuery;
    QAction *actionHistoryQuery;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QTableView *accidentinfo_tableview;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTableView *maintenanceplan_tableview;
    QWidget *tab_3;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_4;
    QTableView *DPCPoint_tableview;
    QWidget *tab_4;
    QGridLayout *gridLayout_4;
    QTableView *lineload_tableview;
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
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport->setIcon(icon1);
        QFont font1;
        actionExport->setFont(font1);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionRecord = new QAction(MainWindow);
        actionRecord->setObjectName(QString::fromUtf8("actionRecord"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/record.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRecord->setIcon(icon3);
        actionAddRecord = new QAction(MainWindow);
        actionAddRecord->setObjectName(QString::fromUtf8("actionAddRecord"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/addrecord.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddRecord->setIcon(icon4);
        actionQuery = new QAction(MainWindow);
        actionQuery->setObjectName(QString::fromUtf8("actionQuery"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/query.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuery->setIcon(icon5);
        actionPreviousQuery = new QAction(MainWindow);
        actionPreviousQuery->setObjectName(QString::fromUtf8("actionPreviousQuery"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/previousquery.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreviousQuery->setIcon(icon6);
        actionHistoryQuery = new QAction(MainWindow);
        actionHistoryQuery->setObjectName(QString::fromUtf8("actionHistoryQuery"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/historyquery.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHistoryQuery->setIcon(icon7);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 0));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        accidentinfo_tableview = new QTableView(tab);
        accidentinfo_tableview->setObjectName(QString::fromUtf8("accidentinfo_tableview"));

        gridLayout->addWidget(accidentinfo_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        maintenanceplan_tableview = new QTableView(tab_2);
        maintenanceplan_tableview->setObjectName(QString::fromUtf8("maintenanceplan_tableview"));

        gridLayout_2->addWidget(maintenanceplan_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_6 = new QGridLayout(tab_3);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout->addWidget(label_5);

        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);


        gridLayout_5->addLayout(horizontalLayout, 0, 0, 1, 1);

        DPCPoint_tableview = new QTableView(tab_3);
        DPCPoint_tableview->setObjectName(QString::fromUtf8("DPCPoint_tableview"));

        gridLayout_5->addWidget(DPCPoint_tableview, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_4 = new QGridLayout(tab_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        lineload_tableview = new QTableView(tab_4);
        lineload_tableview->setObjectName(QString::fromUtf8("lineload_tableview"));

        gridLayout_4->addWidget(lineload_tableview, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

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

        mainToolBar->addAction(actionExport);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRecord);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAddRecord);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionQuery);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPreviousQuery);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHistoryQuery);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\231\272\350\203\275\344\272\244\346\216\245\347\217\255", 0, QApplication::UnicodeUTF8));
        actionExport->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExport->setToolTip(QApplication::translate("MainWindow", "\345\257\274\345\207\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("MainWindow", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRecord->setText(QApplication::translate("MainWindow", "\345\275\225\345\205\245\345\275\223\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRecord->setToolTip(QApplication::translate("MainWindow", "\345\275\225\345\205\245\345\275\223\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAddRecord->setText(QApplication::translate("MainWindow", "\350\241\245\345\275\225\345\211\215\344\270\200\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddRecord->setToolTip(QApplication::translate("MainWindow", "\350\241\245\345\275\225\345\211\215\344\270\200\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionQuery->setText(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\275\223\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionQuery->setToolTip(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\275\223\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPreviousQuery->setText(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\211\215\344\270\200\347\217\255", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPreviousQuery->setToolTip(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\211\215\344\270\200\347\217\255", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionHistoryQuery->setText(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\216\206\345\217\262", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHistoryQuery->setToolTip(QApplication::translate("MainWindow", "\346\237\245\350\257\242\345\216\206\345\217\262", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\344\272\213\346\225\205\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\346\243\200\344\277\256\350\256\241\345\210\222", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "         \346\210\220\345\212\237\346\254\241\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "                    8", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "\346\254\241", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "         \345\244\261\350\264\245\346\254\241\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "                    0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "\346\254\241", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\351\201\245\346\216\247\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "\347\272\277\350\267\257\350\264\237\350\275\275", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
