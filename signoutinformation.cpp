#include "signoutinformation.h"
#include "ui_signoutinformation.h"
#include <QMessageBox>
#include "configuration.h"
#include "ui_configuration.h"
#include <QStandardItemModel>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "signout.h"
#include "signin.h"
#include "signoutinformation.h"
#include "confirm.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"
#include <QCoreApplication>

#include "datedelegate.h"

extern DBPOOLHANDLE dbPoolHandle;
extern  Database *database;
extern InitConTable *initConTable;
extern QList<QString> MVPointlist;

SignOutInformation::SignOutInformation(QWidget *parent) :
    QDialog(parent),	
	ui(new Ui::SignOutInformation)
{
	 ui->setupUi(this);
	 //新增一行，对检修计划单独处理
	 connect(ui->Add,SIGNAL(clicked()),this,SLOT(on_btn_Add_clicked()));
	 //删除一行，对检修计划单独处理
	 connect(ui->Del,SIGNAL(clicked()),this,SLOT(on_btn_Del_clicked()));  
}
 SignOutInformation::~SignOutInformation()
{
    delete ui;
}
void SignOutInformation::firstshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
{
	FAInfo_model = new QStandardItemModel(ui->accidentinfo_tableview);
	FAInfo_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("accident_info")));
	FAInfo_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("work_day")));
	FAInfo_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_index")));
	FAInfo_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("start_time")));
	//FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("end_time")));
	FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("reason")));
	//FAInfo_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("acker")));
	FAInfo_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("note")));
	ui->accidentinfo_tableview->setModel(FAInfo_model);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);//事故原因
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);//备注

	RECORDSETHANDLE firstgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') AND TYPE =1 order by MESSAGETIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//FEEDEROBJECTID事故信息
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID父
		StringData feederiddata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederiddata);
		OMString strfeederid = (OMString)feederiddata;
		QString qstrfeederid = QString::fromUtf8(strfeederid.c_str());
		QStandardItem* item1 = new QStandardItem(qstrfeederid);

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
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//结束时间,预留
		//QStandardItem* item5 = new QStandardItem(QObject::tr(""));//结束时间

		//TYPE事故原因
		ObId sonoffeederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//FEEDEROBJECTID子
		short feedertype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData sonoffeederiddata;
		ToolUtil::databaseRead(sonoffeederid, AT_Name, &sonoffeederiddata);
		OMString strsonoffeederid = (OMString)sonoffeederiddata;
		QString qstrsonoffeederid = QString::fromUtf8(strsonoffeederid.c_str());
		//QString qreason = (QString)(QObject::tr("overcurrent"));
		//qstrsonoffeederid.append(QObject::tr("过流"));
		QStandardItem* item6 = new QStandardItem(qstrsonoffeederid);
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

void SignOutInformation::firstshowMaintenancePlan(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift)
{
	MaintenancePlan_model = new QStandardItemModel(ui->maintenanceplan_tableview);
	MaintenancePlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("station")));
	MaintenancePlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("feeder")));
	MaintenancePlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_day")));
	MaintenancePlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_index")));
	MaintenancePlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("poweroffstart_time")));
	MaintenancePlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("poweroffend_time")));
	MaintenancePlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("powerofftype")));
	MaintenancePlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("poweroffreason")));
	MaintenancePlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("powerofftime")));
	MaintenancePlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->maintenanceplan_tableview->setModel(MaintenancePlan_model);

	ui->maintenanceplan_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
	//QTableView平均分配列宽
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

	DateDelegate *datedelegate = new DateDelegate;

	ui->maintenanceplan_tableview->setItemDelegateForColumn(5,datedelegate);

	


}

void SignOutInformation::firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
{
	DPCPoint_model = new QStandardItemModel(ui->DPCPoint_tableview);
	DPCPoint_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("area")));
	DPCPoint_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("station")));
	DPCPoint_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("feeder")));
	DPCPoint_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("switch")));
	DPCPoint_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_day")));
	DPCPoint_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("work_index")));
	DPCPoint_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("opt_content")));
	DPCPoint_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("opt_time")));
	DPCPoint_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("operatorinfo")));
	DPCPoint_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("guarderinfo")));
	DPCPoint_model->setHorizontalHeaderItem(10, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableView平均分配列宽
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide();

	

	//successTimes = 0;
	//failTimes = 0;
	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	
	for (int i=0;i<rowsOfTer;i++)
	{
		//找到遥控
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,0);
		//找到关联遥测
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//找到psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;
		//1区域
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//2变电站
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));
		//3线路名称
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData feederdata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));
		//4开关名称,obid递归向上查询
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData switchdata;
		ToolUtil::databaseRead(switchid, AT_Name, &switchdata);
		OMString strswitchdata = (OMString)switchdata;
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strswitchdata.c_str()));

		//5交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item5 = new QStandardItem(strstarttimeList[0]);

		//6交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


		//录入时间


		//7操作内容
		QStandardItem* item7;
		int dpcAction = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,1);
		//QString dpcAction = QString::number(intdpcAction);
		int actionResult = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,3);
		//QString actionResult = QString::number(intactionResult);

		if ((dpcAction== 0) && (actionResult == 0))
		{	
			//QString optcontent = "choicesuccess";
			item7 = new QStandardItem(QObject::tr("choicesuccess"));
		
		}
		else if ((dpcAction== 0)&&( actionResult == 1))
		{
			//QString optcontent = "choicefail";
			item7 = new QStandardItem(QObject::tr("choicefail"));
		}

		else if ((dpcAction== 0)&&( actionResult == 2))
		{
			//QString optcontent = "choiceovertime";
			item7 = new QStandardItem(QObject::tr("choiceovertime"));
		}	

		else if ((dpcAction== 1)&&( actionResult == 0))
		{
			//QString optcontent = "actionsuccess";
			item7 = new QStandardItem(QObject::tr("actionsuccess"));
			//successTimes++;
		}	

		else if ((dpcAction== 1)&&( actionResult == 1))
		{
			//QString optcontent = "actionfail";
			item7 = new QStandardItem(QObject::tr("actionfail"));
			//failTimes++;
		}	

		else if ((dpcAction== 1)&&( actionResult == 2))
		{
			//QString optcontent = "actionovertime";
			item7 = new QStandardItem(QObject::tr("actionovertime"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 0))
		{
			//QString optcontent = "cancelsuccess";
			item7 = new QStandardItem(QObject::tr("cancelsuccess"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 1))
		{
			//QString optcontent = "cancelfail";
			item7 = new QStandardItem(QObject::tr("cancelfail"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 2))
		{
			QString optcontent = "cancelovertime";
			item7 = new QStandardItem(QObject::tr("cancelovertime"));
		}	
		else if ((dpcAction== 3)&&( actionResult == 0))
		{
			QString optcontent = "communicationbreakdown";
			item7 = new QStandardItem(QObject::tr("communicationbreakdown"));
		}	
		else if ((dpcAction== 4)&&( actionResult == 0))
		{
			QString optcontent = "controlconflict";
			item7 = new QStandardItem(QObject::tr("controlconflict"));
		}	
		else if ((dpcAction== 5)&&( actionResult == 0))
		{
			QString optcontent = "missionovertime";
			item7 = new QStandardItem(QObject::tr("missionovertime"));
		}	
		else if ((dpcAction== 6)&&( actionResult == 0))
		{
			QString optcontent = "transfersuccess";
			item7 = new QStandardItem(QObject::tr("transfersuccess"));
		}	
		else if ((dpcAction== 6)&&( actionResult == 1))
		{
			QString optcontent = "transferfail";
			item7 = new QStandardItem(QObject::tr("transferfail"));
		}	
		else if ((dpcAction== 7)&&( actionResult == 0))
		{
			QString optcontent = "haltsuccess";
			item7 = new QStandardItem(QObject::tr("haltsuccess"));
		}
		else if ((dpcAction== 7)&&( actionResult == 1))
		{
			QString optcontent = "haltfail";
			item7 = new QStandardItem(QObject::tr("haltfail"));
		}
		else if ((dpcAction== 8)&&( actionResult == 0))
		{
			QString optcontent = "choicestart";
			item7 = new QStandardItem(QObject::tr("choicestart"));
		}
		else if ((dpcAction== 9)&&( actionResult == 0))
		{
			QString optcontent = "actionstart";
			item7 = new QStandardItem(QObject::tr("actionstart"));
		}
		else if((dpcAction== 10)&&( actionResult == 0))
		{
			QString optcontent = "cancelstart";
			item7 = new QStandardItem(QObject::tr("cancelstart"));
		}
		



		//QStandardItem* item7 = new QStandardItem(QObject::tr(""));

		//8操作时间
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(firstgatherDPCPointSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* item8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));
		
		//9操作人
		std::string operatorinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,4);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(operatorinfo.c_str()));
		
		//10监护人
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));
		
		//11备注
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

		item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7 <<item8<<item9<<item10<<item11;

		DPCPoint_model->appendRow(item);


	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);

	ui->label_6->setText(QString::number(getsuccessTimes(strstarttime,strendtime,startshift)));
	ui->label_2->setText(QString::number(getfailTimes(strstarttime,strendtime,startshift)));

}


