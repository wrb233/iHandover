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
	 //����һ�У��Լ��޼ƻ���������
	 connect(ui->Add,SIGNAL(clicked()),this,SLOT(on_btn_Add_clicked()));
	 //ɾ��һ�У��Լ��޼ƻ���������
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
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);//�¹�ԭ��
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);//��ע

	RECORDSETHANDLE firstgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') AND TYPE =1 order by MESSAGETIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//FEEDEROBJECTID�¹���Ϣ
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID��
		StringData feederiddata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederiddata);
		OMString strfeederid = (OMString)feederiddata;
		QString qstrfeederid = QString::fromUtf8(strfeederid.c_str());
		QStandardItem* item1 = new QStandardItem(qstrfeederid);

		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item2 = new QStandardItem(strstarttimeList[0]);
		
		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);

		//¼��ʱ��

		//FAULTTIME����ʱ��	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//����ʱ��,Ԥ��
		//QStandardItem* item5 = new QStandardItem(QObject::tr(""));//����ʱ��

		//TYPE�¹�ԭ��
		ObId sonoffeederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//FEEDEROBJECTID��
		short feedertype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData sonoffeederiddata;
		ToolUtil::databaseRead(sonoffeederid, AT_Name, &sonoffeederiddata);
		OMString strsonoffeederid = (OMString)sonoffeederiddata;
		QString qstrsonoffeederid = QString::fromUtf8(strsonoffeederid.c_str());
		//QString qreason = (QString)(QObject::tr("overcurrent"));
		//qstrsonoffeederid.append(QObject::tr("����"));
		QStandardItem* item6 = new QStandardItem(qstrsonoffeederid);
		//ȷ����
		//QStandardItem* item7 = new QStandardItem(QObject::tr(""));
		//��ע
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

	//����setModel()����������ģ����QTableView��
	ui->maintenanceplan_tableview->setModel(MaintenancePlan_model);

	ui->maintenanceplan_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
	//QTableViewƽ�������п�
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
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

	//����setModel()����������ģ����QTableView��
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableViewƽ�������п�
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
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
		//�ҵ�ң��
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,0);
		//�ҵ�����ң��
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;
		//1����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//2���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));
		//3��·����
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData feederdata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));
		//4��������,obid�ݹ����ϲ�ѯ
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData switchdata;
		ToolUtil::databaseRead(switchid, AT_Name, &switchdata);
		OMString strswitchdata = (OMString)switchdata;
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strswitchdata.c_str()));

		//5������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item5 = new QStandardItem(strstarttimeList[0]);

		//6���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


		//¼��ʱ��


		//7��������
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

		//8����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(firstgatherDPCPointSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* item8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));
		
		//9������
		std::string operatorinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,4);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(operatorinfo.c_str()));
		
		//10�໤��
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));
		
		//11��ע
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
	//ʼ�ջ�ȡ�ϱ�������ҵ��ĳɹ���ʧ�ܴ���
	successTimes = 0;
	//failTimes = 0;
	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);

	for (int i=0;i<rowsOfTer;i++)
	{
		//7��������
		
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
	//ʼ�ջ�ȡ�ϱ�������ҵ��ĳɹ���ʧ�ܴ���
	//successTimes = 0;
	failTimes = 0;
	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);

	for (int i=0;i<rowsOfTer;i++)
	{
		//7��������

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

	//����setModel()����������ģ����QTableView��
	ui->lineload_tableview->setModel(FeederOverload_model);
	//QTableViewƽ�������п�
	ui->lineload_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
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
		//�ҵ�ң��
		ObId mvpointid = CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,0);

		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);

		//1����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//2���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));
		//3��·����
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		StringData feederdata;
		ToolUtil::databaseRead(MemberOfLineid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));
		//4������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item4 = new QStandardItem(strstarttimeList[0]);
		//5���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
		
		//¼��ʱ��
		//6�����PMSBreaker
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		FloatData switchampratingdata;
		ToolUtil::databaseRead(switchid, AT_ampRating, &switchampratingdata);
		QStandardItem* item6 = new QStandardItem(QString::number(switchampratingdata));
		//7������
		int maxcurrent = (int)CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,2);
		QStandardItem* item7 = new QStandardItem(QString::number(maxcurrent));
		//8����ʱ��
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFeederOverloadSetHandle,i,3);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item8 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));
		/*
		//������
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherFeederOverloadSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//��ע
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));

		*/
		//9������
		QStandardItem* item9 = new QStandardItem(QObject::tr(""));
		//10��ע
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



