#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initializtion.h"
#include <QStandardItemModel>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "signout.h"
#include "signin.h"
#include "information.h"
#include "confirm.h"

extern DBPOOLHANDLE dbPoolHandle;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化查询起止时间为当前时间
	QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd");
	QDate date = QDate::fromString(current_date,"yyyyMMdd");
	ui->startTime->setDate(date);
	ui->endTime->setDate(date);
	
	
	//点击交班动作触发交班页面
	connect(ui->actionSignOut,SIGNAL(triggered()),this, SLOT(showSignOutDialog()));
	//点击接班动作触发接班页面
	connect(ui->actionSignIn,SIGNAL(triggered()),this, SLOT(showSignInDialog()));
	//点击确认动作触发交接班确认页面
	connect(ui->actionConfirm,SIGNAL(triggered()),this, SLOT(showConfirmDialog()));
	//点击查询按钮按时间间隔查询交接班交接记录总表
	connect(ui->queryAtTime,SIGNAL(clicked()),this, SLOT(queryAtTime()));
	//一进入界面先展示空的交接班交接记录总表
	showSignInAndOut();
	
}

MainWindow::~MainWindow()
{
    delete ui;
}




//显示交接班交接记录总表的所有字段
void MainWindow::showSignInAndOut()
{
	
	
	
	QStandardItemModel *SignInAndOut_model = new QStandardItemModel();
    SignInAndOut_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("交接确认时间")));
    SignInAndOut_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("交班人")));
    SignInAndOut_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("接班人")));
    SignInAndOut_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("交接日")));
    SignInAndOut_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("交接班序号")));
	SignInAndOut_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("交接备注内容")));
	SignInAndOut_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("上班地点")));	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->SignInAndOut_tableview->setModel(SignInAndOut_model);
	//QTableView平均分配列宽
	ui->SignInAndOut_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
    //ui->DPCPonit_tableview->verticalHeader()->hide(); 	
	RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT";

	
		int rowsOfTer = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
		for (int i=0;i<rowsOfTer;i++)
		{
			ORMTimeStamp signinandouttime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 0);
			QDateTime strsigninandouttime = SetTimeFromDB(signinandouttime);
			QStandardItem* item1 = new QStandardItem(strsigninandouttime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
			
			ObId handoverobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,1);
			QStandardItem* item2 = new QStandardItem(QString::number(handoverobid));

			ObId successorobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,2);
			QStandardItem* item3 = new QStandardItem(QString::number(successorobid));

			ORMTimeStamp workdaytime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 3);
			QDateTime strworkdaytime = SetTimeFromDB(workdaytime);
			QStandardItem* item4 = new QStandardItem(strworkdaytime.toString("yyyy-MM-dd"));

			//非obid怎么办？？？
			int workindex = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,4);
			QStandardItem* item5 = new QStandardItem(QString::number(workindex));

			std::string note = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,5);
			QStandardItem* item6 = new QStandardItem(QString::fromUtf8(note.c_str()));
			//QStandardItem* item6 = new QStandardItem(QString::fromStdString(note));
			//QStandardItem* item6 = new QStandardItem(QObject::tr(note));

			ObId workplaceobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,6);
			QStandardItem* item7 = new QStandardItem(QString::number(workplaceobid));
			
			QList<QStandardItem*> item;
            item << item1 << item2 << item3 << item4 << item5 << item6 << item7;
            SignInAndOut_model->appendRow(item);
	
		}
		CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);





}




void MainWindow::showSignOutDialog()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	SignOut signout;
	
	if (signout.exec()==QDialog::Accepted)
	{
		QDateTime signoutcurDateTime=signout.getcurDateTimedata();

		//2获取用户选取的交班人
		QString signoutname = signout.getsignoutnamedata();


		//3获取交班时间
		QDateTime signoutTime_QDateEdit = signout.getsignoutTime_QDateEditdata();

		//4获取交班序号
		QString signout_shitf = signout.getsignout_shitfdata();



		Information information;


		information.firstShowTableViewsFromOldTables(signoutTime_QDateEdit,signout_shitf);
		
		


		
		information.exec();
	}
	
}

void MainWindow::showSignInDialog()
{


	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	SignIn signin;

	if (signin.exec()==QDialog::Accepted)
	{
		QDateTime signincurDateTime=signin.getcurDateTimedata();

		//2获取用户选取的交班人
		QString signinname = signin.getsigninnamedata();


		//3获取交班时间
		QDateTime signinTime_QDateEdit = signin.getsigninTime_QDateEditdata();

		//4获取交班序号
		QString signin_shitf = signin.getsignin_shitfdata();



		Information information;


		information.firstShowTableViewsFromOldTables(signinTime_QDateEdit,signin_shitf);





		information.exec();
	}


}

