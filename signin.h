#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef SIGNIN_H
#define SIGNIN_H
 
namespace Ui {

class SignIn;

}
 
 
class SignIn : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit SignIn(QWidget *parent = 0);  //explicit 防止歧义
    ~SignIn();

	QDateTime getcurDateTimedata();
	QString getsigninnamedata();
	QDateTime getsigninTime_QDateEditdata();
	QString getsignin_shitfdata();


signals:



	public slots:
		//void SignOutlogin();//点击接班登录按钮执行的槽函数
		void on_okButton_clicked();

    
	private slots:
		//void showInformationDialog();
private:

	//定义private属性信息，供给其他类使用
	//1获取当前日期时间
	QDateTime signincurDateTime;

	//2获取用户选取的交班人
	QString signinname;



	//3获取交接班时间
	QDateTime signinTime_QDateEdit;

	//4获取交接班序号
	QString signin_shitf;

	//用户
	std::list<UserObjectData>* signinlistUser;

	Ui::SignIn *ui;

};
 
#endif // SIGNIN_H