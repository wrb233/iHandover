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
	//��ʼ���û���ȷ�Ͻ��Ӱ�Ի�ҳ������Ľ����ˡ��Ӱ��ˡ����Ӱ�ʱ�䡢���ӱ�ע���ݣ�Ϊ�գ����ϰ�ص���Ϣ
    
	//1������
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
	
	//2�Ӱ���
	ui->signinname->clear(); //����б�
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

	//3���Ӱ�ʱ��
	QDateTime current_date_time =QDateTime::currentDateTime();
	QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signinandoutTime_QDateEdit->setDate(date);

	//4���Ӱ��ԭʼ����,�Ӹ��ذ๤��ʱ�����ñ�ȥ��ȡ
	ui->signinandout_shift->clear(); //����б�
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

	//5�ϰ�ص���Ϣ
	ui->workplacename->clear(); //����б�
	RECORDSETHANDLE WorkPlaceNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkPlaceName = "select WORK_PLACE from H_WORK_HOURS";
	int rowsOfWorkPlaceName = CPS_ORM_RsLoadData(WorkPlaceNameSetHandle,sqlWorkPlaceName.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfWorkPlaceName;i++)
	{
		int workplacename = CPS_ORM_RsGetNumberValue(WorkPlaceNameSetHandle,i,0);
		ui->workplacename->addItem(QString::number(workplacename));
	}
	CPS_ORM_RsFreeRecordSet(WorkPlaceNameSetHandle);
}
 Confirm::~Confirm()

{

    delete ui;

}


 /*
 void Confirm::insertSignInAndOutTable()
 {
	 //���������
	 //QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	 //Information information;
	 //information.exec();
 }
 */

 //�û����ok�����ڽ��Ӱཻ�Ӽ�¼�ܱ����һ�����Ӽ�¼
 void Confirm::on_okButton_clicked()
 {
	 
	 //ǧ��ɲ����ٶ���������������ˣ���ɾղ�����
	 
	 //1��ȡ��ǰ����ʱ��
	 curDateTime=QDateTime::currentDateTime();
	 
	 //2��ȡ�û�ѡȡ�Ľ�����
	 int signoutnameindex = ui->signoutname->currentIndex();
	 QVariant obidSignoutname = ui->signinname->itemData(signoutnameindex,Qt::UserRole);
	 ObId obidsignoutname = obidSignoutname.toULongLong();
	 signoutname = QString::number(obidsignoutname);
	 
	 


	 //3��ȡ�û�ѡȡ�ĽӰ���
	 int signinnameindex = ui->signinname->currentIndex();
	 QVariant obidSigninname = ui->signinname->itemData(signinnameindex,Qt::UserRole);
	 ObId obidsigninname = obidSigninname.toULongLong();
	 signinname = QString::number(obidsigninname);
	 


	 //4��ȡ���Ӱ�ʱ��
	 signinandoutTime_QDateEdit = ui->signinandoutTime_QDateEdit->dateTime();
	 
	 //5��ȡ���Ӱ����


	 int signinandout_shitfindex = ui->signinandout_shift->currentIndex();
	 QVariant signinandout_shitfV = ui->signinandout_shift->itemData(signinandout_shitfindex,Qt::UserRole);
	 int signinandout_shitf_id = signinandout_shitfV.toInt();
	 signinandout_shitf = QString::number(signinandout_shitf_id);
	 
	 //6��ȡ���ӱ�ע����
	 signinandoutNote_QTextEdit = ui->signinandoutNote_QTextEdit->toPlainText();

	 //7��ȡ�û�ѡȡ���ϰ�ص�
	 workplacename = ui->workplacename->currentText();

	 RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	 //ƴ�ӽ��Ӱཻ�Ӽ�¼�ܱ��sql���
	 QString signinandoutsql = QString("INSERT INTO H_SIGNINANDOUT (TIME,HANDOVER,SUCCESSOR,WORK_DAY,WORK_INDEX,NOTE,WORK_PLACE) VALUES (to_timestamp('"+curDateTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss'),"\
		 +QString("%1").arg(obidsignoutname)+","\
		 +QString("%2").arg(obidsigninname)+",to_timestamp('"+signinandoutTime_QDateEdit.toString("yyyy-MM-dd")+"','yyyy-MM-dd'),"\
		 +QString("%4").arg(signinandout_shitf_id)+",'"\
		 +QString("%5").arg(signinandoutNote_QTextEdit)+"',"\
		 +QString("%6").arg(workplacename)+")");
	 //������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
	 InitConTable::executeSQL(signinandoutsql,0);
	 CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);
	 //MainWindow::showSignInAndOut();
	 
	 

	
	
		 

 }


 //Ϊÿһ������дһ��get����
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

 

 
 