//���ϱ��������һ�죬��һ����ε���Ϣ�������е��ֶθ��������



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
	
	//¼��ʱ��
	informationcurDateTime=QDateTime::currentDateTime();






}



//д����Ҫ�ĸ�������strstarttime�����գ�strendtime����ʱ�䣬startshift���Ӱ�Σ�informationcurDateTime¼��ʱ�䣨�ǳ���Ҫ��



void SignOutInformation::firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	int rows = FAInfo_model->rowCount();
	//������ֻ��֪��д����
	for (int i=0;i<rows;i++)
	{
		//�¹���Ϣ������char
		QModelIndex accidentinfo_nameindex = FAInfo_model->index(i,0);
		QString accidentinfo_name = FAInfo_model->data(accidentinfo_nameindex).toString();
		//qDebug()<<name;
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME����ʱ��timestamp
		QModelIndex start_timeindex = FAInfo_model->index(i,3);
		QString start_time = FAInfo_model->data(start_timeindex).toString();

		//����ʱ�䣬���û��Լ�¼���ʱ�䣬��ô���浽����

		//TYPE�¹�ԭ��
		QModelIndex accidentreason_nameindex = FAInfo_model->index(i,4);
		QString accidentreason_name = FAInfo_model->data(accidentreason_nameindex).toString();
		//��ע
		QModelIndex note_nameindex = FAInfo_model->index(i,5);
		QString note_name = FAInfo_model->data(note_nameindex).toString();
		
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(accidentreason_name)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

}

void SignOutInformation::secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	
	int rows = FAInfo_model->rowCount();


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FA_ACCIDENTINFO where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	int hang = rows - items;

	if(hang>0)
	{
		//������ֻ��֪��д����
	for (int i=0;i<hang;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//�¹���Ϣ������char
		QModelIndex accidentinfo_nameindex = FAInfo_model->index(items+i,0);
		QString accidentinfo_name = FAInfo_model->data(accidentinfo_nameindex).toString();
		//qDebug()<<name;
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME����ʱ��timestamp	  
		QModelIndex start_timeindex = FAInfo_model->index(items+i,3);
		QString start_time = FAInfo_model->data(start_timeindex).toString();

		//����ʱ�䣬���û��Լ�¼���ʱ�䣬��ô���浽����

		//TYPE�¹�ԭ��
		QModelIndex accidentreason_nameindex = FAInfo_model->index(items+i,4);
		QString accidentreason_name = FAInfo_model->data(accidentreason_nameindex).toString();
		//��ע
		QModelIndex note_nameindex = FAInfo_model->index(items+i,5);
		QString note_name = FAInfo_model->data(note_nameindex).toString();
		
		
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,note) VALUES ('"\

			+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

			+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

			+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\



			+QString("%3").arg(accidentreason_name)+"','"\
			+QString("%5").arg(note_name)+"')");



		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}
	}
	

}



