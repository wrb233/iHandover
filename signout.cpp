#include "signout.h"
#include "ui_signout.h"
#include <QMessageBox>


#include "signin.h"
#include "information.h"
#include "ui_information.h"

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

    //setWindowTitle("signout");




	
	//connect(ui->okButton,SIGNAL(clicked()),this, SLOT(showInformationDialog()));




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
	

	
	

	ui->signout_shift->clear(); //清除列表
	RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS";
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
	QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signoutTime_QDateEdit->setDate(date);

    
}
 SignOut::~SignOut()

{

    delete ui;

}


 /*
 void SignOut::showInformationDialog()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	//qDebug()<<1;

	
}
*/


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

		 


		 /*
		 emit sendsignoutcurDateTimeData(signoutcurDateTime);

		 emit sendsignoutnameData(signoutname);

		 emit sendsignoutTime_QDateEditData(signoutTime_QDateEdit);

		 emit sendsignout_shiftData(signout_shitf);
		 */








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


