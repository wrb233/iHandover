#include "signout.h"
#include "ui_signout.h"
#include <QMessageBox>


#include "signin.h"
#include "information.h"
#include "ui_information.h"





SignOut::SignOut(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::SignOut)
{
    

	 ui->setupUi(this);

    //setWindowTitle("signout");




	
	//connect(ui->okButton,SIGNAL(clicked()),this, SLOT(showInformationDialog()));

	ui->signoutname->clear(); //清除列表
	for (int i=0;i<20;i++)
		ui->signoutname->addItem(QString::number(i)); //不带图标

	

	ui->signout_shift->clear(); //清除列表
	for (int i=0;i<3;i++)
		ui->signout_shift->addItem(QString::number(i)); //不带图标


    
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
		 signout_shitf = ui->signout_shift->currentText();

		 


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