int SignOutInformation::getsuccessTimes(QString strstarttime, QString strendtime, QString startshift)
{
	//始终获取老表里面查找到的成功和失败次数
	successTimes = 0;
	//failTimes = 0;
	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);

	for (int i=0;i<rowsOfTer;i++)
	{
		//7操作内容
		
		int dpcAction = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,1);
		//QString dpcAction = QString::number(intdpcAction);
		int actionResult = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,3);
		//QString actionResult = QString::number(intactionResult);

		if ((dpcAction== 1)&&( actionResult == 0))
		{
			//QString optcontent = "actionsuccess";
			//item7 = new QStandardItem(QObject::tr("actionsuccess"));
			successTimes++;
		}	
	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);

	
	return successTimes;
	//ui->label_6->setText(QString::number(successTimes));
	//ui->label_2->setText(QString::number(failTimes));

}

int SignOutInformation::getfailTimes(QString strstarttime, QString strendtime, QString startshift)
{
	//始终获取老表里面查找到的成功和失败次数
	//successTimes = 0;
	failTimes = 0;
	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);

	for (int i=0;i<rowsOfTer;i++)
	{
		//7操作内容

		int dpcAction = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,1);
		//QString dpcAction = QString::number(intdpcAction);
		int actionResult = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,3);
		//QString actionResult = QString::number(intactionResult);

		if ((dpcAction== 1)&&( actionResult == 1))
		{
			//QString optcontent = "actionsuccess";
			//item7 = new QStandardItem(QObject::tr("actionsuccess"));
			failTimes++;
		}	
	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);



	//ui->label_6->setText(QString::number(successTimes));
	//ui->label_2->setText(QString::number(failTimes));
	return failTimes;

}




void SignOutInformation::firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift)
{
	FeederOverload_model = new QStandardItemModel(ui->lineload_tableview);
	FeederOverload_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("area")));
	FeederOverload_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("station")));
	FeederOverload_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("feeder")));
	FeederOverload_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_day")));
	FeederOverload_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_index")));
	FeederOverload_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("amprating")));
	FeederOverload_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("max_current")));
	FeederOverload_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("occur_time")));
	FeederOverload_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("overloadrate")));
	FeederOverload_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->lineload_tableview->setModel(FeederOverload_model);
	//QTableView平均分配列宽
	ui->lineload_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

	RECORDSETHANDLE firstgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	qDebug()<<MVPointlist.size();
	QString strMVPointlist="";
	QString strMVPointlist1="";
	QString strMVPointlist2="";

	QString firstsqlFeederOverload;

	QDateTime curDateTime = QDateTime::currentDateTime();
	QString yearmonth = curDateTime.toString("yyyyMM");

	if (MVPointlist.size()<1000)
	{

		for ( int i=0; i!=MVPointlist.size(); ++i )
		{
			strMVPointlist = strMVPointlist.append(MVPointlist[i]).append(",");
		}
		//strMVPointlist.append(MVPointlist[MVPointlist.size()]);
		//qDebug()<<strMVPointlist;
		strMVPointlist.chop(1);
		qDebug()<<strMVPointlist;
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_"+yearmonth+" where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+") order by SDATE";



	} 
	else
	{
		
		for ( int i=0; i<1000; ++i )
		{
			strMVPointlist1 = strMVPointlist1.append(MVPointlist[i]).append(",");
		}
		strMVPointlist1.chop(1);
		
		for ( int j=1000; j!=MVPointlist.size(); ++j )
		{
			strMVPointlist2 = strMVPointlist2.append(MVPointlist[j]).append(",");
		}
		//strMVPointlist.append(MVPointlist[MVPointlist.size()]);
		//qDebug()<<strMVPointlist;
		strMVPointlist2.chop(1);

		qDebug()<<strMVPointlist1;
		qDebug()<<strMVPointlist2;
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_"+yearmonth+" where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


	}
	
	//RECORDSETHANDLE firstgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	//QString firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL from H_HOURMVPOINTSTAT_201909 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+")";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherFeederOverloadSetHandle,firstsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	//AT_Name = database->matchAType("Name");
	
	for (int i=0;i<rowsOfTer;i++)
	{
		//找到遥控
		ObId mvpointid = CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,0);

		//找到psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);

		//1区域
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//2变电站
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));
		//3线路名称
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		StringData feederdata;
		ToolUtil::databaseRead(MemberOfLineid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));
		//4交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item4 = new QStandardItem(strstarttimeList[0]);
		//5交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
		
		//录入时间
		//6额定电流PMSBreaker
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		FloatData switchampratingdata;
		ToolUtil::databaseRead(switchid, AT_ampRating, &switchampratingdata);
		QStandardItem* item6 = new QStandardItem(QString::number(switchampratingdata));
		//7最大电流
		int maxcurrent = (int)CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,2);
		QStandardItem* item7 = new QStandardItem(QString::number(maxcurrent));
		//8出现时间
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFeederOverloadSetHandle,i,3);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item8 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));
		/*
		//重载率
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherFeederOverloadSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//备注
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));

		*/
		//9重载率
		QStandardItem* item9 = new QStandardItem(QObject::tr(""));
		//10备注
		QStandardItem* item10 = new QStandardItem(QObject::tr(""));
		QList<QStandardItem*> item;
		item << item1 << item2 <<item3 <<item4 <<item5 <<item6<<item7<<item8<<item9<<item10;
		double double_switchampratingdata  = switchampratingdata;
		double double_maxcurrent = maxcurrent;
		if(double_switchampratingdata==0.0 || double_maxcurrent==0.0)
			continue;
		if(double_maxcurrent>=double_switchampratingdata*0.75)
		{
			FeederOverload_model->appendRow(item);
		}
		

	}
	CPS_ORM_RsFreeRecordSet(firstgatherFeederOverloadSetHandle);
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



QDateTime SignOutInformation::getSignPageTime(QDateTime signcurtime)
{
	signpagetime = signcurtime;
	return signpagetime;
}

QDateTime SignOutInformation::getSignPageDate(QDateTime signdate)
{
	signpagedate = signdate;
	return signpagedate;
}

QString SignOutInformation::getSignPageShift(QString signshift)
{
	 signpageshift = signshift;
	 return signpageshift;
}

QDateTime SignOutInformation::getcurDateTimedata()
{	 
	return informationcurDateTime;
}





void SignOutInformation::on_okButton_clicked()
{
	
	//录入时间
	informationcurDateTime=QDateTime::currentDateTime();






}



