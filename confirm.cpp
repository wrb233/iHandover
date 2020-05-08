#include "confirm.h"
#include "ui_confirm.h"
#include "signin.h"
#include "signout.h"
#include "information.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"
#include <QString>
#include <QDebug>
#include <QList>
#include <QMessageBox>


extern DBPOOLHANDLE dbPoolHandle;
extern UserMSInterface *g_User;
extern  Database *database;

Confirm::Confirm(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Confirm)
{
	ui->setupUi(this);
	//初始化用户在确认交接班对话页面输入的交班人、接班人、交接班时间、交接备注内容（为空）、上班地点信息
	//1交班人
	ui->signoutname->clear(); //清除列表
	
	signoutlistUser = new std::list<UserObjectData>();
	g_User->ds6k_getUserListCurrentMac(signoutlistUser);
	int signoutindex=0;
	for (std::list<UserObjectData>::iterator iter=signoutlistUser->begin(); iter!= signoutlistUser->end(); ++iter)
	{
		UserObjectData user = *iter;

		std::string name = user.name;
		qDebug()<<QString::fromUtf8(name.c_str());

		ObId userobid = user.objectID;
		qDebug()<<userobid;

		ui->signoutname->addItem(QString::fromUtf8(name.c_str()));
		ui->signoutname->setItemData(signoutindex,user.objectID,Qt::UserRole);
		signoutindex++;
	}
	
	//2接班人
	ui->signinname->clear(); //清除列表
	signinlistUser = new std::list<UserObjectData>();
	g_User->ds6k_getUserListCurrentMac(signinlistUser);
	int signinindex=0;
	for (std::list<UserObjectData>::iterator iter=signinlistUser->begin(); iter!= signinlistUser->end(); ++iter)
	{
		UserObjectData user = *iter;
		std::string name = user.name;
		qDebug()<<QString::fromUtf8(name.c_str());
		qDebug()<<user.objectID;
		ui->signinname->addItem(QString::fromUtf8(name.c_str()));
		ui->signinname->setItemData(signinindex,user.objectID,Qt::UserRole);
		signinindex++;
	}

	//3交接班时间
	QDateTime current_date_time =QDateTime::currentDateTime();
	QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signinandoutTime_QDateEdit->setDate(date);
	ui->signinandoutTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");


	//4交接班次原始数据,从各地班工作时段配置表去读取
	ui->signinandout_shift->clear(); //清除列表
	RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS";
	int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfWorkName;i++)
	{
		std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		qDebug()<<QString::fromUtf8(workname.c_str());
		ui->signinandout_shift->addItem(QString::fromUtf8(workname.c_str()));
		ui->signinandout_shift->setItemData(i,workindex,Qt::UserRole);
	}
	CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);

	//5上班地点

	/*
	ui->workplacename->clear(); //清除列表
	RECORDSETHANDLE WorkPlaceNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkPlaceName = "select WORK_PLACE from H_WORK_HOURS";
	int rowsOfWorkPlaceName = CPS_ORM_RsLoadData(WorkPlaceNameSetHandle,sqlWorkPlaceName.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfWorkPlaceName;i++)
	{
		int workplacename = CPS_ORM_RsGetNumberValue(WorkPlaceNameSetHandle,i,0);
		ui->workplacename->addItem(QString::number(workplacename));
	}
	CPS_ORM_RsFreeRecordSet(WorkPlaceNameSetHandle);

	*/
}
 Confirm::~Confirm()

{

    delete ui;

}



 //用户点击ok，就在交接班交接记录总表添加一条交接记录
 void Confirm::on_okButton_clicked()
 {
	 
	 //千万可不敢再定义变量数据类型了，造成菊部变量
	 
	 //1获取当前日期时间
	 curDateTime=QDateTime::currentDateTime();
	 
	 //2获取用户选取的交班人
	 int signoutnameindex = ui->signoutname->currentIndex();
	 QVariant obidSignoutname = ui->signinname->itemData(signoutnameindex,Qt::UserRole);
	 ObId obidsignoutname = obidSignoutname.toULongLong();
	 signoutname = QString::number(obidsignoutname);
	 
	 


	 //3获取用户选取的接班人
	 int signinnameindex = ui->signinname->currentIndex();
	 QVariant obidSigninname = ui->signinname->itemData(signinnameindex,Qt::UserRole);
	 ObId obidsigninname = obidSigninname.toULongLong();
	 signinname = QString::number(obidsigninname);
	 


	 //4获取交接班时间
	 signinandoutTime_QDateEdit = ui->signinandoutTime_QDateEdit->dateTime();
	 
	 //5获取交接班序号
	 int signinandout_shitfindex = ui->signinandout_shift->currentIndex();
	 QVariant signinandout_shitfV = ui->signinandout_shift->itemData(signinandout_shitfindex,Qt::UserRole);
	 int signinandout_shitf_id = signinandout_shitfV.toInt();
	 signinandout_shitf = QString::number(signinandout_shitf_id);
	 
	 //6获取交接备注内容
	 signinandoutNote_QTextEdit = ui->signinandoutNote_QTextEdit->toPlainText();





	 //7获取用户选取的上班地点
	 /*
	 workplacename = ui->workplacename->currentText();
	 */

	 /*
	 RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	 //拼接交接班交接记录总表的sql语句
	 QString signinandoutsql = QString("INSERT INTO H_SIGNINANDOUT (TIME,HANDOVER,SUCCESSOR,WORK_DAY,WORK_INDEX,NOTE,WORK_PLACE) VALUES (to_timestamp('"+curDateTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss'),"\
		 +QString("%1").arg(obidsignoutname)+","\
		 +QString("%2").arg(obidsigninname)+",to_timestamp('"+signinandoutTime_QDateEdit.toString("yyyy-MM-dd")+"','yyyy-MM-dd'),"\
		 +QString("%4").arg(signinandout_shitf_id)+",'"\
		 +QString("%5").arg(signinandoutNote_QTextEdit)+"',"\
		 +QString("%6").arg(workplacename)+")");
	 //调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
	 InitConTable::executeSQL(signinandoutsql,0);
	 CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);
	 */
	 RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	 //拼接交接班交接记录总表的sql语句
	 QString signinandoutsql = QString("INSERT INTO H_SIGNINANDOUT (TIME,HANDOVER,SUCCESSOR,WORK_DAY,WORK_INDEX,NOTE) VALUES (to_timestamp('"+curDateTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss'),"\
		 +QString("%1").arg(obidsignoutname)+","\
		 +QString("%2").arg(obidsigninname)+",to_timestamp('"+signinandoutTime_QDateEdit.toString("yyyy-MM-dd")+"','yyyy-MM-dd'),"\
		 +QString("%4").arg(signinandout_shitf_id)+",'"\
		 +QString("%5").arg(signinandoutNote_QTextEdit)+"')");
	 //调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
	 InitConTable::executeSQL(signinandoutsql,0);
	 CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);
	 

	
	
		 

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


 /*
 QString Confirm::getworkplacenamedata()
 {	 
	 return workplacename;
 }
 */
 

 
 


