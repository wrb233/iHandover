#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>


#include "signout.h"
#include "information.h"
#include "ui_information.h"

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

	 ui->signinname->clear(); //清除列表

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



	 ui->signin_shift->clear();  //清除列表
	 RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS";
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


	 QDateTime current_date_time =QDateTime::currentDateTime();
	 QString current_date =current_date_time.toString("yyyyMMdd");
	 QDate date = QDate::fromString(current_date,"yyyyMMdd");
	 ui->signinTime_QDateEdit->setDate(date);
    
}
 SignIn::~SignIn()

{

    delete ui;

}
 /*
 void SignIn::showInformationDialog()
 {


	 //测试万金油
	 //QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	 Information information;

	 information.exec();


 }
 */

 void SignIn::on_okButton_clicked()
 {

	 //千万可不敢再定义变量数据类型了，造成菊部变量

	 //1获取当前日期时间
	 signincurDateTime=QDateTime::currentDateTime();

	 //2获取用户选取的接班人
	 signinname = ui->signinname->currentText();

	 

	 //3获取接班时间
	 signinTime_QDateEdit = ui->signinTime_QDateEdit->dateTime();

	 //4获取交接班序号
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