//写表需要四个参数，strstarttime交接日，strendtime结束时间，startshift交接班次，informationcurDateTime录入时间（非常重要）



void SignOutInformation::firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	int rows = FAInfo_model->rowCount();
	//我这里只想知道写几条
	for (int i=0;i<rows;i++)
	{
		//事故信息，汉字char
		QModelIndex accidentinfo_nameindex = FAInfo_model->index(i,0);
		QString accidentinfo_name = FAInfo_model->data(accidentinfo_nameindex).toString();
		//qDebug()<<name;
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME发生时间timestamp
		QModelIndex start_timeindex = FAInfo_model->index(i,3);
		QString start_time = FAInfo_model->data(start_timeindex).toString();

		//结束时间，让用户自己录入的时间，怎么保存到库里

		//TYPE事故原因
		QModelIndex accidentreason_nameindex = FAInfo_model->index(i,4);
		QString accidentreason_name = FAInfo_model->data(accidentreason_nameindex).toString();
		//备注
		QModelIndex note_nameindex = FAInfo_model->index(i,5);
		QString note_name = FAInfo_model->data(note_nameindex).toString();
		
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(accidentreason_name)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

}

void SignOutInformation::secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//这里只能插入新的数据了，不能暴力清空表，重头插，我怎么知道新的数据从哪里开始的？？？

	
	
	int rows = FAInfo_model->rowCount();


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FA_ACCIDENTINFO where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	int hang = rows - items;

	if(hang>0)
	{
		//我这里只想知道写几条
	for (int i=0;i<hang;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//事故信息，汉字char
		QModelIndex accidentinfo_nameindex = FAInfo_model->index(items+i,0);
		QString accidentinfo_name = FAInfo_model->data(accidentinfo_nameindex).toString();
		//qDebug()<<name;
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME发生时间timestamp	  
		QModelIndex start_timeindex = FAInfo_model->index(items+i,3);
		QString start_time = FAInfo_model->data(start_timeindex).toString();

		//结束时间，让用户自己录入的时间，怎么保存到库里

		//TYPE事故原因
		QModelIndex accidentreason_nameindex = FAInfo_model->index(items+i,4);
		QString accidentreason_name = FAInfo_model->data(accidentreason_nameindex).toString();
		//备注
		QModelIndex note_nameindex = FAInfo_model->index(items+i,5);
		QString note_name = FAInfo_model->data(note_nameindex).toString();
		
		
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(accidentreason_name)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}
	}
	

}



