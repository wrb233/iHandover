#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void showDPCPoint();


//把slots作出反应的函数都定义好
private slots:
	void open();
//把用到的控件都重新定义一遍
private:
	QAction *actionExport;
	
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
