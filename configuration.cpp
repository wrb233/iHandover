#include "configuration.h"
#include "ui_configuration.h"

#include "confirm.h"
#include "ui_confirm.h"
#include "signin.h"
#include "signout.h"
#include "signininformation.h"
#include "signoutinformation.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"
#include <QString>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QRegExp>


#include "workhourdelegation.h"  


extern DBPOOLHANDLE dbPoolHandle;
extern UserMSInterface *g_User;
extern  Database *database;


Configuration::Configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);
	ui->tableWidgetConfiguration->setHorizontalHeaderLabels(QStringList()<<QObject::tr("workshift_name")<<QObject::tr("wor_start")<<QObject::tr("wor_end"));
	ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
	ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
	ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
	ui->tableWidgetConfiguration->setSelectionBehavior(QAbstractItemView::SelectRows); 

	//限制用户按照预期输入
	WorkHourDelegate *workHourDelegate = new WorkHourDelegate;
	ui->tableWidgetConfiguration->setItemDelegateForColumn(1, workHourDelegate);
	ui->tableWidgetConfiguration->setItemDelegateForColumn(2, workHourDelegate);

	//初始化配置表
	initConfiguration();
	//新增一行
	connect(ui->Add,SIGNAL(clicked()),this,SLOT(on_btn_Add_clicked()));
	//删除一行
	connect(ui->Del,SIGNAL(clicked()),this,SLOT(on_btn_Del_clicked()));
	//录入配置入库
	connect(ui->okButton,SIGNAL(clicked()),this,SLOT(on_btn_okButton_clicked()));
}

Configuration::~Configuration()
{
    delete ui;
}


void Configuration::on_btn_Add_clicked()
{
	int cols=ui->tableWidgetConfiguration->columnCount();
	int rows=ui->tableWidgetConfiguration->rowCount();
	ui->tableWidgetConfiguration->insertRow(rows);
	for(int i=0;i<cols;i++)
	{
		ui->tableWidgetConfiguration->setItem(rows,i,new QTableWidgetItem(""));
	}
	ui->tableWidgetConfiguration->selectRow(rows);
}
void Configuration::on_btn_Del_clicked()
{
	QTableWidgetItem * item = ui->tableWidgetConfiguration->currentItem();
	if(item==0)
		return;
	ui->tableWidgetConfiguration->removeRow(item->row());
}
void Configuration::on_btn_okButton_clicked()
{
	//先清空配置表
	QString tableWidgetConfiguration = "delete from H_WORK_HOURS";
	InitConTable::executeSQL(tableWidgetConfiguration,0);
	

	int rows=ui->tableWidgetConfiguration->rowCount();
	for (int i=0;i<rows;i++)
	{
		QString work_index = QString::number(i);//取出班序号从0开始
		QString work_name = ui->tableWidgetConfiguration->item(i,0)->text();//取出班名称
		QString wor_start = ui->tableWidgetConfiguration->item(i,1)->text();//取出班开始时段(24小时制)
		QString wor_end = ui->tableWidgetConfiguration->item(i,2)->text();//取出班结束时段(24小时制)
		//拼接交接班交接记录总表的sql语句
		QString configurationsql = QString("INSERT INTO H_WORK_HOURS (WORK_INDEX,WORK_NAME,WOR_START,WOR_END) VALUES (%1,'%2',%3,%4)")
			.arg(work_index)
			.arg(work_name)
			.arg(wor_start)
			.arg(wor_end);
		//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
		InitConTable::executeSQL(configurationsql,0);
	}
	QMessageBox::information(NULL, QObject::tr("Done"), QObject::tr("Configuration has been effected"));
}