void SignOutInformation::firstwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime)
{

	int rows = MaintenancePlan_model->rowCount();
	//我这里只想知道写几条
	for (int i=0;i<rows;i++)
	{
		//station
		QModelIndex stationinfo_nameindex = MaintenancePlan_model->index(i,0);
		QString stationinfo_name = MaintenancePlan_model->data(stationinfo_nameindex).toString();
		
		//feeder
		QModelIndex feederinfo_nameindex = MaintenancePlan_model->index(i,1);
		QString feederinfo_name = MaintenancePlan_model->data(feederinfo_nameindex).toString();
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//停电开始时间timestamp
		QModelIndex poweroff_start_timeindex = MaintenancePlan_model->index(i,4);
		QString poweroff_start_time = MaintenancePlan_model->data(poweroff_start_timeindex).toString();
		

		//停电结束时间
		QModelIndex poweroff_end_timeindex = MaintenancePlan_model->index(i,5);
		QString poweroff_end_time = MaintenancePlan_model->data(poweroff_end_timeindex).toString();


		//停电类型
		QModelIndex type_nameindex = MaintenancePlan_model->index(i,6);
		QString type_name = MaintenancePlan_model->data(type_nameindex).toString();

		//停电原因
		QModelIndex reason_nameindex = MaintenancePlan_model->index(i,7);
		QString reason_name = MaintenancePlan_model->data(reason_nameindex).toString();

		//停电时长（小时）
		QModelIndex hour_nameindex = MaintenancePlan_model->index(i,8);
		int hour_name = MaintenancePlan_model->data(hour_nameindex).toInt();

		//备注
		QModelIndex note_nameindex = MaintenancePlan_model->index(i,9);
		QString note_name = MaintenancePlan_model->data(note_nameindex).toString();

		QString writesqlMaintenancePlanInfo = QString("INSERT INTO H_MAINTENANCE_PLAN (station,feeder,work_day,work_index,time,start_time,end_time,type,reason,powerofftime,note) VALUES ('"\

			+QString("%1").arg(stationinfo_name)+"','"\
			+QString("%2").arg(feederinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+poweroff_start_time+"','yyyy-MM-dd hh24:mi:ss'),"\
			+"to_timestamp('"+poweroff_end_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%8").arg(type_name)+"','"\
			+QString("%9").arg(reason_name)+"',"\
			+QString("%10").arg(hour_name)+",'"\
			+QString("%11").arg(note_name)+"')");




		//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
		//InitConTable::executeSQL(querysqlFAInfo,0);
		InitConTable::executeSQL(writesqlMaintenancePlanInfo,0);

		//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

	}
	////我这里只想知道写几条
	//int rows=ui->maintenanceplan_tableWidget->rowCount();
	//for (int i=0;i<rows;i++)
	//{
	//	QAbstractItemModel *maintenanceplan_model = ui->maintenanceplan_tableWidget->model();

	//	//1变电站
	//	QString station_name = maintenanceplan_model->data(maintenanceplan_model->index(i,0),Qt::DisplayRole).toString();
	//	//2线路名称
	//	QString feeder_name = maintenanceplan_model->data(maintenanceplan_model->index(i,1),Qt::DisplayRole).toString();
	//	//3交接日timestamp
	//	QStringList strstarttimeList = strstarttime.split(" ");
	//	QString strstarttimedate = strstarttimeList[0];
	//	//4交接班序号short
	//	short shift = startshift.toShort();
	//	//5录入时间timestamp(非常重要)
	//	QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
	//	//6停电开始时间
	//	QString startTime = maintenanceplan_model->data(maintenanceplan_model->index(i,4),Qt::DisplayRole).toString(); 
	//	//7停电结束时间
	//	QString endTime = maintenanceplan_model->data(maintenanceplan_model->index(i,5),Qt::DisplayRole).toString();
	//	//8停电类型
	//	QString type = maintenanceplan_model->data(maintenanceplan_model->index(i,6),Qt::DisplayRole).toString();
	//	//9停电原因
	//	QString reason = maintenanceplan_model->data(maintenanceplan_model->index(i,7),Qt::DisplayRole).toString();
	//	//10停电时长
	//	QString hour = maintenanceplan_model->data(maintenanceplan_model->index(i,8),Qt::DisplayRole).toString();
	//	//11备注
	//	QString note_name = maintenanceplan_model->data(maintenanceplan_model->index(i,9),Qt::DisplayRole).toString();
	//	QString writesqlmaintenanceplan = QString("INSERT INTO H_MAINTENANCE_PLAN (station,feeder,work_day,work_index,time,start_time,end_time,type,reason,powerofftime,note) VALUES ('%1','%2',to_timestamp('%3','yyyy-MM-dd'),%4,to_timestamp('%5','yyyy-MM-dd hh24:mi:ss'),to_timestamp('%6','yyyy-MM-dd hh24:mi:ss'),to_timestamp('%7','yyyy-MM-dd hh24:mi:ss'),'%8','%9','%10','%11')").arg(station_name).arg(feeder_name).arg(strstarttimedate).arg(shift).arg(strinformationcurDateTime).arg(startTime).arg(endTime).arg(type).arg(reason).arg(hour).arg(note_name);
	//	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
	//	//InitConTable::executeSQL(querysqlFAInfo,0);
	//	InitConTable::executeSQL(writesqlmaintenanceplan,0);

	//	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

	//}

}

void SignOutInformation::secondwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime)
{

	//这里只能插入新的数据了，不能暴力清空表，重头插，我怎么知道新的数据从哪里开始的？？？


	int rows = MaintenancePlan_model->rowCount();
	//获取上一次录入时间，查询老表的时候就以这个时间为左时间


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_MAINTENANCE_PLAN where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);





	int hang = rows - items;

	if(hang>0)
	{//我这里只想知道写几条
		for (int i=0;i<hang;i++)
		{

			//station
			QModelIndex stationinfo_nameindex = MaintenancePlan_model->index(items+i,0);
			QString stationinfo_name = MaintenancePlan_model->data(stationinfo_nameindex).toString();

			//feeder
			QModelIndex feederinfo_nameindex = MaintenancePlan_model->index(items+i,1);
			QString feederinfo_name = MaintenancePlan_model->data(feederinfo_nameindex).toString();
			//交接日timestamp
			QStringList strstarttimeList = strstarttime.split(" ");
			QString strstarttimedate = strstarttimeList[0];
			//交接班序号short
			short shift = startshift.toShort();
			//录入时间timestamp(非常重要)
			QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
			//停电开始时间timestamp
			QModelIndex poweroff_start_timeindex = MaintenancePlan_model->index(items+i,4);
			QString poweroff_start_time = MaintenancePlan_model->data(poweroff_start_timeindex).toString();


			//停电结束时间
			QModelIndex poweroff_end_timeindex = MaintenancePlan_model->index(items+i,5);
			QString poweroff_end_time = MaintenancePlan_model->data(poweroff_end_timeindex).toString();


			//停电类型
			QModelIndex type_nameindex = MaintenancePlan_model->index(items+i,6);
			QString type_name = MaintenancePlan_model->data(type_nameindex).toString();

			//停电原因
			QModelIndex reason_nameindex = MaintenancePlan_model->index(items+i,7);
			QString reason_name = MaintenancePlan_model->data(reason_nameindex).toString();

			//停电时长（小时）
			QModelIndex hour_nameindex = MaintenancePlan_model->index(items+i,8);
			int hour_name = MaintenancePlan_model->data(hour_nameindex).toInt();

			//备注
			QModelIndex note_nameindex = MaintenancePlan_model->index(items+i,9);
			QString note_name = MaintenancePlan_model->data(note_nameindex).toString();
			


			QString writesqlMaintenancePlanInfo = QString("INSERT INTO H_MAINTENANCE_PLAN (station,feeder,work_day,work_index,time,start_time,end_time,type,reason,powerofftime,note) VALUES ('"\

				+QString("%1").arg(stationinfo_name)+"','"\
				+QString("%2").arg(feederinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+poweroff_start_time+"','yyyy-MM-dd hh24:mi:ss'),"\
				+"to_timestamp('"+poweroff_end_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



				+QString("%8").arg(type_name)+"','"\
				+QString("%9").arg(reason_name)+"',"\
				+QString("%10").arg(hour_name)+",'"\
				+QString("%11").arg(note_name)+"')");

			//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
			//InitConTable::executeSQL(querysqlFAInfo,0);
			InitConTable::executeSQL(writesqlMaintenancePlanInfo,0);

			//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

		}




	}



}




void SignOutInformation::firstwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{
	int rows = DPCPoint_model->rowCount();
	//我这里只想知道写几条
	for (int i=0;i<rows;i++)
	{
		//区域,ObId
		QModelIndex index0 = DPCPoint_model->index(i,0);
		QString area_name = DPCPoint_model->data(index0).toString();
		//变电站
		QModelIndex index1 = DPCPoint_model->index(i,1);
		QString station_name = DPCPoint_model->data(index1).toString(); 
		//线路名称
		QModelIndex index2 = DPCPoint_model->index(i,2);
		QString feeder_name = DPCPoint_model->data(index2).toString();
		//开关名称
		QModelIndex index3 = DPCPoint_model->index(i,3);
		QString switch_name = DPCPoint_model->data(index3).toString();
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//操作内容
		QModelIndex index4 = DPCPoint_model->index(i,6);
		QString opt_content = DPCPoint_model->data(index4).toString();
		//操作时间
		QModelIndex index5 = DPCPoint_model->index(i,7);
		QString opt_time = DPCPoint_model->data(index5).toString();
		//操作人
		QModelIndex index6 = DPCPoint_model->index(i,8);
		QString operatorinfo= DPCPoint_model->data(index6).toString();
		//监护人
		QModelIndex index7 = DPCPoint_model->index(i,9);
		QString guarderinfo = DPCPoint_model->data(index7).toString();
		//备注
		QModelIndex index8 = DPCPoint_model->index(i,10);
		QString note_name = DPCPoint_model->data(index8).toString();
		//为什么只写入了一条，难道是数据库表抽风？
		QString writesqldpcoptrecord = QString("INSERT INTO H_DPC_OPTRECORD (area,station,feeder,switch,work_day,work_index,time,opt_content,opt_time,operatorinfo,guarderinfo,note) VALUES ('"\

			+QString("%1").arg(area_name)+"','"\
			+QString("%2").arg(station_name)+"','"\
			+QString("%3").arg(feeder_name)+"','"\
			+QString("%4").arg(switch_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\
			+QString("%5").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),'"\
			+QString("%6").arg(opt_content)+"',to_timestamp('"+opt_time+"','yyyy-MM-dd hh24:mi:ss'),'"\

			+QString("%7").arg(operatorinfo)+"','"\
			+QString("%8").arg(guarderinfo)+"','"\
			+QString("%9").arg(note_name)+"')");

		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqldpcoptrecord,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);
	
}

	

}


void SignOutInformation::secondwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//这里只能插入新的数据了，不能暴力清空表，重头插，我怎么知道新的数据从哪里开始的？？？

	
	int rows = DPCPoint_model->rowCount();
	//获取上一次录入时间，查询老表的时候就以这个时间为左时间

	
	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_DPC_OPTRECORD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	
	int hang = rows - items;

	if(hang>0)
	{

	//我这里只想知道写几条
	for (int i=0;i<hang;i++)
	{

		//区域
		QModelIndex area_nameindex = DPCPoint_model->index(items+i,0);
		QString area_name = DPCPoint_model->data(area_nameindex).toString();
		//变电站
		QModelIndex station_nameindex = DPCPoint_model->index(items+i,1);
		QString station_name = DPCPoint_model->data(station_nameindex).toString();
		//线路名称
		QModelIndex feeder_nameindex = DPCPoint_model->index(items+i,2);
		QString feeder_name = DPCPoint_model->data(feeder_nameindex).toString();
		//开关名称
		QModelIndex switch_nameindex = DPCPoint_model->index(items+i,3);
		QString switch_name = DPCPoint_model->data(switch_nameindex).toString();
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//操作内容
		QModelIndex opt_contentindex = DPCPoint_model->index(items+i,6);
		QString opt_content = DPCPoint_model->data(opt_contentindex).toString();
		//操作时间

		QModelIndex opt_timeindex = DPCPoint_model->index(items+i,7);
		QString opt_time = DPCPoint_model->data(opt_timeindex).toString();
		//操作人
		QModelIndex operatorinfoindex = DPCPoint_model->index(items+i,8);
		QString operatorinfo= DPCPoint_model->data(operatorinfoindex).toString();
		//监护人
		QModelIndex guarderinfoindex = DPCPoint_model->index(items+i,9);
		QString guarderinfo = DPCPoint_model->data(guarderinfoindex).toString();
		//备注
		QModelIndex note_nameindex = DPCPoint_model->index(items+i,10);
		QString note_name = DPCPoint_model->data(note_nameindex).toString();
		
		
		QString writesqldpcoptrecord = QString("INSERT INTO H_DPC_OPTRECORD (area,station,feeder,switch,work_day,work_index,time,opt_content,opt_time,operatorinfo,guarderinfo,note) VALUES ('"\

			+QString("%1").arg(area_name)+"','"\
			+QString("%2").arg(station_name)+"','"\
			+QString("%3").arg(feeder_name)+"','"\
			+QString("%4").arg(switch_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\
			+QString("%5").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),'"\
			+QString("%6").arg(opt_content)+"',to_timestamp('"+opt_time+"','yyyy-MM-dd hh24:mi:ss'),'"\

			+QString("%7").arg(operatorinfo)+"','"\
			+QString("%8").arg(guarderinfo)+"','"\
			+QString("%9").arg(note_name)+"')");

		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqldpcoptrecord,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	


}
}