void SignOutInformation::firstwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime)
{

	int rows = MaintenancePlan_model->rowCount();
	//������ֻ��֪��д����
	for (int i=0;i<rows;i++)
	{
		//station
		QModelIndex stationinfo_nameindex = MaintenancePlan_model->index(i,0);
		QString stationinfo_name = MaintenancePlan_model->data(stationinfo_nameindex).toString();
		
		//feeder
		QModelIndex feederinfo_nameindex = MaintenancePlan_model->index(i,1);
		QString feederinfo_name = MaintenancePlan_model->data(feederinfo_nameindex).toString();
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//ͣ�翪ʼʱ��timestamp
		QModelIndex poweroff_start_timeindex = MaintenancePlan_model->index(i,4);
		QString poweroff_start_time = MaintenancePlan_model->data(poweroff_start_timeindex).toString();
		

		//ͣ�����ʱ��
		QModelIndex poweroff_end_timeindex = MaintenancePlan_model->index(i,5);
		QString poweroff_end_time = MaintenancePlan_model->data(poweroff_end_timeindex).toString();


		//ͣ������
		QModelIndex type_nameindex = MaintenancePlan_model->index(i,6);
		QString type_name = MaintenancePlan_model->data(type_nameindex).toString();

		//ͣ��ԭ��
		QModelIndex reason_nameindex = MaintenancePlan_model->index(i,7);
		QString reason_name = MaintenancePlan_model->data(reason_nameindex).toString();

		//ͣ��ʱ����Сʱ��
		QModelIndex hour_nameindex = MaintenancePlan_model->index(i,8);
		int hour_name = MaintenancePlan_model->data(hour_nameindex).toInt();

		//��ע
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




		//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
		//InitConTable::executeSQL(querysqlFAInfo,0);
		InitConTable::executeSQL(writesqlMaintenancePlanInfo,0);

		//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

	}
	////������ֻ��֪��д����
	//int rows=ui->maintenanceplan_tableWidget->rowCount();
	//for (int i=0;i<rows;i++)
	//{
	//	QAbstractItemModel *maintenanceplan_model = ui->maintenanceplan_tableWidget->model();

	//	//1���վ
	//	QString station_name = maintenanceplan_model->data(maintenanceplan_model->index(i,0),Qt::DisplayRole).toString();
	//	//2��·����
	//	QString feeder_name = maintenanceplan_model->data(maintenanceplan_model->index(i,1),Qt::DisplayRole).toString();
	//	//3������timestamp
	//	QStringList strstarttimeList = strstarttime.split(" ");
	//	QString strstarttimedate = strstarttimeList[0];
	//	//4���Ӱ����short
	//	short shift = startshift.toShort();
	//	//5¼��ʱ��timestamp(�ǳ���Ҫ)
	//	QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
	//	//6ͣ�翪ʼʱ��
	//	QString startTime = maintenanceplan_model->data(maintenanceplan_model->index(i,4),Qt::DisplayRole).toString(); 
	//	//7ͣ�����ʱ��
	//	QString endTime = maintenanceplan_model->data(maintenanceplan_model->index(i,5),Qt::DisplayRole).toString();
	//	//8ͣ������
	//	QString type = maintenanceplan_model->data(maintenanceplan_model->index(i,6),Qt::DisplayRole).toString();
	//	//9ͣ��ԭ��
	//	QString reason = maintenanceplan_model->data(maintenanceplan_model->index(i,7),Qt::DisplayRole).toString();
	//	//10ͣ��ʱ��
	//	QString hour = maintenanceplan_model->data(maintenanceplan_model->index(i,8),Qt::DisplayRole).toString();
	//	//11��ע
	//	QString note_name = maintenanceplan_model->data(maintenanceplan_model->index(i,9),Qt::DisplayRole).toString();
	//	QString writesqlmaintenanceplan = QString("INSERT INTO H_MAINTENANCE_PLAN (station,feeder,work_day,work_index,time,start_time,end_time,type,reason,powerofftime,note) VALUES ('%1','%2',to_timestamp('%3','yyyy-MM-dd'),%4,to_timestamp('%5','yyyy-MM-dd hh24:mi:ss'),to_timestamp('%6','yyyy-MM-dd hh24:mi:ss'),to_timestamp('%7','yyyy-MM-dd hh24:mi:ss'),'%8','%9','%10','%11')").arg(station_name).arg(feeder_name).arg(strstarttimedate).arg(shift).arg(strinformationcurDateTime).arg(startTime).arg(endTime).arg(type).arg(reason).arg(hour).arg(note_name);
	//	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
	//	//InitConTable::executeSQL(querysqlFAInfo,0);
	//	InitConTable::executeSQL(writesqlmaintenanceplan,0);

	//	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

	//}

}

void SignOutInformation::secondwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����


	int rows = MaintenancePlan_model->rowCount();
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_MAINTENANCE_PLAN where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);





	int hang = rows - items;

	if(hang>0)
	{//������ֻ��֪��д����
		for (int i=0;i<hang;i++)
		{

			//station
			QModelIndex stationinfo_nameindex = MaintenancePlan_model->index(items+i,0);
			QString stationinfo_name = MaintenancePlan_model->data(stationinfo_nameindex).toString();

			//feeder
			QModelIndex feederinfo_nameindex = MaintenancePlan_model->index(items+i,1);
			QString feederinfo_name = MaintenancePlan_model->data(feederinfo_nameindex).toString();
			//������timestamp
			QStringList strstarttimeList = strstarttime.split(" ");
			QString strstarttimedate = strstarttimeList[0];
			//���Ӱ����short
			short shift = startshift.toShort();
			//¼��ʱ��timestamp(�ǳ���Ҫ)
			QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
			//ͣ�翪ʼʱ��timestamp
			QModelIndex poweroff_start_timeindex = MaintenancePlan_model->index(items+i,4);
			QString poweroff_start_time = MaintenancePlan_model->data(poweroff_start_timeindex).toString();


			//ͣ�����ʱ��
			QModelIndex poweroff_end_timeindex = MaintenancePlan_model->index(items+i,5);
			QString poweroff_end_time = MaintenancePlan_model->data(poweroff_end_timeindex).toString();


			//ͣ������
			QModelIndex type_nameindex = MaintenancePlan_model->index(items+i,6);
			QString type_name = MaintenancePlan_model->data(type_nameindex).toString();

			//ͣ��ԭ��
			QModelIndex reason_nameindex = MaintenancePlan_model->index(items+i,7);
			QString reason_name = MaintenancePlan_model->data(reason_nameindex).toString();

			//ͣ��ʱ����Сʱ��
			QModelIndex hour_nameindex = MaintenancePlan_model->index(items+i,8);
			int hour_name = MaintenancePlan_model->data(hour_nameindex).toInt();

			//��ע
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

			//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
			//InitConTable::executeSQL(querysqlFAInfo,0);
			InitConTable::executeSQL(writesqlMaintenancePlanInfo,0);

			//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

		}




	}



}




