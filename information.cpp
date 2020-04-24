#include "information.h"
#include "ui_information.h"
#include <QMessageBox>


#include <QStandardItemModel>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "signout.h"
#include "signin.h"
#include "information.h"
#include "confirm.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"

#include <QCoreApplication>

extern DBPOOLHANDLE dbPoolHandle;

extern  Database *database;


Information::Information(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Information)
{
    

	 ui->setupUi(this);
	 
    
}
 Information::~Information()

{

    delete ui;

}

  
 void Information::firstshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *FAInfo_model = new QStandardItemModel();
	FAInfo_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("accident_info")));
	FAInfo_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("work_day")));
	FAInfo_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_index")));
	FAInfo_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("start_time")));
	//FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("end_time")));
	FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("reason")));
	//FAInfo_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("acker")));
	FAInfo_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("note")));
	ui->accidentinfo_tableview->setModel(FAInfo_model);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	RECORDSETHANDLE firstgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	AT_Name = database->matchAType("Name");
	for (int i=0;i<rowsOfTer;i++)
	{
		//FEEDEROBJECTID事故信息
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID父
		StringData data;
		ToolUtil::databaseRead(feederid, AT_Name, &data);
		OMString str = (OMString)data;
		QString qstr = QString::fromUtf8(str.c_str());
		QStandardItem* item1 = new QStandardItem(qstr);

		//交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item2 = new QStandardItem(strstarttimeList[0]);
		
		//交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);

		//录入时间

		//FAULTTIME发生时间	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//结束时间,让用户去填写

		//QStandardItem* item5 = new QStandardItem(QObject::tr(""));//结束时间

		//TYPE事故原因
		ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID子
		short fatype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("overcurrent"));
		else
			qstr3.append(tr("deflection"));
		QStandardItem* item6 = new QStandardItem(qstr3);

		//确认人
		//QStandardItem* item7 = new QStandardItem(QObject::tr(""));
		//备注
		QStandardItem* item8 = new QStandardItem(QObject::tr(""));
		



		
		QList<QStandardItem*> item;
        //item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

		item << item1 << item2 <<item3 <<item4 <<item6 <<item8;

        FAInfo_model->appendRow(item);
	
		   
	}
	CPS_ORM_RsFreeRecordSet(firstgatherFAInfoSetHandle);
		
}

void Information::firstshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *MAINTPlan_model = new QStandardItemModel();
	MAINTPlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("变电站")));
    MAINTPlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("线路名称")));
	MAINTPlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("交接日")));
	MAINTPlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("交接班序号")));
    MAINTPlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("停电开始时间")));
    MAINTPlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("停电结束时间")));
    MAINTPlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("停电性质")));
	MAINTPlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("停电原因")));
	MAINTPlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("停电时长（小时）")));
	MAINTPlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("备注")));
	ui->maintenanceplan_tableview->setModel(MAINTPlan_model);
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	
}

void Information::firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *DPCPoint_model = new QStandardItemModel();
	DPCPoint_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("区域")));
	DPCPoint_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("变电站")));
	DPCPoint_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("线路名称")));
	DPCPoint_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("开关名称")));
	DPCPoint_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("交接日")));
	DPCPoint_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("交接班序号")));
	DPCPoint_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("操作内容")));
	DPCPoint_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("操作时间")));
	DPCPoint_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("操作人")));
	DPCPoint_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("监护人")));
	DPCPoint_model->setHorizontalHeaderItem(10, new QStandardItem(QObject::tr("备注")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableView平均分配列宽
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

}

void Information::firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift)
{

	QStandardItemModel *FeederOverload_model = new QStandardItemModel();
	FeederOverload_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("区域")));
	FeederOverload_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("变电站")));
	FeederOverload_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("线路名称")));
	FeederOverload_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("交接日")));
	FeederOverload_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("交接班序号")));
	FeederOverload_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("额定电流（A）")));
	FeederOverload_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("最大电流")));
	FeederOverload_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("出现时间")));
	FeederOverload_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("重载率")));
	FeederOverload_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("备注")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->lineload_tableview->setModel(FeederOverload_model);
	//QTableView平均分配列宽
	ui->lineload_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

}


