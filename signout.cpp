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

	ui->signoutname->clear(); //����б�


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
	

	
	//����H_WORK_HOURSȫ�����ҵ���ʼʱ��

	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//��ǰʱ����Ű�ʱ�����ʱ���
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	QDateTime current_date_time =QDateTime::currentDateTime();
	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//��ǰ������Сʱ
	QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	QString current_date = current_date_timeminus.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signoutTime_QDateEdit->setDate(date);
	ui->signoutTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");



	//4���Ӱ��ԭʼ����,�Ӹ��ذ๤��ʱ�����ñ�ȥ��ȡ
	ui->signout_shift->clear(); //����б�
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
	//Ĭ�Ͻ�����
	Configuration configuration;
	int intDefaultIndex = configuration.whichShift();
	ui->signout_shift->setCurrentIndex(intDefaultIndex);


	/*
	ui->signout_shift->clear(); //����б�
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
	//����H_WORK_HOURSȫ�����ҵ���ʼʱ��
	QString strstarttime;//��ʼ����ʱ��
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=1";
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


	

	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//��ǰ��9Сʱ
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
	 //���޼ƻ���H_FA_ACCIDENTINFO
     //�¹���Ϣ��H_MAINTENANCE_PLAN
	 //ң�ؼ�¼��H_DPC_OPTRECORD
	 //��·���ر�H_FEEDER_OVERLOAD

		 //ǧ��ɲ����ٶ���������������ˣ���ɾղ�����
		 //1��ȡ��ǰ����ʱ��
		 signoutcurDateTime=QDateTime::currentDateTime();

		 
		 
		 
		 
		 //2��ȡ�û�ѡȡ�Ľ�����
		 signoutname = ui->signoutname->currentText();

		 
		
		 
		 
		 //3��ȡ����ʱ��
		 signoutTime_QDateEdit = ui->signoutTime_QDateEdit->dateTime();

		 
		 
		 
		 
		 
		 //4��ȡ�������
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