void SignOutInformation::firstwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{
	int rows = DPCPoint_model->rowCount();
	//������ֻ��֪��д����
	for (int i=0;i<rows;i++)
	{
		//����,ObId
		QModelIndex index0 = DPCPoint_model->index(i,0);
		QString area_name = DPCPoint_model->data(index0).toString();
		//���վ
		QModelIndex index1 = DPCPoint_model->index(i,1);
		QString station_name = DPCPoint_model->data(index1).toString(); 
		//��·����
		QModelIndex index2 = DPCPoint_model->index(i,2);
		QString feeder_name = DPCPoint_model->data(index2).toString();
		//��������
		QModelIndex index3 = DPCPoint_model->index(i,3);
		QString switch_name = DPCPoint_model->data(index3).toString();
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//��������
		QModelIndex index4 = DPCPoint_model->index(i,6);
		QString opt_content = DPCPoint_model->data(index4).toString();
		//����ʱ��
		QModelIndex index5 = DPCPoint_model->index(i,7);
		QString opt_time = DPCPoint_model->data(index5).toString();
		//������
		QModelIndex index6 = DPCPoint_model->index(i,8);
		QString operatorinfo= DPCPoint_model->data(index6).toString();
		//�໤��
		QModelIndex index7 = DPCPoint_model->index(i,9);
		QString guarderinfo = DPCPoint_model->data(index7).toString();
		//��ע
		QModelIndex index8 = DPCPoint_model->index(i,10);
		QString note_name = DPCPoint_model->data(index8).toString();
		//Ϊʲôֻд����һ�����ѵ������ݿ���磿
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

		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqldpcoptrecord,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);
	
}

	

}


void SignOutInformation::secondwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	int rows = DPCPoint_model->rowCount();
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��

	
	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_DPC_OPTRECORD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	
	int hang = rows - items;

	if(hang>0)
	{

	//������ֻ��֪��д����
	for (int i=0;i<hang;i++)
	{

		//����
		QModelIndex area_nameindex = DPCPoint_model->index(items+i,0);
		QString area_name = DPCPoint_model->data(area_nameindex).toString();
		//���վ
		QModelIndex station_nameindex = DPCPoint_model->index(items+i,1);
		QString station_name = DPCPoint_model->data(station_nameindex).toString();
		//��·����
		QModelIndex feeder_nameindex = DPCPoint_model->index(items+i,2);
		QString feeder_name = DPCPoint_model->data(feeder_nameindex).toString();
		//��������
		QModelIndex switch_nameindex = DPCPoint_model->index(items+i,3);
		QString switch_name = DPCPoint_model->data(switch_nameindex).toString();
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//��������
		QModelIndex opt_contentindex = DPCPoint_model->index(items+i,6);
		QString opt_content = DPCPoint_model->data(opt_contentindex).toString();
		//����ʱ��

		QModelIndex opt_timeindex = DPCPoint_model->index(items+i,7);
		QString opt_time = DPCPoint_model->data(opt_timeindex).toString();
		//������
		QModelIndex operatorinfoindex = DPCPoint_model->index(items+i,8);
		QString operatorinfo= DPCPoint_model->data(operatorinfoindex).toString();
		//�໤��
		QModelIndex guarderinfoindex = DPCPoint_model->index(items+i,9);
		QString guarderinfo = DPCPoint_model->data(guarderinfoindex).toString();
		//��ע
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

		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqldpcoptrecord,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	


}
}













