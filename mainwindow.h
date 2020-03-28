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


//��slots������Ӧ�ĺ����������
private slots:
	void open();
//���õ��Ŀؼ������¶���һ��
private:
	QAction *actionExport;
	
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
