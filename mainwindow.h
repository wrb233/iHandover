#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <string>
#include <QDateTime>
#include "libORM/orm.h"



//�����õ�
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
	


//��slots������Ӧ�ĺ����������
private slots:

	void showSignOutDialog();

	void showSignInDialog();

	void showConfirmDialog();

	void queryAtTime();
	
//���õ��Ŀؼ������¶���һ��
private:
	
	QAction *actionSignOut;
	QAction *actionSignIn;
	QAction *actionConfirm;
    
private:
    Ui::MainWindow *ui;

	
};

#endif // MAINWINDOW_H
