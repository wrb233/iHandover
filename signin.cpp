#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>

#include "configuration.h"
#include "ui_configuration.h"


#include "signout.h"
#include "signininformation.h"
#include "ui_signininformation.h"

#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <list>
extern UserMSInterface *g_User;


extern DBPOOLHANDLE dbPoolHandle;

extern  Database *database;

SignIn::SignIn(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::SignIn)
{
    

	 ui->setupUi(this);

    //setWindowTitle("signout");
	
	 //connect(ui->okButton,SIGNAL(clicked()),this, SLOT(showInformationDialog()));

	 ui->signinname->clear(); //����б�

	 signinlistUser = new std::list<UserObjectData>();
	 g_User->ds6k_getUserListCurrentMac(signinlistUser);


	 int signinindex=0;
	 for (std::list<UserObjectData>::iterator iter=signinlistUser->begin(); iter!= signinlistUser->end(); ++iter)
	 {
		 UserObjectData user = *iter;

		 std::string name = user.name;
		 qDebug()<<QString::fromUtf8(name.c_str());

		 ObId userobid = user.objectID;
		 qDebug()<<userobid;

		 ui->signinname->addItem(QString::fromUtf8(name.c_str()));
		 ui->signinname->setItemData(signinindex,user.objectID,Qt::UserRole);
		 signinindex++;
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
	 ui->signinTime_QDateEdit->setDate(date);
	 ui->signinTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");



	 //4���Ӱ��ԭʼ����,�Ӹ��ذ๤��ʱ�����ñ�ȥ��ȡ
	 ui->signin_shift->clear(); //����б�
	 RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS order by work_index";
	 int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	 for (int i=0;i<rowsOfWorkName;i++)
	 {
		 std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		 int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		 qDebug()<<QString::fromUtf8(workname.c_str());
		 ui->signin_shift->addItem(QString::fromUtf8(workname.c_str()));
		 ui->signin_shift->setItemData(i,workindex,Qt::UserRole);


	 }
	 CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);
	 //Ĭ�Ͻ�����
	 Configuration configuration;
	 int intDefaultIndex = configuration.whichShift();
	 ui->signin_shift->setCurrentIndex(intDefaultIndex);

    
}
 SignIn::~SignIn()

{

    delete ui;

}
 /*
 void SignIn::showInformationDialog()
 {


	 //���������
	 //QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	 Information information;

	 information.exec();


 }
 */

 void SignIn::on_okButton_clicked()
 {

	 //ǧ��ɲ����ٶ���������������ˣ���ɾղ�����

	 //1��ȡ��ǰ����ʱ��
	 signincurDateTime=QDateTime::currentDateTime();

	 //2��ȡ�û�ѡȡ�ĽӰ���
	 signinname = ui->signinname->currentText();

	 

	 //3��ȡ�Ӱ�ʱ��
	 signinTime_QDateEdit = ui->signinTime_QDateEdit->dateTime();

	 //4��ȡ���Ӱ����
	 //signin_shitf = ui->signin_shift->currentText();


	 int signin_shitfindex = ui->signin_shift->currentIndex();
	 QVariant signin_shitfV = ui->signin_shift->itemData(signin_shitfindex,Qt::UserRole);
	 int signin_shitf_id = signin_shitfV.toInt();
	 signin_shitf = QString::number(signin_shitf_id);

	

	 







 }

 QDateTime SignIn::getcurDateTimedata()
 {	 
	 return signincurDateTime;
 }
 QString SignIn::getsigninnamedata()
 {	 
	 return signinname;
 }

 QDateTime SignIn::getsigninTime_QDateEditdata()
 {
	 return signinTime_QDateEdit;
 }
 QString SignIn::getsignin_shitfdata()
 {	 
	 return signin_shitf;
 }