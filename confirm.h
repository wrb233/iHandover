#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef CONFIRM_H
#define CONFIRM_H
namespace Ui {
class Confirm;
}
class Confirm : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit Confirm(QWidget *parent = 0);  //explicit 防止歧义
    ~Confirm();
	//7个属性
	QDateTime getcurDateTimedata();
	QString getsigninnamedata();
	QString getsignoutnamedata();
	QDateTime getsigninandoutTime_QDateEditdata();
	QString getsigninandout_shitfdata();
	QString getsigninandoutNote_QTextEditdata();
	//QString getworkplacenamedata();
signals:
 
public slots:
	void on_okButton_clicked();
private slots:
	//void insertSignInAndOutTable();
private:
	
	//定义private属性信息，供给其他类使用

	//交班人接班人用户
	std::list<UserObjectData>* signoutlistUser;
	std::list<UserObjectData>* signinlistUser;

	//1获取当前日期时间
	QDateTime curDateTime;

	//2获取用户选取的交班人
	QString signoutname;
	

	//3获取用户选取的接班人
	QString signinname;
	

	//4获取交接班时间
	QDateTime signinandoutTime_QDateEdit;

	//5获取交接班序号
	QString signinandout_shitf;

	//6获取交接备注内容
	QString signinandoutNote_QTextEdit;

	//7获取用户选取的上班地点
	//QString workplacename;


	

	Ui::Confirm *ui;


};
 
#endif // CONFIRM_H