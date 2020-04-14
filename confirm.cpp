#include "confirm.h"
#include "ui_confirm.h"
#include <QMessageBox>


#include "signin.h"
#include "signout.h"
#include "information.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <QString>

#include <QDebug>

#include <QList>

extern DBPOOLHANDLE dbPoolHandle;


Confirm::Confirm(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Confirm)
{
    

	ui->setupUi(this);

    
	
	//�û����ok�����������Ϣ�嵽���Ӱཻ�Ӽ�¼�ܱ�
	//connect(ui->okButton,SIGNAL(clicked()),this, SLOT(insertSignInAndOutTable()));


	//��ʼ���û���ȷ�Ͻ��Ӱ�Ի�ҳ������Ľ����ˡ��Ӱ��ˡ����Ӱ�ʱ�䡢���ӱ�ע���ݣ�Ϊ�գ����ϰ�ص���Ϣ
    
	//������
	ui->signoutname->clear(); //����б�
	for (int i=0;i<20;i++)
		ui->signoutname->addItem(QString::number(i)); //����ͼ��
	//�Ӱ���
	ui->signinname->clear(); //����б�
	for (int i=0;i<20;i++)
		ui->signinname->addItem(QString::number(i)); //����ͼ��
	//���Ӱ��
	ui->signinandout_shift->clear(); //����б�
	for (int i=0;i<3;i++)
		ui->signinandout_shift->addItem(QString::number(i)); //����ͼ��
	//�ϰ�ص���Ϣ
	ui->workplacename->clear(); //����б�
	for (int i=100;i<106;i++)
		ui->workplacename->addItem(QString::number(i)); //����ͼ��

	//���Ӱ�ʱ��
	QDateTime current_date_time =QDateTime::currentDateTime();
	QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->signinandoutTime_QDateEdit->setDate(date);

	
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
	 signoutname = ui->signoutname->currentText();

	 //3��ȡ�û�ѡȡ�ĽӰ���
	 signinname = ui->signinname->currentText();

	 //4��ȡ���Ӱ�ʱ��
	 signinandoutTime_QDateEdit = ui->signinandoutTime_QDateEdit->dateTime();
	 
	 //5��ȡ���Ӱ����
	 signinandout_shitf = ui->signinandout_shift->currentText();

	 //6��ȡ���ӱ�ע����
	 signinandoutNote_QTextEdit = ui->signinandoutNote_QTextEdit->toPlainText();

	 //7��ȡ�û�ѡȡ���ϰ�ص�
	 workplacename = ui->workplacename->currentText();

	 RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	 //ƴ�ӽ��Ӱཻ�Ӽ�¼�ܱ��sql���
	 QString signinandoutsql = QString("INSERT INTO H_SIGNINANDOUT (TIME,HANDOVER,SUCCESSOR,WORK_DAY,WORK_INDEX,NOTE,WORK_PLACE) VALUES (to_timestamp('"+curDateTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss'),"\
		 +QString("%1").arg(signoutname)+","\
		 +QString("%2").arg(signinname)+",to_timestamp('"+signinandoutTime_QDateEdit.toString("yyyy-MM-dd")+"','yyyy-MM-dd'),"\
		 +QString("%4").arg(signinandout_shitf)+",'"\
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