/*
void Information::receivesignoutcurDateTimeData(QDateTime data)

{

	qDebug()<<data;
}
void Information::receivesignoutnameData(QString data)
{
	qDebug()<<data;
	
}
void Information::receivesignoutTime_QDateEditData(QDateTime data)

{

	qDebug()<<data;
}
void Information::receivesignout_shiftData(QString data)
{

	qDebug()<<data;
}
*/



//从老表里面查哪一天，哪一个班次的信息，把已有的字段给他先填好
void Information::firstShowTableViewsFromOldTables(QString strstarttime, QString strendtime, QString startshift)
{
	firstshowFAaccidentInfo(strstarttime, strendtime, startshift);
	firstshowMaintenancePlan(strstarttime, strendtime, startshift);
	firstshowDPCOptRecord(strstarttime, strendtime, startshift);
	firstshowFeederOverload(strstarttime, strendtime, startshift);
}


QDateTime Information::getSignPageTime(QDateTime signcurtime)
{
	QDateTime signpagetime = signcurtime;
	return signpagetime;
}

QDateTime Information::getSignPageDate(QDateTime signdate)
{
	QDateTime signpagedate = signdate;
	return signpagedate;
}

QString Information::getSignPageShift(QString signshift)
{
	 QString signpageshift = signshift;
	 return signpageshift;
}

QDateTime Information::getcurDateTimedata()
{	 
	return informationcurDateTime;
}





void Information::on_okButton_clicked()
{
	
	//录入时间
	informationcurDateTime=QDateTime::currentDateTime();






}



//写表需要四个参数，strstarttime交接日，strendtime结束时间，startshift交接班次，informationcurDateTime录入时间（非常重要）

void Information::firstwriteTables(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)

{

	
	firstwriteFAaccidentTable(strstarttime, strendtime, startshift, informationcurDateTime);





}
void Information::secondwriteTables(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)

{


	secondwriteFAaccidentTable(strstarttime, strendtime, startshift, informationcurDateTime);





}


void Information::firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	RECORDSETHANDLE queryFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	

	QString querysqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	
	

	int rowsOfTer = CPS_ORM_RsLoadData(queryFAInfoSetHandle,querysqlFAInfo.toUtf8().data(),dbPoolHandle);


	//我这里只想知道写几条
	for (int i=0;i<rowsOfTer;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//事故信息，汉字char
	    QAbstractItemModel *accidentinfo_model = ui->accidentinfo_tableview->model();
		QModelIndex index = accidentinfo_model->index(i,0);
		QString accidentinfo_name = accidentinfo_model->data(index).toString();
		//qDebug()<<name;
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME发生时间timestamp	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//结束时间，让用户自己录入的时间，怎么保存到库里

		//TYPE事故原因
		ObId cbid = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,1);//OBJECTID子
		short fatype = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("过流"));
		else
			qstr3.append(tr("变位"));
		//确认人
		//QModelIndex index2 = accidentinfo_model->index(i,6);
		//QString confirm_name = accidentinfo_model->data(index2).toString();
		//qDebug()<<name2;
		//备注
		QModelIndex index3 = accidentinfo_model->index(i,5);
		QString note_name = accidentinfo_model->data(index3).toString();
		
		/*
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,acker,note) VALUES ('"\
				
				+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


				
				+QString("%3").arg(qstr3)+"','"\
				+QString("%4").arg(confirm_name)+"','"\
				+QString("%5").arg(note_name)+"')");

				*/
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(qstr3)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	CPS_ORM_RsFreeRecordSet(queryFAInfoSetHandle);

}

