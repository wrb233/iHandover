#include "signin.h"
#include "ui_signin.h"
#include <QMessageBox>


#include "signout.h"
#include "information.h"
#include "ui_information.h"





SignIn::SignIn(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::SignIn)
{
    

	 ui->setupUi(this);

    //setWindowTitle("signout");
	
	 connect(ui->okButton,SIGNAL(clicked()),this, SLOT(showInformationDialog()));

	 ui->signinname->clear(); //清除列表
	 for (int i=0;i<20;i++)
		 ui->signinname->addItem(QString::number(i)); //不带图标



	 ui->signin_shift->clear(); //清除列表
	 for (int i=0;i<3;i++)
		 ui->signin_shift->addItem(QString::number(i)); //不带图标
    
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
	 signin_shitf = ui->signin_shift->currentText();

	

	 







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