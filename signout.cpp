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

	ui->signoutname->clear(); //����б�
	for (int i=0;i<20;i++)
		ui->signoutname->addItem(QString::number(i)); //����ͼ��

	

	ui->signout_shift->clear(); //����б�
	for (int i=0;i<3;i++)
		ui->signout_shift->addItem(QString::number(i)); //����ͼ��


    
}
 SignOut::~SignOut()

{

    delete ui;

}


 /*
 void SignOut::showInformationDialog()
{
    
	
	//���������
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	//qDebug()<<1;

	
}
*/


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


