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
#include "ToolUtil.h"

#include <QCoreApplication>


extern DBPOOLHANDLE dbPoolHandle;
extern  Database *database;

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
    SignInAndOut_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("time")));
    SignInAndOut_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("handover")));
    SignInAndOut_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("successor")));
    SignInAndOut_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_day")));
    SignInAndOut_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_index")));
	SignInAndOut_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("note")));
	SignInAndOut_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("work_place")));	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->SignInAndOut_tableview->setModel(SignInAndOut_model);
	//QTableView平均分配列宽
	ui->SignInAndOut_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
    //ui->DPCPonit_tableview->verticalHeader()->hide(); 	
	RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	//QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT order by TIME";

	QString sqlSignInAndOut = "select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE, a.WORK_PLACE from H_SIGNINANDOUT a,H_WORK_HOURS b where a.WORK_INDEX=b.WORK_INDEX order by a.TIME";
	
		int rowsOfTer = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
		AT_Name = database->matchAType("Name");
		for (int i=0;i<rowsOfTer;i++)
		{
			ORMTimeStamp signinandouttime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 0);
			QDateTime strsigninandouttime = SetTimeFromDB(signinandouttime);
			QStandardItem* item1 = new QStandardItem(strsigninandouttime.toString("yyyy-MM-dd hh:mm:ss"));
			
			ObId handoverobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,1);
			StringData handoverobiddata;
			ToolUtil::databaseRead(handoverobid, AT_Name, &handoverobiddata);
			OMString handoverstr = (OMString)handoverobiddata;
			QString qhandoverstr = QString::fromUtf8(handoverstr.c_str());
			QStandardItem* item2 = new QStandardItem(qhandoverstr);

			ObId successorobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,2);
			StringData successorobiddata;
			ToolUtil::databaseRead(successorobid, AT_Name, &successorobiddata);
			OMString successorstr = (OMString)successorobiddata;
			QString qsuccessorstr = QString::fromUtf8(successorstr.c_str());
			QStandardItem* item3 = new QStandardItem(qsuccessorstr);

			

			ORMTimeStamp workdaytime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 3);
			QDateTime strworkdaytime = SetTimeFromDB(workdaytime);
			QStandardItem* item4 = new QStandardItem(strworkdaytime.toString("yyyy-MM-dd"));

			//ui->SignInAndOut_tableview->setIndexWidget(SignInAndOut_model->index(0,5),new QPushButton("dayin"));
			//ui->SignInAndOut_tableview->setIndexWidget(SignInAndOut_model->index(0,6),new QPushButton("detail"));
			
			
			
			
			//非obid怎么办？？？
			//int workindex = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,4);
			//QStandardItem* item5 = new QStandardItem(QString::number(workindex));
			
			std::string workname = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,4);
			QStandardItem* item5 = new QStandardItem(QString::fromUtf8(workname.c_str()));
			
			
			
			

			
			
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
		//1获取点击交班时的当前日期时间,右时间
		QDateTime signoutcurDateTime=signout.getcurDateTimedata();
		//2获取用户选取的交班人
		QString signoutname = signout.getsignoutnamedata();
		//3获取交班时间,左时间
		QDateTime signoutTime_QDateEdit = signout.getsignoutTime_QDateEditdata();
		//4获取交班序号
		QString signout_shitf = signout.getsignout_shitfdata();

		Information information;

		
		//获取用户选择的交班时间段，右时间
		QDateTime endtime = information.getSignPageTime(signoutcurDateTime);
		QString strendtime = endtime.toString("yyyy-MM-dd hh:mm:ss");
		//qDebug()<<endtime;

		//获取用户选择的交班时间段，左时间
		QDateTime startdate = information.getSignPageDate(signoutTime_QDateEdit);
		QString strstartdate = startdate.toString("yyyy-MM-dd");  
		
		QString startshift = information.getSignPageShift(signout_shitf);
		

		//根据date和shift拼接字符串左时间时间点
		QString strstarttime;//初始化左时间
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
		QString strhour = QString::number(work_start);
		if (strhour.length()==1)
			strstarttime = QString("%1 0%2:00:00")
					               .arg(strstartdate)
								   .arg(strhour);
		else
			strstarttime = QString("%1 %2:00:00")
				                   .arg(strstartdate)
				                   .arg(strhour);
		//qDebug()<<work_start;
		//QString strstarttime = strstartdate+' '+strhourminsec;
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);

		//判断新表里面数据条数
		RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlisNull = "select COUNT(*) from H_FA_ACCIDENTINFO";
		int rowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
		int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
		CPS_ORM_RsFreeRecordSet(isNullSetHandle);

		//先从老表里面展示已有数据
		if (items == 0)
			information.firstShowTableViewsFromOldTables(strstarttime, strendtime ,startshift);
		//新表里面已经有数据了，先把新表数据展示，然后再查老表
		else
			information.secondShowTableViewsFromOldTables(strstarttime, strendtime ,startshift);


		



		//用户编辑完点确认，进行入库操作，其中录入时间是主键
		if (information.exec()==QDialog::Accepted)
		{


			//获取点击信息页面确认的当前日期时间,录入时间（非常重要）
			QDateTime informationcurDateTime=information.getcurDateTimedata();
			qDebug()<<informationcurDateTime;



			if (items == 0)
				information.firstwriteTables(strstarttime, strendtime, startshift, informationcurDateTime);

			else
				information.secondwriteTables(strstarttime, strendtime, startshift, informationcurDateTime);





		}

		

		
	}
	
}