void Information::secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//这里只能插入新的数据了，不能暴力清空表，重头插，我怎么知道新的数据从哪里开始的？？？

	
	
	//获取上一次录入时间，查询老表的时候就以这个时间为左时间

	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO";
	int queryrowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, queryrowsOfTer-1, 4);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//多一秒
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FA_ACCIDENTINFO";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	
	
	
	RECORDSETHANDLE queryFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString querysqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int newrowsOfTer = CPS_ORM_RsLoadData(queryFAInfoSetHandle,querysqlFAInfo.toUtf8().data(),dbPoolHandle);


	//我这里只想知道写几条
	for (int i=0;i<newrowsOfTer;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//事故信息，汉字char
	    QAbstractItemModel *accidentinfo_model = ui->accidentinfo_tableview->model();
		QModelIndex index = accidentinfo_model->index(i,0);
		QString accidentinfo_name = accidentinfo_model->data(index).toString();
		//qDebug()<<name;
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME发生时间timestamp	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//结束时间，让用户自己录入的时间，怎么保存到库里

		//TYPE事故原因
		ObId cbid = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,1);//OBJECTID子
		short fatype = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("过流"));
		else
			qstr3.append(tr("变位"));
		//确认人
		//QModelIndex index2 = accidentinfo_model->index(i,6);
		//QString confirm_name = accidentinfo_model->data(index2).toString();
		//qDebug()<<name2;
		//备注
		QModelIndex index3 = accidentinfo_model->index(items+i,5);
		QString note_name = accidentinfo_model->data(index3).toString();
		
		/*
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,acker,note) VALUES ('"\
				
				+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


				
				+QString("%3").arg(qstr3)+"','"\
				+QString("%4").arg(confirm_name)+"','"\
				+QString("%5").arg(note_name)+"')");

				*/
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(qstr3)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	CPS_ORM_RsFreeRecordSet(queryFAInfoSetHandle);

}






















void Information::secondShowTableViewsFromOldTables(QString strstarttime, QString strendtime, QString startshift)
{
	secondshowFAaccidentInfo(strstarttime, strendtime, startshift);
	//secondshowMaintenancePlan(strstarttime, strendtime, startshift);
	//secondshowDPCOptRecord(strstarttime, strendtime, startshift);
	//secondshowFeederOverload(strstarttime, strendtime, startshift);
}


void Information::secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *FAInfo_model = new QStandardItemModel();
	FAInfo_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("accident_info")));
	FAInfo_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("work_day")));
	FAInfo_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_index")));
	FAInfo_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("start_time")));
	//FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("end_time")));
	FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("reason")));
	//FAInfo_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("acker")));
	FAInfo_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("note")));
	ui->accidentinfo_tableview->setModel(FAInfo_model);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	

	//从新表里面去查啦，哈哈哈
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//ACCIDENT_INFO事故信息
		std::string accident_info = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(accident_info.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY交接日	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle,i,1);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item2 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX交接班序号
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherFAInfoSetHandle,i,2);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strstartshift.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);

		//录入时间


		//START_TIME发生时间
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 4);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//END_TIME结束时间,系统默认时间
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//REASON事故原因
		std::string reason = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,5);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(reason.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//ACKER确认人
		std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//NOTE备注
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(note.c_str()));
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1 << item2 <<item3 <<item5 << item7 <<item9;
		FAInfo_model->appendRow(item);


	}


	//获取上一次录入时间，查询老表的时候就以这个时间为左时间
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, rowsOfTer-1, 4);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//多一秒
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);


	//再从老表里面去查，记得卡时间，左时间是上一次录入时间，右时间是这一次点击okbutton时间,追加数据

	RECORDSETHANDLE firstgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	AT_Name = database->matchAType("Name");
	for (int i=0;i<rowsOfTer2;i++)
	{
		//FEEDEROBJECTID事故信息
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID父
		StringData data;
		ToolUtil::databaseRead(feederid, AT_Name, &data);
		OMString str = (OMString)data;
		QString qstr = QString::fromUtf8(str.c_str());
		QStandardItem* newitem1 = new QStandardItem(qstr);

		//交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem2 = new QStandardItem(strstarttimeList[0]);

		//交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);

		//录入时间

		//FAULTTIME发生时间	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* newitem4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//结束时间,让用户去填写

		//QStandardItem* newitem5 = new QStandardItem(QObject::tr(""));//结束时间

		//TYPE事故原因
		ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID子
		short fatype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("过流"));
		else
			qstr3.append(tr("变位"));
		QStandardItem* newitem6 = new QStandardItem(qstr3);

		//确认人
		//QStandardItem* newitem7 = new QStandardItem(QObject::tr(""));
		//备注
		QStandardItem* newitem8 = new QStandardItem(QObject::tr(""));





		QList<QStandardItem*> newitem;
		//newitem << newitem1 << newitem2 <<newitem3 <<newitem4 <<newitem5 << newitem6 <<newitem7 <<newitem8;
		newitem << newitem1 << newitem2 <<newitem3 <<newitem4 << newitem6 <<newitem8;
		FAInfo_model->appendRow(newitem);


	}
	CPS_ORM_RsFreeRecordSet(firstgatherFAInfoSetHandle);

}










