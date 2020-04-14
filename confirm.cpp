#include "confirm.h"
#include "ui_confirm.h"
#include <QMessageBox>


#include "signin.h"
#include "signout.h"
#include "information.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <QString>

#include <QDebug>

#include <QList>

extern DBPOOLHANDLE dbPoolHandle;


Confirm::Confirm(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Confirm)
{
    

	ui->setupUi(this);

    
	
	//用户点击ok，将输入的信息插到交接班交接记录总表
	//connect(ui->okButton,SIGNAL(clicked()),this, SLOT(insertSignInAndOutTable()));


	//初始化用户在确认交接班对话页面输入的交班人、接班人、交接班时间、交接备注内容（为空）、上班地点信息
    
	//交班人
	ui->signoutname->clear(); //清除列表
	for (int i=0;i<20;i++)
		ui->signoutname->addItem(QString::number(i)); //不带图标
	//接班人
	ui->signinname->clear(); //清除列表
	for (int i=0;i<20;i++)
		ui->signinname->addItem(QString::number(i)); //不带图标
	//交接班次
	ui->signinandout_shift->clear(); //清除列表
	for (int i=0;i<3;i++)
		ui->signinandout_shift->addItem(QString::number(i)); //不带图标
	//上班地点信息
	ui->workplacename->clear(); //清除列表
	for (int i=100;i<106;i++)
		ui->workplacename->addItem(QString::number(i)); //不带图标

	//交接班时间
	QDateTime current_date_time =QDateTime::currentDateTime();
	QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signinandoutTime_QDateEdit->setDate(date);

	
	}
 Confirm::~Confirm()

{

    delete ui;

}


 /*
 void Confirm::insertSignInAndOutTable()
 {


	 //测试万金油
	 //QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	 //Information information;

	 //information.exec();


 }
 */

 //用户点击ok，就在交接班交接记录总表添加一条交接记录
 void Confirm::on_okButton_clicked()
 {
	 
	 //千万可不敢再定义变量数据类型了，造成菊部变量
	 
	 //1获取当前日期时间
	 curDateTime=QDateTime::currentDateTime();
	 
	 //2获取用户选取的交班人
	 signoutname = ui->signoutname->currentText();

	 //3获取用户选取的接班人
	 signinname = ui->signinname->currentText();

	 //4获取交接班时间
	 signinandoutTime_QDateEdit = ui->signinandoutTime_QDateEdit->dateTime();
	 
	 //5获取交接班序号
	 signinandout_shitf = ui->signinandout_shift->currentText();

	 //6获取交接备注内容
	 signinandoutNote_QTextEdit = ui->signinandoutNote_QTextEdit->toPlainText();

	 //7获取用户选取的上班地点
	 workplacename = ui->workplacename->currentText();

	 RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	 //拼接交接班交接记录总表的sql语句
	 QString signinandoutsql = QString("INSERT INTO H_SIGNINANDOUT (TIME,HANDOVER,SUCCESSOR,WORK_DAY,WORK_INDEX,NOTE,WORK_PLACE) VALUES (to_timestamp('"+curDateTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss'),"\
		 +QString("%1").arg(signoutname)+","\
		 +QString("%2").arg(signinname)+",to_timestamp('"+signinandoutTime_QDateEdit.toString("yyyy-MM-dd")+"','yyyy-MM-dd'),"\
		 +QString("%4").arg(signinandout_shitf)+",'"\
		 +QString("%5").arg(signinandoutNote_QTextEdit)+"',"\
		 +QString("%6").arg(workplacename)+")");
	 //调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
	 InitConTable::executeSQL(signinandoutsql,0);
	 CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);
	 //MainWindow::showSignInAndOut();
	 
	 

	
	
		 

 }


 //为每一个属性写一遍get方法
 QDateTime Confirm::getcurDateTimedata()
 {	 
	 return curDateTime;
 }
 QString Confirm::getsignoutnamedata()
 {	 
	 return signoutname;
 }
 QString Confirm::getsigninnamedata()
 {	 
	 return signinname;
 }
 QDateTime Confirm::getsigninandoutTime_QDateEditdata()
 {
	return signinandoutTime_QDateEdit;
 }
 QString Confirm::getsigninandout_shitfdata()
 {	 
	 return signinandout_shitf;
 }
 QString Confirm::getsigninandoutNote_QTextEditdata()
 {	 
	 return signinandoutNote_QTextEdit;
 }
 QString Confirm::getworkplacenamedata()
 {	 
	 return workplacename;
 }