void SignOutInformation::firstwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	int rows = FeederOverload_model->rowCount();
	//我这里只想知道写几条
	for (int i=0;i<rows;i++)
	{
		//区域
		QModelIndex index0 = FeederOverload_model->index(i,0);
		QString area_name = FeederOverload_model->data(index0).toString();
		//变电站
		QModelIndex index1 = FeederOverload_model->index(i,1);
		QString station_name = FeederOverload_model->data(index1).toString();
		//线路名称
		QModelIndex index2 = FeederOverload_model->index(i,2);
		QString feeder_name = FeederOverload_model->data(index2).toString();
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 

		//额定电流
		QModelIndex index4 = FeederOverload_model->index(i,5);
		QString amprating = FeederOverload_model->data(index4).toString();

		//最大电流

		QModelIndex index5 = FeederOverload_model->index(i,6);
		QString max_current = FeederOverload_model->data(index5).toString();
		//出现时间
		QModelIndex index6 = FeederOverload_model->index(i,7);
		QString occur_time = FeederOverload_model->data(index6).toString();

		//重载率
		QModelIndex index7 = FeederOverload_model->index(i,8);
		QString overloadrate = FeederOverload_model->data(index7).toString();


		//备注
		QModelIndex index8 = FeederOverload_model->index(i,9);
		QString note_name = FeederOverload_model->data(index8).toString();
		
		QString writesqlfeederoverload = QString("INSERT INTO H_FEEDER_OVERLOAD (area,station,feeder,work_day,work_index,time,amprating,max_current,occur_time,overloadrate,note) VALUES ('"\

			+QString("%1").arg(area_name)+"','"\
			+QString("%2").arg(station_name)+"','"\
			+QString("%3").arg(feeder_name)+"',"\
			+"to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\
			+QString("%5").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),'"\
			+QString("%7").arg(amprating)+"','"\
			+QString("%8").arg(max_current)+"',to_timestamp('"+occur_time+"','yyyy-MM-dd hh24:mi:ss'),'"\

			
			+QString("%10").arg(overloadrate)+"','"\
			+QString("%11").arg(note_name)+"')");

		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlfeederoverload,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);
	
}

	

}


void SignOutInformation::secondwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//这里只能插入新的数据了，不能暴力清空表，重头插，我怎么知道新的数据从哪里开始的？？？

	
	int rows = FeederOverload_model->rowCount();
	//获取上一次录入时间，查询老表的时候就以这个时间为左时间

	


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FEEDER_OVERLOAD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);//后面写入的时候要用到
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	int hang = rows - items;

	if(hang>0)
	{

	//我这里只想知道写几条
	for (int i=0;i<hang;i++)
	{
		//区域
		QModelIndex area_nameindex = FeederOverload_model->index(items+i,0);
		QString area_name = FeederOverload_model->data(area_nameindex).toString();
		//变电站
		QModelIndex station_nameindex = FeederOverload_model->index(items+i,1);
		QString station_name = FeederOverload_model->data(area_nameindex).toString();
		//线路名称
		QModelIndex feeder_nameindex = FeederOverload_model->index(items+i,2);
		QString feeder_name = FeederOverload_model->data(feeder_nameindex).toString();
		//交接日timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//交接班序号short
		short shift = startshift.toShort();
		//录入时间timestamp(非常重要)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//额定电流
		QModelIndex ampratingindex = FeederOverload_model->index(items+i,5);
		QString amprating = FeederOverload_model->data(ampratingindex).toString();
		//最大电流
		QModelIndex max_currentindex = FeederOverload_model->index(items+i,6);
		QString max_current = FeederOverload_model->data(max_currentindex).toString();
		//出现时间
		QModelIndex occur_timeindex = FeederOverload_model->index(items+i,7);
		QString occur_time = FeederOverload_model->data(occur_timeindex).toString();
		//重载率
		QModelIndex overloadrateindex = FeederOverload_model->index(items+i,8);
		QString overloadrate = FeederOverload_model->data(overloadrateindex).toString();
		//备注
		QModelIndex note_nameindex = FeederOverload_model->index(items+i,9);
		QString note_name = FeederOverload_model->data(note_nameindex).toString();




		
		QString writesqlfeederoverload = QString("INSERT INTO H_FEEDER_OVERLOAD (area,station,feeder,work_day,work_index,time,amprating,max_current,occur_time,overloadrate,note) VALUES ('"\

			+QString("%1").arg(area_name)+"','"\
			+QString("%2").arg(station_name)+"','"\
			+QString("%3").arg(feeder_name)+"',"\
			+"to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\
			+QString("%5").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),'"\
			+QString("%7").arg(amprating)+"','"\
			+QString("%8").arg(max_current)+"',to_timestamp('"+occur_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


			+QString("%10").arg(overloadrate)+"','"\
			+QString("%11").arg(note_name)+"')");

		
	//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlfeederoverload,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	


}
}



void SignOutInformation::secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
{
	FAInfo_model = new QStandardItemModel(ui->accidentinfo_tableview);//新的一个model
	FAInfo_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("accident_info")));
	FAInfo_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("work_day")));
	FAInfo_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_index")));
	FAInfo_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("start_time")));
	//FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("end_time")));
	FAInfo_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("reason")));
	//FAInfo_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("acker")));
	FAInfo_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("note")));
	ui->accidentinfo_tableview->setModel(FAInfo_model);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);//事故原因
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);//备注
	

	//从新表里面去查啦，哈哈哈
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO where START_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by START_TIME";
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
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') AND TYPE =1 order by MESSAGETIME";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer2;i++)
	{
		//FEEDEROBJECTID事故信息
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID父
		StringData feederiddata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederiddata);
		OMString strfeederid = (OMString)feederiddata;
		QString qstrfeederid = QString::fromUtf8(strfeederid.c_str());
		QStandardItem* newitem1 = new QStandardItem(qstrfeederid);

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
		ObId sonoffeederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID子
		short feedertype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData sonoffeederiddata;
		ToolUtil::databaseRead(sonoffeederid, AT_Name, &sonoffeederiddata);
		OMString strsonoffeederid = (OMString)sonoffeederiddata;
		QString qstrsonoffeederid = QString::fromUtf8(strsonoffeederid.c_str());
		
		QStandardItem* newitem6 = new QStandardItem(qstrsonoffeederid);

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



