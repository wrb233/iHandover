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

	 ui->signinname->clear(); //����б�
	 for (int i=0;i<20;i++)
		 ui->signinname->addItem(QString::number(i)); //����ͼ��



	 ui->signin_shift->clear(); //����б�
	 for (int i=0;i<3;i++)
		 ui->signin_shift->addItem(QString::number(i)); //����ͼ��
    
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