void MainWindow::showSignInDialog()
{


	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	SignIn signin;

	if (signin.exec()==QDialog::Accepted)
	{

		/*
		//1获取点击接班时的当前日期时间
		QDateTime signincurDateTime=signin.getcurDateTimedata();

		//2获取用户选取的接班人
		QString signinname = signin.getsigninnamedata();


		//3获取接班时间
		QDateTime signinTime_QDateEdit = signin.getsigninTime_QDateEditdata();

		//4获取接班序号
		short signin_shitf = signin.getsignin_shitfdata();



		


		//information.firstShowTableViewsFromOldTables(signincurDateTime, signinTime_QDateEdit, signin_shitf);
		
		//获取用户选择的接班时间段
		QDateTime endtime = signincurDateTime;
		//qDebug()<<endtime;
		QDateTime startdate = signinTime_QDateEdit;
		QString strstartdate = startdate.toString("yyyy-MM-dd");  
		QString startshift = signin_shitf;

		/*
		QString strBuffer;
		if (startshift == "1")
		{
			strBuffer = " 06:00:00";  	
		}
		else if (startshift == "2")
		{
			strBuffer = " 14:00:00";  
		}
		else if (startshift == "3")
		{
			strBuffer = " 22:00:00";  
		}
		QString strstarttime = strstartdate.append(strBuffer);
		QDateTime starttime = QDateTime::fromString(strstarttime, "yyyy-MM-dd hh:mm:ss");

		*/
		Information information;

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
	//QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT \
							  where TIME between to_date('"+startTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1";



	QString sqlSignInAndOut="select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE, a.WORK_PLACE from H_SIGNINANDOUT a,H_WORK_HOURS b \
							where a.WORK_INDEX=b.WORK_INDEX and a.TIME between to_date('"+startTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1 order by a.TIME";

	int rowsOfTer = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{

		ORMTimeStamp signinandouttime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 0);
		QDateTime strsigninandouttime = SetTimeFromDB(signinandouttime);
		QStandardItem* item1 = new QStandardItem(strsigninandouttime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
		
		ObId handoverobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,1);
		StringData handoverobiddata;
		ToolUtil::databaseRead(handoverobid, AT_Name, &handoverobiddata);
		OMString handoverstr = (OMString)handoverobiddata;
		QString qhandoverstr = QString::fromUtf8(handoverstr.c_str());
		QStandardItem* item2 = new QStandardItem(qhandoverstr);

		ObId successorobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,2);
		StringData successorobiddata;
		ToolUtil::databaseRead(successorobid, AT_Name, &successorobiddata);
		OMString successorstr = (OMString)successorobiddata;
		QString qsuccessorstr = QString::fromUtf8(successorstr.c_str());
		QStandardItem* item3 = new QStandardItem(qsuccessorstr);



		ORMTimeStamp workdaytime = CPS_ORM_RsGetTimeValue(SignInAndOutSetHandle, i, 3);
		QDateTime strworkdaytime = SetTimeFromDB(workdaytime);
		QStandardItem* item4 = new QStandardItem(strworkdaytime.toString("yyyy-MM-dd"));





		//非obid怎么办？？？
		//int workindex = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,4);
		//QStandardItem* item5 = new QStandardItem(QString::number(workindex));

		std::string workname = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,4);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(workname.c_str()));







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