void SignOutInformation::secondshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
{
	DPCPoint_model = new QStandardItemModel(ui->DPCPoint_tableview);//新的一个model
	DPCPoint_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("area")));
	DPCPoint_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("station")));
	DPCPoint_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("feeder")));
	DPCPoint_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("switch")));
	DPCPoint_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_day")));
	DPCPoint_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("work_index")));
	DPCPoint_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("opt_content")));
	DPCPoint_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("opt_time")));
	DPCPoint_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("operatorinfo")));
	DPCPoint_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("guarderinfo")));
	DPCPoint_model->setHorizontalHeaderItem(10, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableView平均分配列宽
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

	//从新表里面去查啦，哈哈哈
	RECORDSETHANDLE secondgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlDPCPoint = "select area,station,feeder,switch,work_day,work_index,opt_content,opt_time,operatorinfo,guarderinfo,note from H_DPC_OPTRECORD where OPT_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OPT_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherDPCPointSetHandle,secondsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//区域ObId转name
		std::string strsubcontrolareafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//变电站ObId转name
		std::string strpmsstationdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//线路ObId转name
		std::string strfeederdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,2);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		//开关名称ObId转name
		std::string strswitchdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,2);
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strswitchdatafromold.c_str()));
		item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item4->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY交接日	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle,i,4);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item5 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX交接班序号
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,5);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(strstartshift.c_str()));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);

		//录入时间

		
		//opt_content操作内容
		std::string opt_content_info = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,6);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(opt_content_info.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 


		//opt_time操作时间
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle,i,7);
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* item8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		
		//操作人
		std::string stdopertor = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,8);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(stdopertor.c_str()));
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 

		//监护人
		std::string stdguarder = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,9);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(stdguarder.c_str()));
		item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item10->setForeground(QBrush(QColor(0, 255, 0))); 
		

		
		//NOTE备注
		std::string stdnote = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,10);
		QStandardItem* item11 = new QStandardItem(QString::fromUtf8(stdnote.c_str()));
		item11->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item11->setForeground(QBrush(QColor(0, 255, 0))); 
		


		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1  << item2 <<item3<<item4 <<item5 <<item6  <<item7   <<item8 <<item9<<item10 <<item11;
		DPCPoint_model->appendRow(item);


	}

	
	//获取上一次录入时间，查询老表的时候就以这个时间为左时间
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle, rowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//多一秒
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherDPCPointSetHandle);


	//再从老表里面去查，记得卡时间，左时间是上一次录入时间，右时间是这一次点击okbutton时间,追加数据

	

	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer2;i++)
	{
		//找到遥控
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,0);
		//找到关联遥测
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//找到psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;



		//区域
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* newitem1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//变电站
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* newitem2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));



		//线路名称
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData feederdata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));










		//开关名称,obid递归向上查询



		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData switchdata;
		ToolUtil::databaseRead(switchid, AT_Name, &switchdata);
		OMString strswitchdata = (OMString)switchdata;
		QStandardItem* newitem4 = new QStandardItem(QString::fromUtf8(strswitchdata.c_str()));

		//交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem5 = new QStandardItem(strstarttimeList[0]);

		//交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem6 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


		//录入时间


		//操作内容
		QStandardItem* newitem7;
		int dpcAction = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,1);
		//QString dpcAction = QString::number(intdpcAction);
		int actionResult = (int)CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,3);
		//QString actionResult = QString::number(intactionResult);

		if ((dpcAction== 0) && (actionResult == 0))
		{	
			//QString optcontent = "choicesuccess";
			newitem7 = new QStandardItem(QObject::tr("choicesuccess"));

		}
		else if ((dpcAction== 0)&&( actionResult == 1))
		{
			//QString optcontent = "choicefail";
			newitem7 = new QStandardItem(QObject::tr("choicefail"));
		}

		else if ((dpcAction== 0)&&( actionResult == 2))
		{
			//QString optcontent = "choiceovertime";
			newitem7 = new QStandardItem(QObject::tr("choiceovertime"));
		}	

		else if ((dpcAction== 1)&&( actionResult == 0))
		{
			//QString optcontent = "actionsuccess";
			newitem7 = new QStandardItem(QObject::tr("actionsuccess"));
		}	

		else if ((dpcAction== 1)&&( actionResult == 1))
		{
			//QString optcontent = "actionfail";
			newitem7 = new QStandardItem(QObject::tr("actionfail"));
		}	

		else if ((dpcAction== 1)&&( actionResult == 2))
		{
			//QString optcontent = "actionovertime";
			newitem7 = new QStandardItem(QObject::tr("actionovertime"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 0))
		{
			//QString optcontent = "cancelsuccess";
			newitem7 = new QStandardItem(QObject::tr("cancelsuccess"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 1))
		{
			//QString optcontent = "cancelfail";
			newitem7 = new QStandardItem(QObject::tr("cancelfail"));
		}	
		else if ((dpcAction== 2)&&( actionResult == 2))
		{
			QString optcontent = "cancelovertime";
			newitem7 = new QStandardItem(QObject::tr("cancelovertime"));
		}	
		else if ((dpcAction== 3)&&( actionResult == 0))
		{
			QString optcontent = "communicationbreakdown";
			newitem7 = new QStandardItem(QObject::tr("communicationbreakdown"));
		}	
		else if ((dpcAction== 4)&&( actionResult == 0))
		{
			QString optcontent = "controlconflict";
			newitem7 = new QStandardItem(QObject::tr("controlconflict"));
		}	
		else if ((dpcAction== 5)&&( actionResult == 0))
		{
			QString optcontent = "missionovertime";
			newitem7 = new QStandardItem(QObject::tr("missionovertime"));
		}	
		else if ((dpcAction== 6)&&( actionResult == 0))
		{
			QString optcontent = "transfersuccess";
			newitem7 = new QStandardItem(QObject::tr("transfersuccess"));
		}	
		else if ((dpcAction== 6)&&( actionResult == 1))
		{
			QString optcontent = "transferfail";
			newitem7 = new QStandardItem(QObject::tr("transferfail"));
		}	
		else if ((dpcAction== 7)&&( actionResult == 0))
		{
			QString optcontent = "haltsuccess";
			newitem7 = new QStandardItem(QObject::tr("haltsuccess"));
		}
		else if ((dpcAction== 7)&&( actionResult == 1))
		{
			QString optcontent = "haltfail";
			newitem7 = new QStandardItem(QObject::tr("haltfail"));
		}
		else if ((dpcAction== 8)&&( actionResult == 0))
		{
			QString optcontent = "choicestart";
			newitem7 = new QStandardItem(QObject::tr("choicestart"));
		}
		else if ((dpcAction== 9)&&( actionResult == 0))
		{
			QString optcontent = "actionstart";
			newitem7 = new QStandardItem(QObject::tr("actionstart"));
		}
		else if((dpcAction== 10)&&( actionResult == 0))
		{
			QString optcontent = "cancelstart";
			newitem7 = new QStandardItem(QObject::tr("cancelstart"));
		}




		//QStandardItem* item7 = new QStandardItem(QObject::tr(""));

		//操作时间
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(firstgatherDPCPointSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* newitem8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));

		//操作人
		std::string operatorinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,4);
		QStandardItem* newitem9 = new QStandardItem(QString::fromUtf8(operatorinfo.c_str()));

		//监护人
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,5);
		QStandardItem* newitem10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//备注
		QStandardItem* newitem11 = new QStandardItem(QObject::tr(""));





		QList<QStandardItem*> newitem;
		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

		newitem << newitem1 << newitem2 <<newitem3 <<newitem4 <<newitem5 <<newitem6 <<newitem7 <<newitem8<<newitem9<<newitem10<<newitem11;

		DPCPoint_model->appendRow(newitem);


	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);

	ui->label_6->setText(QString::number(getsuccessTimes(strstarttime,strendtime,startshift)));
	ui->label_2->setText(QString::number(getfailTimes(strstarttime,strendtime,startshift)));
	
}