void MainWindow::showConfirmDialog()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	Confirm confirm;

	//confirm.show();
	if (confirm.exec()==QDialog::Accepted)
	{
		
		/*
		qDebug()<<confirm.getcurDateTimedata();
		qDebug()<<confirm.getsignoutnamedata();
		qDebug()<<confirm.getsigninnamedata();
		qDebug()<<confirm.getsigninandoutTime_QDateEditdata();
		qDebug()<<confirm.getsigninandout_shitfdata();
		qDebug()<<confirm.getsigninandoutNote_QTextEditdata();
		qDebug()<<confirm.getworkplacenamedata();
		//ui->SignInAndOut_tableview.

		*/
		//这里直接调用展示总表的方法，因为数据库总表已经有新的一行数据了，所以我们重新遍历一遍总表
		showSignInAndOut();
	}
	/*
	qDebug()<<"SSSSSSSSSSSSS";
	qDebug()<<confirm.getcurDateTimedata();
	qDebug()<<"SSSSSSSSSSSSS";
	*/
}



void MainWindow::queryAtTime()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	QStandardItemModel *SignInAndOut_model = new QStandardItemModel();
	SignInAndOut_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("交接确认时间")));
	SignInAndOut_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("交班人")));
	SignInAndOut_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("接班人")));
	SignInAndOut_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("交接日")));
	SignInAndOut_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("交接班序号")));
	SignInAndOut_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("交接备注内容")));
	SignInAndOut_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("上班地点")));	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->SignInAndOut_tableview->setModel(SignInAndOut_model);
	//QTableView平均分配列宽
	ui->SignInAndOut_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);


	QDateTime startTime = ui->startTime->dateTime();
	QDateTime endTime = ui->endTime->dateTime();
	
	RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT \
							  where TIME between to_date('"+startTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1";


	int rowsOfTer = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		ORMTimeStamp signinandouttime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 0);
		QDateTime strsigninandouttime = SetTimeFromDB(signinandouttime);
		QStandardItem* item1 = new QStandardItem(strsigninandouttime.toString("yyyy-MM-dd hh:mm:ss.zzz"));

		ObId handoverobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::number(handoverobid));

		ObId successorobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,2);
		QStandardItem* item3 = new QStandardItem(QString::number(successorobid));

		ORMTimeStamp workdaytime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 3);
		QDateTime strworkdaytime = SetTimeFromDB(workdaytime);
		QStandardItem* item4 = new QStandardItem(strworkdaytime.toString("yyyy-MM-dd"));

		//非obid怎么办？？？
		int workindex = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,4);
		QStandardItem* item5 = new QStandardItem(QString::number(workindex));

		std::string note = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,5);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(note.c_str()));
		//QStandardItem* item6 = new QStandardItem(QString::fromStdString(note));
		//QStandardItem* item6 = new QStandardItem(QObject::tr(note));

		ObId workplaceobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,6);
		QStandardItem* item7 = new QStandardItem(QString::number(workplaceobid));

		QList<QStandardItem*> item;
		item << item1 << item2 << item3 << item4 << item5 << item6 << item7;
		SignInAndOut_model->appendRow(item);

	}
	CPS_ORM_RsFreeRecordSet(SignInAndOutSetHandle);
	
	
}





QDateTime SetTimeFromDB(ORMTimeStamp time)
{
	//int mSec = time.m_ulFraction/1000000;
	QString recordtime = QString("%1").arg(time.m_nYear)+"-"+QString("%1").arg(time.m_nMonth,2,10,QChar('0'))
		+"-"+QString("%1").arg(time.m_nDay,2,10,QChar('0'))+" "+QString("%1").arg(time.m_nHour,2,10,QChar('0'))
		+":"+QString("%1").arg(time.m_nMinute,2,10,QChar('0'))+":"+QString("%1").arg(time.m_nSecond,2,10,QChar('0'))
		+"."+QString("%1").arg(time.m_ulFraction,9,10,QChar('0')).mid(0,3);//
	QDateTime RecordTime =  QDateTime::fromString(recordtime,"yyyy-MM-dd hh:mm:ss.zzz");
	//qDebug()<<"SetTimeFromDB(): "<<RecordTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
	return RecordTime;
}
