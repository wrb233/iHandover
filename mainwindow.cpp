#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initializtion.h"
#include <QStandardItemModel>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include "signout.h"
#include "signin.h"
#include "signininformation.h"
#include "signoutinformation.h"
#include "confirm.h"
#include "ToolUtil.h"

#include "print.h"
#include "configuration.h"

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
	ui->startTime->setDisplayFormat("yyyy/MM/dd");
	ui->endTime->setDate(date);
	ui->endTime->setDisplayFormat("yyyy/MM/dd");
	
	
	ui->SignInAndOut_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->SignInAndOut_tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);

	//双击实现查询
	connect(ui->SignInAndOut_tableview,SIGNAL(doubleClicked  (const QModelIndex &)),this,SLOT(slotDoubleClickTreeNode(const QModelIndex &)));

	

	//点击交班动作触发交班页面
	connect(ui->actionSignOut,SIGNAL(triggered()),this, SLOT(showSignOutDialog()));
	//点击接班动作触发接班页面
	connect(ui->actionSignIn,SIGNAL(triggered()),this, SLOT(showSignInDialog()));
	//点击确认动作触发交接班确认页面
	connect(ui->actionConfirm,SIGNAL(triggered()),this, SLOT(showConfirmDialog()));

	//点击打印动作触发打印预览页面
	connect(ui->actionPrint,SIGNAL(triggered()),this, SLOT(showPrintDialog()));


	//点击配置动作触发确认配置页面
	connect(ui->actionConfiguration,SIGNAL(triggered()),this, SLOT(showConfigurationForm()));





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
	SignInAndOut_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("signinandoutnote")));
	//SignInAndOut_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("work_place")));	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->SignInAndOut_tableview->setModel(SignInAndOut_model);
	//QTableView平均分配列宽
	ui->SignInAndOut_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
    //ui->DPCPonit_tableview->verticalHeader()->hide(); 	
	RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	//QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT order by TIME";

	//QString sqlSignInAndOut = "select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE, a.WORK_PLACE from H_SIGNINANDOUT a,H_WORK_HOURS b where a.WORK_INDEX=b.WORK_INDEX order by a.TIME";
	QString sqlSignInAndOut = "select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE from H_SIGNINANDOUT a,H_WORK_HOURS b where a.WORK_INDEX=b.WORK_INDEX order by a.TIME";

		int rowsOfTersqlSignInAndOut = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
		AT_Name = database->matchAType("Name");
		for (int i=0;i<rowsOfTersqlSignInAndOut;i++)
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


			/*
			ObId workplaceobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,6);
			QStandardItem* item7 = new QStandardItem(QString::number(workplaceobid));
			*/


			QList<QStandardItem*> item;
            item << item1 << item2 << item3 << item4 << item5 << item6;
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
		//这里用户每次交班查询都会先查看用户所选班次内新表里面有没有对应的数据，如果没有，那么就从老表去拿（左右时间就固定在所选班次对应的左右时间），如果有就追加（左时间为当前对应班次已有的时间+1s，右时间为所选班次对应的右时间）
		
		//1获取点击交班时的当前日期时间
		QDateTime signoutcurDateTime=signout.getcurDateTimedata();
		//2获取用户选取的交班人
		QString signoutname = signout.getsignoutnamedata();
		//3获取交班时间
		QDateTime signoutTime_QDateEdit = signout.getsignoutTime_QDateEditdata();
		//4获取交班序号
		QString signout_shitf = signout.getsignout_shitfdata();

		SignOutInformation signoutinformation;

		//根据交班序号可以取到左右时间，特殊情况为跨天


	

		//获取用户选择的交班时间段，左时间和右时间
		QDateTime startdate = signoutinformation.getSignPageDate(signoutTime_QDateEdit);

		QString startshift = signoutinformation.getSignPageShift(signout_shitf);


		Configuration configuration;
		QString strstarttime = configuration.leftTime(startdate,startshift);//左时间
		
		QString strendtime = configuration.rightTime(startdate,startshift);//右时间
		




		//1判断事故信息新表里面数据条数,必须判断新表里用户所选的班次内有没有数据
		RECORDSETHANDLE isfaaccidentNullSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlfaaccidentisNull = "select COUNT(*) from H_FA_ACCIDENTINFO where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
		int rowsOfTersqlfaaccidentisNull = CPS_ORM_RsLoadData(isfaaccidentNullSetHandle,sqlfaaccidentisNull.toUtf8().data(),dbPoolHandle);
		int itemsoffaaccident = CPS_ORM_RsGetNumberValue(isfaaccidentNullSetHandle,0,0);
		CPS_ORM_RsFreeRecordSet(isfaaccidentNullSetHandle);

		//1先从事故信息老表里面展示已有数据
		if (itemsoffaaccident == 0)
			signoutinformation.firstshowFAaccidentInfo(strstarttime, strendtime ,startshift);
		//1事故信息新表里面已经有数据了，先把新表数据展示，然后再查老表
		else
			signoutinformation.secondshowFAaccidentInfo(strstarttime, strendtime ,startshift);

		//2判断检修计划新表里面数据条数,必须判断新表里用户所选的班次内有没有数据
		RECORDSETHANDLE ismaintenanceplanNullSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlmaintenanceplanisNull = "select COUNT(*) from H_MAINTENANCE_PLAN where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
		int rowsOfTersqlmaintenanceplanisNull = CPS_ORM_RsLoadData(ismaintenanceplanNullSetHandle,sqlmaintenanceplanisNull.toUtf8().data(),dbPoolHandle);
		int itemsofmaintenanceplan = CPS_ORM_RsGetNumberValue(ismaintenanceplanNullSetHandle,0,0);
		CPS_ORM_RsFreeRecordSet(ismaintenanceplanNullSetHandle);

		//2先从检修计划老表里面展示已有数据
		if (itemsofmaintenanceplan == 0)
			signoutinformation.firstshowMaintenancePlan(strstarttime, strendtime ,startshift);
		//2检修计划新表里面已经有数据了，先把新表数据展示，然后再查老表
		else
			signoutinformation.secondshowMaintenancePlan(strstarttime, strendtime ,startshift);

		//3判断遥控记录新表里面数据条数,必须判断新表里用户所选的班次内有没有数据
		RECORDSETHANDLE isdpcpointNullSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqldpcpointisNull = "select COUNT(*) from H_DPC_OPTRECORD where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
		int rowsOfTersqldpcpointisNull = CPS_ORM_RsLoadData(isdpcpointNullSetHandle,sqldpcpointisNull.toUtf8().data(),dbPoolHandle);
		int itemsofdpcpoint = CPS_ORM_RsGetNumberValue(isdpcpointNullSetHandle,0,0);
		CPS_ORM_RsFreeRecordSet(isdpcpointNullSetHandle);

		//3先从遥控记录老表里面展示已有数据
		if (itemsofdpcpoint == 0)
			signoutinformation.firstshowDPCOptRecord(strstarttime, strendtime ,startshift);
		//3遥控记录新表里面已经有数据了，先把新表数据展示，然后再查老表
		else
			signoutinformation.secondshowDPCOptRecord(strstarttime, strendtime ,startshift);

			

		//4判断线路重载新表里面数据条数,必须判断新表里用户所选的班次内有没有数据
		RECORDSETHANDLE isfeederoverloadNullSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlfeederoverloadisNull = "select COUNT(*) from H_FEEDER_OVERLOAD where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
		int rowsOfTersqlfeederoverloadisNull = CPS_ORM_RsLoadData(isfeederoverloadNullSetHandle,sqlfeederoverloadisNull.toUtf8().data(),dbPoolHandle);
		int itemsoffeederoverload = CPS_ORM_RsGetNumberValue(isfeederoverloadNullSetHandle,0,0);
		CPS_ORM_RsFreeRecordSet(isfeederoverloadNullSetHandle);

		//4先从线路重载老表里面展示已有数据
		if (itemsoffeederoverload == 0)
			signoutinformation.firstshowFeederOverload(strstarttime, strendtime ,startshift);
		//4线路重载新表里面已经有数据了，先把新表数据展示，然后再查老表
		else
			signoutinformation.secondshowFeederOverload(strstarttime, strendtime ,startshift);

		



		//用户编辑完点确认，进行入库操作，其中录入时间是主键
		if (signoutinformation.exec()==QDialog::Accepted)
		{


			//获取点击信息页面确认的当前日期时间,录入时间（非常重要）
			QDateTime informationcurDateTime=signoutinformation.getcurDateTimedata();
			qDebug()<<informationcurDateTime;


			//事故信息写入
			if (itemsoffaaccident == 0)
				signoutinformation.firstwriteFAaccidentTable(strstarttime, strendtime, startshift, informationcurDateTime);

			else
				signoutinformation.secondwriteFAaccidentTable(strstarttime, strendtime, startshift, informationcurDateTime);
			//检修计划写入
			if (itemsofmaintenanceplan == 0)
				signoutinformation.firstwriteMaintenancePlanTable(strstarttime, strendtime ,startshift, informationcurDateTime);
			
			else
				signoutinformation.secondwriteMaintenancePlanTable(strstarttime, strendtime ,startshift, informationcurDateTime);
			
			//遥控记录写入
			if (itemsofdpcpoint == 0)
				signoutinformation.firstwriteDPCOptRecordTable(strstarttime, strendtime, startshift, informationcurDateTime);
			
			else
				signoutinformation.secondwriteDPCOptRecordTable(strstarttime, strendtime, startshift, informationcurDateTime);

			//线路重载写入
			if (itemsoffeederoverload == 0)
				signoutinformation.firstwriteFeederOverloadTable(strstarttime, strendtime, startshift, informationcurDateTime);
			
			else
				signoutinformation.secondwriteFeederOverloadTable(strstarttime, strendtime, startshift, informationcurDateTime);



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
		//1获取点击接班时的当前日期时间,右时间
		QDateTime signincurDateTime=signin.getcurDateTimedata();
		//2获取用户选取的接班人
		QString signinname = signin.getsigninnamedata();
		//3获取接班时间,左时间
		QDateTime signinTime_QDateEdit = signin.getsigninTime_QDateEditdata();
		//4获取接班序号
		QString signin_shitf = signin.getsignin_shitfdata();

		SignInInformation signininformation;


		
		//根据交班序号可以取到左右时间，特殊情况为跨天




		//获取用户选择的交班时间段，左时间和右时间
		QDateTime startdate = signininformation.getSignPageDate(signinTime_QDateEdit);

		QString startshift = signininformation.getSignPageShift(signin_shitf);


		Configuration configuration;
		QString strstarttime = configuration.leftTime(startdate,startshift);//左时间

		QString strendtime = configuration.rightTime(startdate,startshift);//右时间

		

	

		signininformation.showsigninFAaccidentInfo(strstarttime, strendtime ,startshift);
		signininformation.showsigninMaintenancePlan(strstarttime, strendtime ,startshift);
		signininformation.showsigninDPCOptRecord(strstarttime, strendtime ,startshift);
		signininformation.showsigninFeederOverload(strstarttime, strendtime ,startshift);

		signininformation.exec();

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

void MainWindow::slotDoubleClickTreeNode(const QModelIndex index)
{
	
	/*
	
	//1获取点击接班时的当前日期时间,右时间
	QDateTime signincurDateTime=QDateTime::currentDateTime();
	//2获取用户选取的接班人
	QString signinname = "";
	//3获取接班时间,左时间
	QDateTime signinTime_QDateEdit = QDateTime::currentDateTime();
	//4获取接班序号
	QString signin_shitf = "1";

	Information information;


	//获取用户选择的接班时间段，右时间
	QDateTime endtime = information.getSignPageTime(signincurDateTime);
	QString strendtime = endtime.toString("yyyy-MM-dd hh:mm:ss");
	//qDebug()<<endtime;

	//获取用户选择的接班时间段，左时间
	QDateTime startdate = information.getSignPageDate(signinTime_QDateEdit);
	QString strstartdate = startdate.toString("yyyy-MM-dd");  

	QString startshift = information.getSignPageShift(signin_shitf);


	//根据date和shift拼接字符串左时间时间点
	QString strstarttime;//初始化左时间
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX="+startshift;
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
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



	qDebug()<<strstarttime;
	qDebug()<<strendtime;

	//查我自己建的四张新表里的数据，如果没有就只展示列名称

	information.showsigninFAaccidentInfo(strstarttime, strendtime ,startshift);
	information.showsigninMaintenancePlan(strstarttime, strendtime ,startshift);
	information.showsigninDPCOptRecord(strstarttime, strendtime ,startshift);
	information.showsigninFeederOverload(strstarttime, strendtime ,startshift);

	information.exec();



	*/
	QAbstractItemModel *Imodel=ui->SignInAndOut_tableview->model();
	QModelIndex Iindex = Imodel->index(index.row(),3);//index.row()为算选择的行号。1为所选中行的第一列。。
	QVariant datatemp=Imodel->data(Iindex);
	QString name=datatemp.toString();//name即为所选择行的第3列的值



	

	SignInInformation signininformation;

	signininformation.exec();
}

void MainWindow::queryAtTime()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	QStandardItemModel *SignInAndOut_model = new QStandardItemModel();
	SignInAndOut_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("time")));
	SignInAndOut_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("handover")));
	SignInAndOut_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("successor")));
	SignInAndOut_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_day")));
	SignInAndOut_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_index")));
	SignInAndOut_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("signinandoutnote")));
	//SignInAndOut_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("work_place")));	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->SignInAndOut_tableview->setModel(SignInAndOut_model);
	//QTableView平均分配列宽
	ui->SignInAndOut_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	//查找时间差
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//当前时间和排班时间存在时间差
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	//起始时间
	QDateTime startTime = ui->startTime->dateTime();
	int startTime_timetimestamp = startTime.toTime_t()+60*60*work_start;//往后推若干小时
	QDateTime startTime_timeplus = QDateTime::fromTime_t(startTime_timetimestamp);
	//结束时间
	QDateTime endTime = ui->endTime->dateTime();
	int endTime_timetimestamp = endTime.toTime_t()+60*60*work_start;//往后推若干小时
	QDateTime endTime_timeplus = QDateTime::fromTime_t(endTime_timetimestamp);
	
	RECORDSETHANDLE SignInAndOutSetHandle = CPS_ORM_RsNewRecordSet();
	//QString sqlSignInAndOut = "select TIME, HANDOVER, SUCCESSOR, WORK_DAY, WORK_INDEX, NOTE, WORK_PLACE from H_SIGNINANDOUT \
							  where TIME between to_date('"+startTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1";


	/*
	QString sqlSignInAndOut="select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE, a.WORK_PLACE from H_SIGNINANDOUT a,H_WORK_HOURS b \
							where a.WORK_INDEX=b.WORK_INDEX and a.TIME between to_date('"+startTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1 order by a.TIME";
    */


	QString sqlSignInAndOut="select a.TIME, a.HANDOVER, a.SUCCESSOR, a.WORK_DAY, b.WORK_NAME, a.NOTE from H_SIGNINANDOUT a,H_WORK_HOURS b \
							where a.WORK_INDEX=b.WORK_INDEX and a.TIME between to_date('"+startTime_timeplus.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss') and to_date('"+endTime_timeplus.toString("yyyy-MM-dd hh:mm:ss")+"','yyyy-MM-dd hh24:mi:ss')+1 order by a.TIME";






	int rowsOfTersqlSignInAndOut = CPS_ORM_RsLoadData(SignInAndOutSetHandle,sqlSignInAndOut.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTersqlSignInAndOut;i++)
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





		//非obid怎么办？？？
		//int workindex = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,4);
		//QStandardItem* item5 = new QStandardItem(QString::number(workindex));

		std::string workname = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,4);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(workname.c_str()));







		std::string note = CPS_ORM_RsGetStringValue(SignInAndOutSetHandle,i,5);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(note.c_str()));
		//QStandardItem* item6 = new QStandardItem(QString::fromStdString(note));
		//QStandardItem* item6 = new QStandardItem(QObject::tr(note));



		/*
		ObId workplaceobid = CPS_ORM_RsGetNumberValue(SignInAndOutSetHandle,i,6);
		QStandardItem* item7 = new QStandardItem(QString::number(workplaceobid));
		*/


		

		QList<QStandardItem*> item;
		item << item1 << item2 << item3 << item4 << item5 << item6;
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




void MainWindow::showPrintDialog()
{
    
	
	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	Print print;

	//confirm.show();
	if (print.exec()==QDialog::Accepted)
	{



	}
	
}




void MainWindow::showConfigurationForm()
{


	//测试万金油
	//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	Configuration configuration;

	//configuration.show();

	//configuration.exec();

	//configuration.show();

	
	if (configuration.exec()==QDialog::Accepted)
	{


		





	}
	
	
	
}