void SignOutInformation::secondshowFeederOverload(QString strstarttime, QString strendtime, QString startshift)
{


	FeederOverload_model = new QStandardItemModel(ui->lineload_tableview);//新的一个model
	FeederOverload_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("area")));
	FeederOverload_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("station")));
	FeederOverload_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("feeder")));
	FeederOverload_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_day")));
	FeederOverload_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("work_index")));
	FeederOverload_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("amprating")));
	FeederOverload_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("max_current")));
	FeederOverload_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("occur_time")));
	FeederOverload_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("overloadrate")));
	FeederOverload_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->lineload_tableview->setModel(FeederOverload_model);
	//QTableView平均分配列宽
	ui->lineload_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 





	//从新表里面去查啦，哈哈哈
	RECORDSETHANDLE secondgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFeederOverload = "select area,station,feeder,work_day,work_index,amprating,max_current,occur_time,overloadrate,note from H_FEEDER_OVERLOAD where OCCUR_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OCCUR_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//区域ObId转name
		std::string strsubcontrolareafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//变电站ObId转name
		std::string strpmsstationdatafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//线路ObId转name
		std::string strfeederdatafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,2);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		

		//WORK_DAY交接日	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle,i,3);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item4 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item4->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX交接班序号
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,4);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(strstartshift.c_str()));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);

		//录入时间

		//额定电流

		double amprating = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,5);
		QString stramprating = QString::number(amprating);
		QStandardItem* item6 = new QStandardItem(stramprating);
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 



		//最大电流

		
		double maxcurrent = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,6);
		QString strmaxcurrent = QString::number(maxcurrent);
		QStandardItem* item7 = new QStandardItem(strmaxcurrent);
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 
		


		//出现时间
		ORMTimeStamp occur_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle,i,7);
		QDateTime stroccurtime = SetTimeFromDB(occur_time);
		QStandardItem* item8 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		//重载率
		double overloadrate = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,8);
		QString stroverloadrate = QString::number(overloadrate);
		
		QStandardItem* item9 = new QStandardItem(stroverloadrate);
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 


		//NOTE备注
		std::string stdnote = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,9);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(stdnote.c_str()));
		item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item10->setForeground(QBrush(QColor(0, 255, 0))); 



		



		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1  << item2 <<item3<<item4 <<item5 <<item6   <<item7<<item8 <<item9<<item10;
		FeederOverload_model->appendRow(item);


	}

	//获取上一次录入时间，查询老表的时候就以这个时间为左时间
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle, rowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//多一秒
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFeederOverloadSetHandle);


	//再从老表里面去查，记得卡时间，左时间是上一次录入时间，右时间是这一次点击okbutton时间,追加数据

	RECORDSETHANDLE firstgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();


	QString strMVPointlist="";
	QString strMVPointlist1="";
	QString strMVPointlist2="";

	QString firstsqlFeederOverload;

	QDateTime curDateTime = QDateTime::currentDateTime();
	QString yearmonth = curDateTime.toString("yyyyMM");

	if (MVPointlist.size()<1000)
	{

		for ( int i=0; i!=MVPointlist.size(); ++i )
		{
			strMVPointlist = strMVPointlist.append(MVPointlist[i]).append(",");
		}
		//strMVPointlist.append(MVPointlist[MVPointlist.size()]);
		//qDebug()<<strMVPointlist;
		strMVPointlist.chop(1);
		qDebug()<<strMVPointlist;
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_"+yearmonth+" where SDATE between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+") order by SDATE";



	} 
	else
	{

		for ( int i=0; i<1000; ++i )
		{
			strMVPointlist1 = strMVPointlist1.append(MVPointlist[i]).append(",");
		}
		strMVPointlist1.chop(1);

		for ( int j=1000; j!=MVPointlist.size(); ++j )
		{
			strMVPointlist2 = strMVPointlist2.append(MVPointlist[j]).append(",");
		}
		//strMVPointlist.append(MVPointlist[MVPointlist.size()]);
		//qDebug()<<strMVPointlist;
		strMVPointlist2.chop(1);

		qDebug()<<strMVPointlist1;
		qDebug()<<strMVPointlist2;
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_"+yearmonth+" where SDATE between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


	}

	
	
	
	
	
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherFeederOverloadSetHandle,firstsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer2;i++)
	
	{

		//找到遥控
		ObId mvpointid = CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,0);
		

		//找到psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);
		

		



		//区域
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* newitem1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//变电站
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* newitem2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));



		//线路名称
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		StringData feederdata;
		ToolUtil::databaseRead(MemberOfLineid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));



		//交接日
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem4 = new QStandardItem(strstarttimeList[0]);

		//交接班序号
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem5 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);



		

		
		//录入时间


		//额定电流PMSBreaker
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		FloatData switchampratingdata;
		ToolUtil::databaseRead(switchid, AT_ampRating, &switchampratingdata);
		QStandardItem* newitem6 = new QStandardItem(QString::number(switchampratingdata));

		//最大电流
		int maxcurrent = (int)CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,2);//FAULTTIME
		QStandardItem* newitem7 = new QStandardItem(QString::number(maxcurrent));
		
		//出现时间
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFeederOverloadSetHandle,i,3);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		QStandardItem* newitem8 = new QStandardItem(start_time);


		/*
		//重载率
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherFeederOverloadSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//备注
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));

		*/
		QStandardItem* newitem9 = new QStandardItem(QObject::tr(""));
		QStandardItem* newitem10 = new QStandardItem(QObject::tr(""));



		QList<QStandardItem*> newitem;
		newitem << newitem1 << newitem2 <<newitem3 <<newitem4 <<newitem5 <<newitem6<<newitem7<<newitem8<<newitem9<<newitem10;

		

		double double_switchampratingdata  = switchampratingdata;
		double double_maxcurrent = maxcurrent;
		if(double_switchampratingdata==0.0 || double_maxcurrent==0.0)
			continue;
		if(double_maxcurrent>=double_switchampratingdata*0.75)
		{
			FeederOverload_model->appendRow(newitem);
		}


	}



	CPS_ORM_RsFreeRecordSet(firstgatherFeederOverloadSetHandle);






	
	
	

}



