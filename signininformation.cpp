#include "signininformation.h"
#include "ui_signininformation.h"
#include <QMessageBox>


#include <QStandardItemModel>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "signout.h"
#include "signin.h"
#include "signininformation.h"
#include "confirm.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"

#include <QCoreApplication>

extern DBPOOLHANDLE dbPoolHandle;

extern  Database *database;

extern InitConTable *initConTable;

extern QList<QString> MVPointlist;

SignInInformation::SignInInformation(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::SignInInformation)
{
    

	 ui->setupUi(this);


	 
    
}
 SignInInformation::~SignInInformation()

{

    delete ui;

}

 void SignInInformation::showsigninFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
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
	
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);
 }
 void SignInInformation::showsigninMaintenancePlan(QString strstarttime, QString strendtime, QString startshift)
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
 }
 void SignInInformation::showsigninDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
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
	 
	 CPS_ORM_RsFreeRecordSet(secondgatherDPCPointSetHandle);


 }
 void SignInInformation::showsigninFeederOverload(QString strstarttime, QString strendtime, QString startshift)
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

	 CPS_ORM_RsFreeRecordSet(secondgatherFeederOverloadSetHandle);



 }
 

QDateTime SignInInformation::getSignPageTime(QDateTime signcurtime)
{
	signpagetime = signcurtime;
	return signpagetime;
}

QDateTime SignInInformation::getSignPageDate(QDateTime signdate)
{
	signpagedate = signdate;
	return signpagedate;
}

QString SignInInformation::getSignPageShift(QString signshift)
{
	 signpageshift = signshift;
	 return signpageshift;
}

QDateTime SignInInformation::getcurDateTimedata()
{	 
	return informationcurDateTime;
}





void SignInInformation::on_okButton_clicked()
{
	
	//¼��ʱ��
	informationcurDateTime=QDateTime::currentDateTime();






}
















