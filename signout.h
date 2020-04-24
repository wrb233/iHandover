#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef SIGNOUT_H
#define SIGNOUT_H
 
namespace Ui {

class SignOut;

}
 
 
class SignOut : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit SignOut(QWidget *parent = 0);  //explicit 防止歧义
    ~SignOut();


public:
	QDateTime getcurDateTimedata();
	QString getsignoutnamedata();
	QDateTime getsignoutTime_QDateEditdata();
	QString getsignout_shitfdata();

	



	
	




signals:
	void sendsignoutcurDateTimeData(QDateTime);
	void sendsignoutnameData(QString);
	void sendsignoutTime_QDateEditData(QDateTime);

	void sendsignout_shiftData(QString);
 


	
public slots:
    //void SignOutlogin();//点击交班登录按钮执行的槽函数
	void on_okButton_clicked();
private slots:
	//void showInformationDialog();
private:


	//定义private属性信息，供给其他类使用
	//1获取当前日期时间
	QDateTime signoutcurDateTime;

	//2获取用户选取的交班人
	QString signoutname;

	

	//3获取交接班时间
	QDateTime signoutTime_QDateEdit;

	//4获取交接班序号
	QString signout_shitf;



	//用户
	std::list<UserObjectData>* signoutlistUser;

	

	Ui::SignOut *ui;

};
 
#endif // SIGNOUT_H