void SignOutInformation::secondshowMaintenancePlan(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift)
{
	MaintenancePlan_model = new QStandardItemModel(ui->maintenanceplan_tableview);//新的一个model
	MaintenancePlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("station")));
	MaintenancePlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("feeder")));
	MaintenancePlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_day")));
	MaintenancePlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_index")));
	MaintenancePlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("poweroffstart_time")));
	MaintenancePlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("poweroffend_time")));
	MaintenancePlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("powerofftype")));
	MaintenancePlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("poweroffreason")));
	MaintenancePlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("powerofftime")));
	MaintenancePlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));

	//利用setModel()方法将数据模型与QTableView绑定
	ui->maintenanceplan_tableview->setModel(MaintenancePlan_model);

	ui->maintenanceplan_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
	//QTableView平均分配列宽
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 
	


	//从新表里面去查啦，哈哈哈
	RECORDSETHANDLE secondgatherMaintenancePlanSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlMaintenancePlanInfo = "select station,feeder, WORK_DAY, WORK_INDEX, TIME, START_TIME, END_TIME, type, REASON, powerofftime, NOTE from H_MAINTENANCE_PLAN where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherMaintenancePlanSetHandle,secondsqlMaintenancePlanInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//变电站信息
		std::string station_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(station_info.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 


		//线路信息
		std::string feeder_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(feeder_info.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY交接日	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle,i,2);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item3 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX交接班序号
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherMaintenancePlanSetHandle,i,3);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strstartshift.c_str()));
		item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item4->setForeground(QBrush(QColor(0, 255, 0))); 
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);

		//录入时间


		//START_TIME发生时间
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 5);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		
		//END_TIME结束时间
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 6);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		

		//type
		std::string type = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,7);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(type.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 

		//reason
		std::string reason = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,8);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(reason.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//ACKER确认人
		std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		*/

		int hour = CPS_ORM_RsGetNumberValue(secondgatherMaintenancePlanSetHandle,i,9);
		QString str_hour = QString::number(hour);
		QStandardItem* item9 = new QStandardItem(str_hour);
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 



		//NOTE备注
		std::string note = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,10);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(note.c_str()));
		item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item10->setForeground(QBrush(QColor(0, 255, 0))); 





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1 << item2 <<item3 <<item4<<item5 <<item6<< item7 <<item8<<item9<<item10;
		MaintenancePlan_model->appendRow(item);


	}

	CPS_ORM_RsFreeRecordSet(secondgatherMaintenancePlanSetHandle);

	//ui->maintenanceplan_tableWidget->setColumnCount(10);//10列
	//ui->maintenanceplan_tableWidget->setHorizontalHeaderLabels(QStringList()<<QObject::tr("station")<<QObject::tr("feeder")<<QObject::tr("work_day")<<QObject::tr("work_index")<<QObject::tr("poweroffstart_time")<<QObject::tr("poweroffend_time")<<QObject::tr("powerofftype")<<QObject::tr("poweroffreason")<<QObject::tr("powerofftime")<<QObject::tr("note"));
	//ui->maintenanceplan_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);



	//QStandardItemModel *maintenanceplan_model = new QStandardItemModel();

	//ui->maintenanceplan_tableWidget->setModel(maintenanceplan_model);

	////从新表里面去查啦，哈哈哈
	//RECORDSETHANDLE secondgatherMaintenancePlanSetHandle = CPS_ORM_RsNewRecordSet();
	//QString secondsqlMaintenancePlanInfo = "select STATION, FEEDER, WORK_DAY, WORK_INDEX, TIME, START_TIME, END_TIME, TYPE, REASON, POWEROFFTIME, NOTE from H_MAINTENANCE_PLAN where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	//int rowsOfTer = CPS_ORM_RsLoadData(secondgatherMaintenancePlanSetHandle,secondsqlMaintenancePlanInfo.toUtf8().data(),dbPoolHandle);
	//for (int i=0;i<rowsOfTer;i++)
	//{
	//	//变电站信息
	//	std::string station_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,0);
	//	QStandardItem* item1 = new QStandardItem(QString::fromUtf8(station_info.c_str()));
	//	item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item1->setForeground(QBrush(QColor(0, 255, 0))); 

	//	
	//		

	//	//线路信息
	//	std::string line_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,1);
	//	QStandardItem* item2 = new QStandardItem(QString::fromUtf8(line_info.c_str()));
	//	item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item2->setForeground(QBrush(QColor(0, 255, 0))); 

	//	//ui->maintenanceplan_tableWidget->setCellWidget(i,1,(QWidget *)item2);

	//	//WORK_DAY交接日	  
	//	ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle,i,2);
	//	QDateTime strwork_day = SetTimeFromDB(work_day);
	//	QStandardItem* item3 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
	//	item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item3->setForeground(QBrush(QColor(0, 255, 0))); 

	//	//ui->maintenanceplan_tableWidget->setCellWidget(i,2,(QWidget *)item3);
	//	//WORK_INDEX交接班序号
	//	int work_index = CPS_ORM_RsGetNumberValue(secondgatherMaintenancePlanSetHandle,i,3);
	//	QString strwork_index = QString::number(work_index);
	//	RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
	//	QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
	//	int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
	//	std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
	//	QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strstartshift.c_str()));
	//	item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item4->setForeground(QBrush(QColor(0, 255, 0))); 
	//	CPS_ORM_RsFreeRecordSet(shiftSetHandle);

	//	//录入时间


	//	//START_TIME发生时间
	//	ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 5);
	//	QDateTime strstarttime = SetTimeFromDB(start_time);
	//	QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item5->setForeground(QBrush(QColor(0, 255, 0))); 


	//	/*
	//	//END_TIME结束时间,系统默认时间
	//	ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
	//	QDateTime strendtime = SetTimeFromDB(end_time);
	//	QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item6->setForeground(QBrush(QColor(0, 255, 0))); 
	//	*/

	//	//END_TIME结束时间
	//	ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 6);
	//	QDateTime strendtime = SetTimeFromDB(end_time);
	//	QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item6->setForeground(QBrush(QColor(0, 255, 0))); 


	//	//事故类型
	//	std::string type = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,7);
	//	QStandardItem* item7 = new QStandardItem(QString::fromUtf8(type.c_str()));
	//	item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item7->setForeground(QBrush(QColor(0, 255, 0))); 



	//	//REASON事故原因
	//	std::string reason = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,8);
	//	QStandardItem* item8 = new QStandardItem(QString::fromUtf8(reason.c_str()));
	//	item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item8->setForeground(QBrush(QColor(0, 255, 0))); 


	//	/*
	//	//ACKER确认人
	//	std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
	//	QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
	//	item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item8->setForeground(QBrush(QColor(0, 255, 0))); 
	//	*/

	//	//REASON事故原因
	//	std::string poweroff_time = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,9);
	//	QStandardItem* item9 = new QStandardItem(QString::fromUtf8(poweroff_time.c_str()));
	//	item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item9->setForeground(QBrush(QColor(0, 255, 0))); 


	//	//NOTE备注
	//	std::string note = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,10);
	//	QStandardItem* item10 = new QStandardItem(QString::fromUtf8(note.c_str()));
	//	item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item10->setForeground(QBrush(QColor(0, 255, 0))); 




	//	
	//	QList<QStandardItem*> item;
	//	//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
	//	item << item1 << item2 <<item3 <<item4 <<item5 <<item6 << item7 <<item8 <<item9<<item10;
	//	ui->maintenanceplan_tableWidget->appendRow(item);

	//}




}




void SignOutInformation::on_btn_Add_clicked()
{

	

	//变电站信息
	QString station_info = "";
	QStandardItem* item1 = new QStandardItem(station_info);

	

	//线路信息
	QString feeder_info = "";
	QStandardItem* item2 = new QStandardItem(feeder_info);

	//交接日
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//当前时间和排班时间存在时间差
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	QDateTime current_date_time =QDateTime::currentDateTime();
	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//往前推若干小时
	QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	QString current_date = current_date_timeminus.toString("yyyy-MM-dd");
	//QDate date = QDate::fromString(current_date,"yyyyMMdd");
	QStandardItem* item3 = new QStandardItem(current_date);

	
	
	//默认交接班次
	Configuration configuration;
	int intDefaultIndex = configuration.whichShift();
	//ui->signinandout_shift->setCurrentIndex(intDefaultIndex);
	RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS where work_index="+QString::number(intDefaultIndex);
	int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	
	std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,0,0);
	QString worknameshift = QString::fromUtf8(workname.c_str());


	
	CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);

	QStandardItem* item4 = new QStandardItem(worknameshift);



	DateDelegate *datedelegate = new DateDelegate;
	ui->maintenanceplan_tableview->setItemDelegateForColumn(4,datedelegate);
	ui->maintenanceplan_tableview->setItemDelegateForColumn(5,datedelegate);
	/*
	
	//停电开始时间
	QString poweroff_start_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	QStandardItem* item5 = new QStandardItem(poweroff_start_time);
	//停电结束时间
	QString poweroff_end_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	QStandardItem* item6 = new QStandardItem(poweroff_end_time);

	*/
	////录入时间

	////FAULTTIME发生时间	  
	//ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
	//QDateTime stroccurtime = SetTimeFromDB(occurtime);
	//QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

	////结束时间,让用户去填写

	////QStandardItem* item5 = new QStandardItem(QObject::tr(""));//结束时间

	////TYPE事故原因
	//ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID子
	//short fatype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
	//StringData data3;
	//ToolUtil::databaseRead(cbid, AT_Name, &data3);
	//OMString str3 = (OMString)data3;
	//QString qstr3 = QString::fromUtf8(str3.c_str());
	//if(fatype == 0)
	//	qstr3.append(QObject::tr("overcurrent"));
	//else
	//	qstr3.append(QObject::tr("deflection"));
	//QStandardItem* item6 = new QStandardItem(qstr3);

	////确认人
	////QStandardItem* item7 = new QStandardItem(QObject::tr(""));
	////备注
	//QStandardItem* item8 = new QStandardItem(QObject::tr(""));

	QList<QStandardItem*> item;
	//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

	//item << item1<< item2 <<item3 <<item4<<item5<<item6;

	item << item1<< item2 <<item3 <<item4;

	MaintenancePlan_model->appendRow(item);
}
void SignOutInformation::on_btn_Del_clicked()
{
	QModelIndex item = ui->maintenanceplan_tableview->currentIndex();
	MaintenancePlan_model->removeRow(item.row());
}













