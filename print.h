#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef PRINT_H
#define PRINT_H
 
namespace Ui {

class Print;

}
 
 
class Print : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit Print(QWidget *parent = 0);  //explicit 防止歧义
    ~Print();

	
	QDateTime getfromDateTimedata();
	QString getfromshiftdata();
	QDateTime gettoDateTimedata();
	QString gettoshiftdata();

	QString getstrstarttimedata();
	QString getstrendtimedata();

signals:



	public slots:
		//void SignOutlogin();//点击接班登录按钮执行的槽函数
		void on_okButton_clicked();

    
	private slots:
		//void showInformationDialog();

		//void printReport(QPrinter* printer);
		void printReport(QPrinter* printer);
private:

	
	QDateTime fromTime_QDateEdit;

	
	QString from_shitf;

	QDateTime toTime_QDateEdit;


	QString to_shitf;


	QString strstarttime;
	QString strendtime;
	

	Ui::Print *ui;

};
 
#endif // PRINT_H