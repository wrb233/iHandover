#include "signout.h"
#include "ui_signout.h"
#include <QMessageBox>

#include "configuration.h"
#include "ui_configuration.h"

#include "signin.h"
#include "signoutinformation.h"
#include "ui_signoutinformation.h"

#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <list>
extern UserMSInterface *g_User;

extern DBPOOLHANDLE dbPoolHandle;
extern  Database *database;

SignOut::SignOut(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::SignOut)
{
    

	 ui->setupUi(this);

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
	

	
	//根据H_WORK_HOURS全天班查找到起始时间

	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//当前时间和排班时间存在时间差
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	QDateTime current_date_time =QDateTime::currentDateTime();
	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//往前推若干小时
	QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	QString current_date = current_date_timeminus.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signoutTime_QDateEdit->setDate(date);
	ui->signoutTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");



	//4交接班次原始数据,从各地班工作时段配置表去读取
	ui->signout_shift->clear(); //清除列表
	RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS order by work_index";
	int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfWorkName;i++)
	{
		std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		qDebug()<<QString::fromUtf8(workname.c_str());
		ui->signout_shift->addItem(QString::fromUtf8(workname.c_str()));
		ui->signout_shift->setItemData(i,workindex,Qt::UserRole);


	}
	CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);
	//默认交班班次
	Configuration configuration;
	int intDefaultIndex = configuration.whichShift();
	ui->signout_shift->setCurrentIndex(intDefaultIndex);


	/*
	ui->signout_shift->clear(); //清除列表
	RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS order by work_index";
	int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfWorkName;i++)
	{
		std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		qDebug()<<QString::fromUtf8(workname.c_str());
		ui->signout_shift->addItem(QString::fromUtf8(workname.c_str()));
		ui->signout_shift->setItemData(i,workindex,Qt::UserRole);
	}
	CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);
	


	
	QDateTime current_date_time =QDateTime::currentDateTime();
	//根据H_WORK_HOURS全天班查找到起始时间
	QString strstarttime;//初始化左时间
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=1";
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


	

	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//往前推9小时
	QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	QString current_date = current_date_timeminus.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signoutTime_QDateEdit->setReadOnly(true);
	ui->signoutTime_QDateEdit->setDate(date);
	ui->signoutTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");

	*/

    
	

    
}
 SignOut::~SignOut()

{

    delete ui;

}



 void SignOut::on_okButton_clicked()
 {
	 //检修计划表H_FA_ACCIDENTINFO
     //事故信息表H_MAINTENANCE_PLAN
	 //遥控记录表H_DPC_OPTRECORD
	 //线路重载表H_FEEDER_OVERLOAD

		 //千万可不敢再定义变量数据类型了，造成菊部变量
		 //1获取当前日期时间
		 signoutcurDateTime=QDateTime::currentDateTime();

		 
		 
		 
		 
		 //2获取用户选取的交班人
		 signoutname = ui->signoutname->currentText();

		 
		
		 
		 
		 //3获取交班时间
		 signoutTime_QDateEdit = ui->signoutTime_QDateEdit->dateTime();

		 
		 
		 
		 
		 
		 //4获取交班序号
		 //signout_shitf = ui->signout_shift->currentText();
		 /*
		 int signinandout_shitfindex = ui->signinandout_shift->currentIndex();
		 QVariant signinandout_shitfV = ui->signinandout_shift->itemData(signinandout_shitfindex,Qt::UserRole);
		 int signinandout_shitf = signinandout_shitfV.toInt();
		 QString signinandout_shitf_id = QString::number(signinandout_shitf);
		 */


		 

		 int signout_shitfindex = ui->signout_shift->currentIndex();
		 QVariant signout_shitfV = ui->signout_shift->itemData(signout_shitfindex,Qt::UserRole);
		 int signout_shitf_id = signout_shitfV.toInt();
		 signout_shitf = QString::number(signout_shitf_id);


 }

 QDateTime SignOut::getcurDateTimedata()
 {	 
	 return signoutcurDateTime;
 }
 QString SignOut::getsignoutnamedata()
 {	 
	 return signoutname;
 }
 
 QDateTime SignOut::getsignoutTime_QDateEditdata()
 {
	 return signoutTime_QDateEdit;
 }
QString SignOut::getsignout_shitfdata()
 {	 
	 return signout_shitf;
 }