void SignOutInformation::firstwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	int rows = FeederOverload_model->rowCount();
	//������ֻ��֪��д����
	for (int i=0;i<rows;i++)
	{
		//����
		QModelIndex index0 = FeederOverload_model->index(i,0);
		QString area_name = FeederOverload_model->data(index0).toString();
		//���վ
		QModelIndex index1 = FeederOverload_model->index(i,1);
		QString station_name = FeederOverload_model->data(index1).toString();
		//��·����
		QModelIndex index2 = FeederOverload_model->index(i,2);
		QString feeder_name = FeederOverload_model->data(index2).toString();
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 

		//�����
		QModelIndex index4 = FeederOverload_model->index(i,5);
		QString amprating = FeederOverload_model->data(index4).toString();

		//������

		QModelIndex index5 = FeederOverload_model->index(i,6);
		QString max_current = FeederOverload_model->data(index5).toString();
		//����ʱ��
		QModelIndex index6 = FeederOverload_model->index(i,7);
		QString occur_time = FeederOverload_model->data(index6).toString();

		//������
		QModelIndex index7 = FeederOverload_model->index(i,8);
		QString overloadrate = FeederOverload_model->data(index7).toString();


		//��ע
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

		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlfeederoverload,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);
	
}

	

}


void SignOutInformation::secondwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	int rows = FeederOverload_model->rowCount();
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��

	


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FEEDER_OVERLOAD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);//����д���ʱ��Ҫ�õ�
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	int hang = rows - items;

	if(hang>0)
	{

	//������ֻ��֪��д����
	for (int i=0;i<hang;i++)
	{
		//����
		QModelIndex area_nameindex = FeederOverload_model->index(items+i,0);
		QString area_name = FeederOverload_model->data(area_nameindex).toString();
		//���վ
		QModelIndex station_nameindex = FeederOverload_model->index(items+i,1);
		QString station_name = FeederOverload_model->data(area_nameindex).toString();
		//��·����
		QModelIndex feeder_nameindex = FeederOverload_model->index(items+i,2);
		QString feeder_name = FeederOverload_model->data(feeder_nameindex).toString();
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//�����
		QModelIndex ampratingindex = FeederOverload_model->index(items+i,5);
		QString amprating = FeederOverload_model->data(ampratingindex).toString();
		//������
		QModelIndex max_currentindex = FeederOverload_model->index(items+i,6);
		QString max_current = FeederOverload_model->data(max_currentindex).toString();
		//����ʱ��
		QModelIndex occur_timeindex = FeederOverload_model->index(items+i,7);
		QString occur_time = FeederOverload_model->data(occur_timeindex).toString();
		//������
		QModelIndex overloadrateindex = FeederOverload_model->index(items+i,8);
		QString overloadrate = FeederOverload_model->data(overloadrateindex).toString();
		//��ע
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

		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlfeederoverload,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	


}
}