void Configuration::initConfiguration()
{
    
	RECORDSETHANDLE numConfigurationSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlnumConfiguration = "select COUNT(*) from H_WORK_HOURS";
	int numrowsOfTers = CPS_ORM_RsLoadData(numConfigurationSetHandle,sqlnumConfiguration.toUtf8().data(),dbPoolHandle);
	int rows = CPS_ORM_RsGetNumberValue(numConfigurationSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(numConfigurationSetHandle);

	ui->tableWidgetConfiguration->setRowCount(rows);
	for(int i=0;i<rows;i++)
	{
		RECORDSETHANDLE queryConfigurationSetHandle = CPS_ORM_RsNewRecordSet();
		QString querysqlConfiguration = "select work_index, work_name, wor_start, wor_end from H_WORK_HOURS order by work_index";
		int newrowsOfTer = CPS_ORM_RsLoadData(queryConfigurationSetHandle,querysqlConfiguration.toUtf8().data(),dbPoolHandle);

		std::string work_name = CPS_ORM_RsGetStringValue(queryConfigurationSetHandle,i,1);//班名称
		ui->tableWidgetConfiguration->setItem(i,0,new QTableWidgetItem(QString::fromUtf8(work_name.c_str())));
		 
		int wor_start = (int)CPS_ORM_RsGetNumberValue(queryConfigurationSetHandle,i,2);//班开始时间
		ui->tableWidgetConfiguration->setItem(i,1,new QTableWidgetItem(QString::number(wor_start)));
		

		int wor_end = (int)CPS_ORM_RsGetNumberValue(queryConfigurationSetHandle,i,3);//班结束时间
		ui->tableWidgetConfiguration->setItem(i,2,new QTableWidgetItem(QString::number(wor_end)));

	}

}

bool Configuration::isTimeScale(int startTimeHour, int endTimeHour)
{
	//将时间都转化为分钟数，利于判断
	int startMin = startTimeHour * 60;
	int endMin = endTimeHour * 60;

	QTime current_time =QTime::currentTime();
	int hour = current_time.hour();//当前的小时
	int curMin = hour * 60;
	//不跨天的情况
	if (startMin < endMin )
	{
		if (startMin <= curMin && curMin < endMin)//左闭右开
		{
			return true;
		}
	}
	//跨天的情况
	else if (startMin > endMin)
	{
		if (startMin <= curMin || curMin <= endMin)
		{
			return true;
		}
	}
	//全天的情况
	else if (startMin = endMin)
	{
		return true;
	}
	return false;
}

int Configuration::whichShift()
{
	RECORDSETHANDLE numConfigurationSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlnumConfiguration = "select COUNT(*) from H_WORK_HOURS";
	int numrowsOfTers = CPS_ORM_RsLoadData(numConfigurationSetHandle,sqlnumConfiguration.toUtf8().data(),dbPoolHandle);
	int rows = CPS_ORM_RsGetNumberValue(numConfigurationSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(numConfigurationSetHandle);

	for(int i=0;i<rows;i++)
	{
		RECORDSETHANDLE queryConfigurationSetHandle = CPS_ORM_RsNewRecordSet();
		QString querysqlConfiguration = "select work_index, work_name, wor_start, wor_end from H_WORK_HOURS order by work_index";
		int newrowsOfTer = CPS_ORM_RsLoadData(queryConfigurationSetHandle,querysqlConfiguration.toUtf8().data(),dbPoolHandle);

		int work_index = (int)CPS_ORM_RsGetNumberValue(queryConfigurationSetHandle,i,0);//班序号
		int wor_start = (int)CPS_ORM_RsGetNumberValue(queryConfigurationSetHandle,i,2);//班开始时间
		int wor_end = (int)CPS_ORM_RsGetNumberValue(queryConfigurationSetHandle,i,3);//班结束时间

		if (isTimeScale(wor_start,wor_end))
		{
			return work_index;
		}
		CPS_ORM_RsFreeRecordSet(queryConfigurationSetHandle);

	}
}

QString Configuration::leftTime(QDateTime startdate,QString work_index)
{
	RECORDSETHANDLE leftTimeSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlleftTime = "select wor_start,wor_end from H_WORK_HOURS where work_index="+work_index;
	int numrowsOfTers = CPS_ORM_RsLoadData(leftTimeSetHandle,sqlleftTime.toUtf8().data(),dbPoolHandle);
	int wor_start = (int)CPS_ORM_RsGetNumberValue(leftTimeSetHandle,0,0);//班开始时间
	int wor_end = (int)CPS_ORM_RsGetNumberValue(leftTimeSetHandle,0,1);//班结束时间

	CPS_ORM_RsFreeRecordSet(leftTimeSetHandle);

	QString strhour = QString::number(wor_start);

	QString strstartdate = startdate.toString("yyyy-MM-dd");

	if (wor_start>=wor_end)//跨天
	{
		QString strstarttime;//初始化左时间
		strstarttime = QString("%1 %2:00:00")
			.arg(strstartdate)
			.arg(strhour);

		return strstarttime;
	}else                 //不跨天
	{
		QString strstarttime;//初始化左时间
		strstarttime = QString("%1 %2:00:00")
			.arg(strstartdate)
			.arg(strhour);

		return strstarttime;
	}
	


}


QString Configuration::rightTime(QDateTime enddate,QString work_index)
{
	RECORDSETHANDLE rightTimeSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlrightTime = "select wor_start,wor_end from H_WORK_HOURS where work_index="+work_index;
	int numrowsOfTers = CPS_ORM_RsLoadData(rightTimeSetHandle,sqlrightTime.toUtf8().data(),dbPoolHandle);
	int wor_start = (int)CPS_ORM_RsGetNumberValue(rightTimeSetHandle,0,0);//班开始时间
	int wor_end = (int)CPS_ORM_RsGetNumberValue(rightTimeSetHandle,0,1);//班结束时间

	CPS_ORM_RsFreeRecordSet(rightTimeSetHandle);

	QString strhour = QString::number(wor_end);

	QString strenddate = enddate.toString("yyyy-MM-dd");
	QString strenddateplus = enddate.addDays(1).toString("yyyy-MM-dd");
	if (wor_start>=wor_end)//跨天
	{
		QString strendtime;//初始化右时间
		strendtime = QString("%1 %2:00:00")
			.arg(strenddateplus)
			.arg(strhour);

		return strendtime;
	}else                 //不跨天
	{
		QString strendtime;//初始化右时间
		strendtime = QString("%1 %2:00:00")
			.arg(strenddate)
			.arg(strhour);

		return strendtime;
	}



}
















