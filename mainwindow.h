#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <string>
#include <QDateTime>
#include "libORM/orm.h"



//经常用到
extern	QDateTime SetTimeFromDB(ORMTimeStamp time);



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	
	 void showSignInAndOut();
	


//把slots作出反应的函数都定义好
private slots:

	void showSignOutDialog();

	void showSignInDialog();

	void showConfirmDialog();

	void queryAtTime();
	
//把用到的控件都重新定义一遍
private:
	
	QAction *actionSignOut;
	QAction *actionSignIn;
	QAction *actionConfirm;
    
private:
    Ui::MainWindow *ui;

	
};

#endif // MAINWINDOW_H