void SignOutInformation::secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
{
	FAInfo_model = new QStandardItemModel(ui->accidentinfo_tableview);//�µ�һ��model
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
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(4,QHeaderView::Stretch);//�¹�ԭ��
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(5,QHeaderView::Stretch);//��ע
	

	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO where START_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by START_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//ACCIDENT_INFO�¹���Ϣ
		std::string accident_info = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(accident_info.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle,i,1);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item2 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX���Ӱ����
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

		//¼��ʱ��


		//START_TIME����ʱ��
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 4);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//END_TIME����ʱ��,ϵͳĬ��ʱ��
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//REASON�¹�ԭ��
		std::string reason = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,5);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(reason.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//ACKERȷ����
		std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(note.c_str()));
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1 << item2 <<item3 <<item5 << item7 <<item9;
		FAInfo_model->appendRow(item);


	}


	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, rowsOfTer-1, 4);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);


	//�ٴ��ϱ�����ȥ�飬�ǵÿ�ʱ�䣬��ʱ������һ��¼��ʱ�䣬��ʱ������һ�ε��okbuttonʱ��,׷������

	RECORDSETHANDLE firstgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') AND TYPE =1 order by MESSAGETIME";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer2;i++)
	{
		//FEEDEROBJECTID�¹���Ϣ
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID��
		StringData feederiddata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederiddata);
		OMString strfeederid = (OMString)feederiddata;
		QString qstrfeederid = QString::fromUtf8(strfeederid.c_str());
		QStandardItem* newitem1 = new QStandardItem(qstrfeederid);

		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem2 = new QStandardItem(strstarttimeList[0]);

		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);

		//¼��ʱ��

		//FAULTTIME����ʱ��	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* newitem4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//����ʱ��,���û�ȥ��д

		//QStandardItem* newitem5 = new QStandardItem(QObject::tr(""));//����ʱ��

		//TYPE�¹�ԭ��
		ObId sonoffeederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID��
		short feedertype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData sonoffeederiddata;
		ToolUtil::databaseRead(sonoffeederid, AT_Name, &sonoffeederiddata);
		OMString strsonoffeederid = (OMString)sonoffeederiddata;
		QString qstrsonoffeederid = QString::fromUtf8(strsonoffeederid.c_str());
		
		QStandardItem* newitem6 = new QStandardItem(qstrsonoffeederid);

		//ȷ����
		//QStandardItem* newitem7 = new QStandardItem(QObject::tr(""));
		//��ע
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
	DPCPoint_model = new QStandardItemModel(ui->DPCPoint_tableview);//�µ�һ��model
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

	//����setModel()����������ģ����QTableView��
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableViewƽ�������п�
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 

	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlDPCPoint = "select area,station,feeder,switch,work_day,work_index,opt_content,opt_time,operatorinfo,guarderinfo,note from H_DPC_OPTRECORD where OPT_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OPT_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherDPCPointSetHandle,secondsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//����ObIdתname
		std::string strsubcontrolareafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//���վObIdתname
		std::string strpmsstationdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//��·ObIdתname
		std::string strfeederdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,2);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		//��������ObIdתname
		std::string strswitchdatafromold = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,2);
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strswitchdatafromold.c_str()));
		item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item4->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle,i,4);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item5 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX���Ӱ����
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

		//¼��ʱ��

		
		//opt_content��������
		std::string opt_content_info = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,6);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(opt_content_info.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 


		//opt_time����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle,i,7);
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* item8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		
		//������
		std::string stdopertor = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,8);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(stdopertor.c_str()));
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 

		//�໤��
		std::string stdguarder = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,9);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(stdguarder.c_str()));
		item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item10->setForeground(QBrush(QColor(0, 255, 0))); 
		

		
		//NOTE��ע
		std::string stdnote = CPS_ORM_RsGetStringValue(secondgatherDPCPointSetHandle,i,10);
		QStandardItem* item11 = new QStandardItem(QString::fromUtf8(stdnote.c_str()));
		item11->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item11->setForeground(QBrush(QColor(0, 255, 0))); 
		


		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1  << item2 <<item3<<item4 <<item5 <<item6  <<item7   <<item8 <<item9<<item10 <<item11;
		DPCPoint_model->appendRow(item);


	}

	
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle, rowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherDPCPointSetHandle);


	//�ٴ��ϱ�����ȥ�飬�ǵÿ�ʱ�䣬��ʱ������һ��¼��ʱ�䣬��ʱ������һ�ε��okbuttonʱ��,׷������

	

	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString year = curDateTime.toString("yyyy");
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_"+year+" where ACTIONTIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer2;i++)
	{
		//�ҵ�ң��
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(firstgatherDPCPointSetHandle,i,0);
		//�ҵ�����ң��
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;



		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* newitem1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* newitem2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));



		//��·����
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData feederdata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));










		//��������,obid�ݹ����ϲ�ѯ



		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData switchdata;
		ToolUtil::databaseRead(switchid, AT_Name, &switchdata);
		OMString strswitchdata = (OMString)switchdata;
		QStandardItem* newitem4 = new QStandardItem(QString::fromUtf8(strswitchdata.c_str()));

		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem5 = new QStandardItem(strstarttimeList[0]);

		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem6 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


		//¼��ʱ��


		//��������
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

		//����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(firstgatherDPCPointSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* newitem8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));

		//������
		std::string operatorinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,4);
		QStandardItem* newitem9 = new QStandardItem(QString::fromUtf8(operatorinfo.c_str()));

		//�໤��
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,5);
		QStandardItem* newitem10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//��ע
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


	FeederOverload_model = new QStandardItemModel(ui->lineload_tableview);//�µ�һ��model
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

	//����setModel()����������ģ����QTableView��
	ui->lineload_tableview->setModel(FeederOverload_model);
	//QTableViewƽ�������п�
	ui->lineload_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 





	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFeederOverload = "select area,station,feeder,work_day,work_index,amprating,max_current,occur_time,overloadrate,note from H_FEEDER_OVERLOAD where OCCUR_TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OCCUR_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//����ObIdתname
		std::string strsubcontrolareafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//���վObIdתname
		std::string strpmsstationdatafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//��·ObIdתname
		std::string strfeederdatafromold = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,2);
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		

		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle,i,3);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item4 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item4->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX���Ӱ����
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

		//¼��ʱ��

		//�����

		double amprating = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,5);
		QString stramprating = QString::number(amprating);
		QStandardItem* item6 = new QStandardItem(stramprating);
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 



		//������

		
		double maxcurrent = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,6);
		QString strmaxcurrent = QString::number(maxcurrent);
		QStandardItem* item7 = new QStandardItem(strmaxcurrent);
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 
		


		//����ʱ��
		ORMTimeStamp occur_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle,i,7);
		QDateTime stroccurtime = SetTimeFromDB(occur_time);
		QStandardItem* item8 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		//������
		double overloadrate = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,8);
		QString stroverloadrate = QString::number(overloadrate);
		
		QStandardItem* item9 = new QStandardItem(stroverloadrate);
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 


		//NOTE��ע
		std::string stdnote = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,9);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(stdnote.c_str()));
		item10->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item10->setForeground(QBrush(QColor(0, 255, 0))); 



		



		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1  << item2 <<item3<<item4 <<item5 <<item6   <<item7<<item8 <<item9<<item10;
		FeederOverload_model->appendRow(item);


	}

	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle, rowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFeederOverloadSetHandle);


	//�ٴ��ϱ�����ȥ�飬�ǵÿ�ʱ�䣬��ʱ������һ��¼��ʱ�䣬��ʱ������һ�ε��okbuttonʱ��,׷������

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

		//�ҵ�ң��
		ObId mvpointid = CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,0);
		

		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);
		

		



		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* newitem1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* newitem2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));



		//��·����
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		StringData feederdata;
		ToolUtil::databaseRead(MemberOfLineid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* newitem3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));



		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* newitem4 = new QStandardItem(strstarttimeList[0]);

		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* newitem5 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);



		

		
		//¼��ʱ��


		//�����PMSBreaker
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		FloatData switchampratingdata;
		ToolUtil::databaseRead(switchid, AT_ampRating, &switchampratingdata);
		QStandardItem* newitem6 = new QStandardItem(QString::number(switchampratingdata));

		//������
		int maxcurrent = (int)CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,2);//FAULTTIME
		QStandardItem* newitem7 = new QStandardItem(QString::number(maxcurrent));
		
		//����ʱ��
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFeederOverloadSetHandle,i,3);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		QStandardItem* newitem8 = new QStandardItem(start_time);


		/*
		//������
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherFeederOverloadSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//��ע
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
	MaintenancePlan_model = new QStandardItemModel(ui->maintenanceplan_tableview);//�µ�һ��model
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

	//����setModel()����������ģ����QTableView��
	ui->maintenanceplan_tableview->setModel(MaintenancePlan_model);

	ui->maintenanceplan_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
	//QTableViewƽ�������п�
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
	//ui->DPCPonit_tableview->verticalHeader()->hide(); 
	


	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherMaintenancePlanSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlMaintenancePlanInfo = "select station,feeder, WORK_DAY, WORK_INDEX, TIME, START_TIME, END_TIME, type, REASON, powerofftime, NOTE from H_MAINTENANCE_PLAN where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherMaintenancePlanSetHandle,secondsqlMaintenancePlanInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//���վ��Ϣ
		std::string station_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(station_info.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 


		//��·��Ϣ
		std::string feeder_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,1);
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(feeder_info.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 

		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle,i,2);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item3 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 


		//WORK_INDEX���Ӱ����
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

		//¼��ʱ��


		//START_TIME����ʱ��
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 5);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		
		//END_TIME����ʱ��
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
		//ACKERȷ����
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



		//NOTE��ע
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

	//ui->maintenanceplan_tableWidget->setColumnCount(10);//10��
	//ui->maintenanceplan_tableWidget->setHorizontalHeaderLabels(QStringList()<<QObject::tr("station")<<QObject::tr("feeder")<<QObject::tr("work_day")<<QObject::tr("work_index")<<QObject::tr("poweroffstart_time")<<QObject::tr("poweroffend_time")<<QObject::tr("powerofftype")<<QObject::tr("poweroffreason")<<QObject::tr("powerofftime")<<QObject::tr("note"));
	//ui->maintenanceplan_tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);



	//QStandardItemModel *maintenanceplan_model = new QStandardItemModel();

	//ui->maintenanceplan_tableWidget->setModel(maintenanceplan_model);

	////���±�����ȥ������������
	//RECORDSETHANDLE secondgatherMaintenancePlanSetHandle = CPS_ORM_RsNewRecordSet();
	//QString secondsqlMaintenancePlanInfo = "select STATION, FEEDER, WORK_DAY, WORK_INDEX, TIME, START_TIME, END_TIME, TYPE, REASON, POWEROFFTIME, NOTE from H_MAINTENANCE_PLAN where time between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	//int rowsOfTer = CPS_ORM_RsLoadData(secondgatherMaintenancePlanSetHandle,secondsqlMaintenancePlanInfo.toUtf8().data(),dbPoolHandle);
	//for (int i=0;i<rowsOfTer;i++)
	//{
	//	//���վ��Ϣ
	//	std::string station_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,0);
	//	QStandardItem* item1 = new QStandardItem(QString::fromUtf8(station_info.c_str()));
	//	item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item1->setForeground(QBrush(QColor(0, 255, 0))); 

	//	
	//		

	//	//��·��Ϣ
	//	std::string line_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,1);
	//	QStandardItem* item2 = new QStandardItem(QString::fromUtf8(line_info.c_str()));
	//	item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item2->setForeground(QBrush(QColor(0, 255, 0))); 

	//	//ui->maintenanceplan_tableWidget->setCellWidget(i,1,(QWidget *)item2);

	//	//WORK_DAY������	  
	//	ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle,i,2);
	//	QDateTime strwork_day = SetTimeFromDB(work_day);
	//	QStandardItem* item3 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
	//	item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item3->setForeground(QBrush(QColor(0, 255, 0))); 

	//	//ui->maintenanceplan_tableWidget->setCellWidget(i,2,(QWidget *)item3);
	//	//WORK_INDEX���Ӱ����
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

	//	//¼��ʱ��


	//	//START_TIME����ʱ��
	//	ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 5);
	//	QDateTime strstarttime = SetTimeFromDB(start_time);
	//	QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item5->setForeground(QBrush(QColor(0, 255, 0))); 


	//	/*
	//	//END_TIME����ʱ��,ϵͳĬ��ʱ��
	//	ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
	//	QDateTime strendtime = SetTimeFromDB(end_time);
	//	QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item6->setForeground(QBrush(QColor(0, 255, 0))); 
	//	*/

	//	//END_TIME����ʱ��
	//	ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 6);
	//	QDateTime strendtime = SetTimeFromDB(end_time);
	//	QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
	//	item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item6->setForeground(QBrush(QColor(0, 255, 0))); 


	//	//�¹�����
	//	std::string type = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,7);
	//	QStandardItem* item7 = new QStandardItem(QString::fromUtf8(type.c_str()));
	//	item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item7->setForeground(QBrush(QColor(0, 255, 0))); 



	//	//REASON�¹�ԭ��
	//	std::string reason = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,8);
	//	QStandardItem* item8 = new QStandardItem(QString::fromUtf8(reason.c_str()));
	//	item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item8->setForeground(QBrush(QColor(0, 255, 0))); 


	//	/*
	//	//ACKERȷ����
	//	std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
	//	QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
	//	item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item8->setForeground(QBrush(QColor(0, 255, 0))); 
	//	*/

	//	//REASON�¹�ԭ��
	//	std::string poweroff_time = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,9);
	//	QStandardItem* item9 = new QStandardItem(QString::fromUtf8(poweroff_time.c_str()));
	//	item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//	item9->setForeground(QBrush(QColor(0, 255, 0))); 


	//	//NOTE��ע
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

	

	//���վ��Ϣ
	QString station_info = "";
	QStandardItem* item1 = new QStandardItem(station_info);

	

	//��·��Ϣ
	QString feeder_info = "";
	QStandardItem* item2 = new QStandardItem(feeder_info);

	//������
	RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//��ǰʱ����Ű�ʱ�����ʱ���
	int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	QDateTime current_date_time =QDateTime::currentDateTime();
	int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//��ǰ������Сʱ
	QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	QString current_date = current_date_timeminus.toString("yyyy-MM-dd");
	//QDate date = QDate::fromString(current_date,"yyyyMMdd");
	QStandardItem* item3 = new QStandardItem(current_date);

	
	
	//Ĭ�Ͻ��Ӱ��
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
	
	//ͣ�翪ʼʱ��
	QString poweroff_start_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	QStandardItem* item5 = new QStandardItem(poweroff_start_time);
	//ͣ�����ʱ��
	QString poweroff_end_time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	QStandardItem* item6 = new QStandardItem(poweroff_end_time);

	*/
	////¼��ʱ��

	////FAULTTIME����ʱ��	  
	//ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
	//QDateTime stroccurtime = SetTimeFromDB(occurtime);
	//QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

	////����ʱ��,���û�ȥ��д

	////QStandardItem* item5 = new QStandardItem(QObject::tr(""));//����ʱ��

	////TYPE�¹�ԭ��
	//ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID��
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

	////ȷ����
	////QStandardItem* item7 = new QStandardItem(QObject::tr(""));
	////��ע
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













