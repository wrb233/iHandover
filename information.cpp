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

extern InitConTable *initConTable;

extern QList<QString> MVPointlist;

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

 void Information::showsigninFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift)
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

	 //���±�����ȥ������������
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by START_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//1ACCIDENT_INFO�¹���Ϣ
		std::string accident_info = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,0);
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(accident_info.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 

		//2WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle,i,1);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QStandardItem* item2 = new QStandardItem(strwork_day.toString("yyyy-MM-dd"));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 


		//3WORK_INDEX���Ӱ����
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

		//4¼��ʱ��


		//5START_TIME����ʱ��
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 4);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QStandardItem* item5 = new QStandardItem(strstarttime.toString("yyyy-MM-dd hh:mm:ss"));
		item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item5->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//6END_TIME����ʱ��,ϵͳĬ��ʱ��
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//7REASON�¹�ԭ��
		std::string reason = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,5);
		QStandardItem* item7 = new QStandardItem(QString::fromUtf8(reason.c_str()));
		item7->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item7->setForeground(QBrush(QColor(0, 255, 0))); 


		/*
		//8ACKERȷ����
		std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		*/


		//9NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(note.c_str()));
		item9->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item9->setForeground(QBrush(QColor(0, 255, 0))); 





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item5 <<item6 << item7 <<item8 <<item9;
		item << item1 << item2 <<item3 <<item5 << item7 <<item9;
		FAInfo_model->appendRow(item);


	}
	
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);
 }
 void Information::showsigninMaintenancePlan(QString strstarttime, QString strendtime, QString startshift)
 {
	 QStandardItemModel *MAINTPlan_model = new QStandardItemModel();
	 MAINTPlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("station")));
	 MAINTPlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("feeder")));
	 MAINTPlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_day")));
	 MAINTPlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_index")));
	 MAINTPlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("poweroffstart_time")));
	 MAINTPlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("poweroffend_time")));
	 MAINTPlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("powerofftype")));
	 MAINTPlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("poweroffreason")));
	 MAINTPlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("powerofftime")));
	 MAINTPlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));
	 ui->maintenanceplan_tableview->setModel(MAINTPlan_model);
	 ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
 }
 void Information::showsigninDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
 {
	 QStandardItemModel *DPCPoint_model = new QStandardItemModel();
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
	 QString secondsqlDPCPoint = "select area,station,feeder,switch,work_day,work_index,opt_content,opt_time,operatorinfo,guarderinfo,note from H_DPC_OPTRECORD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OPT_TIME";
	 int rowsOfTer = CPS_ORM_RsLoadData(secondgatherDPCPointSetHandle,secondsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	 for (int i=0;i<rowsOfTer;i++)
	 {
		 //����ObIdתname
		 ObId subcontrolareaidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,0);
		 StringData subcontrolareadatafromold;
		 ToolUtil::databaseRead(subcontrolareaidfromold, AT_Name, &subcontrolareadatafromold);
		 OMString strsubcontrolareafromold = (OMString)subcontrolareadatafromold;
		 QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		 item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		 item1->setForeground(QBrush(QColor(0, 255, 0))); 
		 //���վObIdתname
		 ObId pmsstationidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,1);
		 StringData pmsstationdatafromold;
		 ToolUtil::databaseRead(pmsstationidfromold, AT_Name, &pmsstationdatafromold);
		 OMString strpmsstationdatafromold = (OMString)pmsstationdatafromold;
		 QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		 item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		 item2->setForeground(QBrush(QColor(0, 255, 0))); 
		 //��·ObIdתname
		 ObId feederidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,2);
		 StringData feederdatafromold;
		 ToolUtil::databaseRead(feederidfromold, AT_Name, &feederdatafromold);
		 OMString strfeederdatafromold = (OMString)feederdatafromold;
		 QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		 item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		 item3->setForeground(QBrush(QColor(0, 255, 0))); 
		 //��������ObIdתname
		 ObId switchidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,3);
		 //qDebug()<<switchid;
		 StringData switchdatafromold;
		 ToolUtil::databaseRead(switchidfromold, AT_Name, &switchdatafromold);
		 OMString strswitchdatafromold = (OMString)switchdatafromold;
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


	 
	 CPS_ORM_RsFreeRecordSet(secondgatherDPCPointSetHandle);




 }
 void Information::showsigninFeederOverload(QString strstarttime, QString strendtime, QString startshift)
 {
	 QStandardItemModel *FeederOverload_model = new QStandardItemModel();
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


	 //���±�����ȥ������������
	 RECORDSETHANDLE secondgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	 QString secondsqlFeederOverload = "select area,station,feeder,work_day,work_index,amprating,max_current,occur_time,overloadrate,note from H_FEEDER_OVERLOAD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by OCCUR_TIME";
	 int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	 for (int i=0;i<rowsOfTer;i++)
	 {
		 //����ObIdתname
		 ObId subcontrolareaidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,0);
		 StringData subcontrolareadatafromold;
		 ToolUtil::databaseRead(subcontrolareaidfromold, AT_Name, &subcontrolareadatafromold);
		 OMString strsubcontrolareafromold = (OMString)subcontrolareadatafromold;
		 QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		 item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		 item1->setForeground(QBrush(QColor(0, 255, 0))); 
		 //���վObIdתname
		 ObId pmsstationidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,1);
		 StringData pmsstationdatafromold;
		 ToolUtil::databaseRead(pmsstationidfromold, AT_Name, &pmsstationdatafromold);
		 OMString strpmsstationdatafromold = (OMString)pmsstationdatafromold;
		 QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		 item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		 item2->setForeground(QBrush(QColor(0, 255, 0))); 
		 //��·ObIdתname
		 ObId feederidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,2);
		 StringData feederdatafromold;
		 ToolUtil::databaseRead(feederidfromold, AT_Name, &feederdatafromold);
		 OMString strfeederdatafromold = (OMString)feederdatafromold;
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
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by MESSAGETIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	AT_Name = database->matchAType("Name");
	for (int i=0;i<rowsOfTer;i++)
	{
		//FEEDEROBJECTID�¹���Ϣ
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID��
		StringData data;
		ToolUtil::databaseRead(feederid, AT_Name, &data);
		OMString str = (OMString)data;
		QString qstr = QString::fromUtf8(str.c_str());
		QStandardItem* item1 = new QStandardItem(qstr);

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
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QStandardItem* item4 = new QStandardItem(stroccurtime.toString("yyyy-MM-dd hh:mm:ss"));

		//����ʱ��,���û�ȥ��д

		//QStandardItem* item5 = new QStandardItem(QObject::tr(""));//����ʱ��

		//TYPE�¹�ԭ��
		ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID��
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

void Information::firstshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *MAINTPlan_model = new QStandardItemModel();
	MAINTPlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("station")));
    MAINTPlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("feeder")));
	MAINTPlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_day")));
	MAINTPlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_index")));
    MAINTPlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("poweroffstart_time")));
    MAINTPlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("poweroffend_time")));
    MAINTPlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("powerofftype")));
	MAINTPlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("poweroffreason")));
	MAINTPlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("powerofftime")));
	MAINTPlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));
	ui->maintenanceplan_tableview->setModel(MAINTPlan_model);
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	
}

void Information::firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *DPCPoint_model = new QStandardItemModel();
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


	RECORDSETHANDLE firstgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020 where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by ACTIONTIME";
	int rowsOfTer = CPS_ORM_RsLoadData(firstgatherDPCPointSetHandle,firstsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	AT_Name = database->matchAType("Name");
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



		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));
		
		
		
		//��·����
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData feederdata;
		ToolUtil::databaseRead(feederid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));



		






		//��������,obid�ݹ����ϲ�ѯ

		

		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData switchdata;
		ToolUtil::databaseRead(switchid, AT_Name, &switchdata);
		OMString strswitchdata = (OMString)switchdata;
		QStandardItem* item4 = new QStandardItem(QString::fromUtf8(strswitchdata.c_str()));

		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item5 = new QStandardItem(strstarttimeList[0]);

		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item6 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);


		//¼��ʱ��


		//��������
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
		}	

		else if ((dpcAction== 1)&&( actionResult == 1))
		{
			//QString optcontent = "actionfail";
			item7 = new QStandardItem(QObject::tr("actionfail"));
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

		//����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(firstgatherDPCPointSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QStandardItem* item8 = new QStandardItem(stroperationtime.toString("yyyy-MM-dd hh:mm:ss"));
		
		//������
		std::string operatorinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,4);
		QStandardItem* item9 = new QStandardItem(QString::fromUtf8(operatorinfo.c_str()));
		
		//�໤��
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherDPCPointSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));
		
		//��ע
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

		item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7 <<item8<<item9<<item10<<item11;

		DPCPoint_model->appendRow(item);


	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);

}

void Information::firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift)
{

	QStandardItemModel *FeederOverload_model = new QStandardItemModel();
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
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_202005 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+") order by SDATE";



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
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_201909 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


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
		

		



		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData subcontrolareadata;
		ToolUtil::databaseRead(subcontrolareaid, AT_Name, &subcontrolareadata);
		OMString strsubcontrolarea = (OMString)subcontrolareadata;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolarea.c_str()));
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		//qDebug()<<switchid;
		StringData pmsstationdata;
		ToolUtil::databaseRead(pmsstationid, AT_Name, &pmsstationdata);
		OMString strpmsstationdata = (OMString)pmsstationdata;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdata.c_str()));



		//��·����
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		StringData feederdata;
		ToolUtil::databaseRead(MemberOfLineid, AT_Name, &feederdata);
		OMString strfeederdata = (OMString)feederdata;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdata.c_str()));



		//������
		QStringList strstarttimeList = strstarttime.split(" ");
		QStandardItem* item4 = new QStandardItem(strstarttimeList[0]);

		//���Ӱ����
		RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString sqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+startshift;
		int rowsOfTer = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string startshift = CPS_ORM_RsGetStringValue(startshiftSetHandle,0,0);
		QStandardItem* item5 = new QStandardItem(QString::fromUtf8(startshift.c_str()));
		CPS_ORM_RsFreeRecordSet(startshiftSetHandle);



		

		
		//¼��ʱ��


		//�����PMSBreaker
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		//qDebug()<<switchid;
		FloatData switchampratingdata;
		ToolUtil::databaseRead(switchid, AT_ampRating, &switchampratingdata);
		QStandardItem* item6 = new QStandardItem(QString::number(switchampratingdata));

		//������
		int maxcurrent = (int)CPS_ORM_RsGetNumberValue(firstgatherFeederOverloadSetHandle,i,2);//FAULTTIME
		QStandardItem* item7 = new QStandardItem(QString::number(maxcurrent));
		
		//����ʱ��
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(firstgatherFeederOverloadSetHandle,i,3);
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		QStandardItem* item8 = new QStandardItem(start_time);


		/*
		//������
		std::string guarderinfo = CPS_ORM_RsGetStringValue(firstgatherFeederOverloadSetHandle,i,5);
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(guarderinfo.c_str()));

		//��ע
		QStandardItem* item11 = new QStandardItem(QObject::tr(""));

		*/
		QStandardItem* item9 = new QStandardItem(QObject::tr(""));
		QStandardItem* item10 = new QStandardItem(QObject::tr(""));



		QList<QStandardItem*> item;
		item << item1 << item2 <<item3 <<item4 <<item5 <<item6<<item7<<item8<<item9<<item10;

		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7 <<item8<<item9<<item10<<item11;

		FeederOverload_model->appendRow(item);


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



QDateTime Information::getSignPageTime(QDateTime signcurtime)
{
	signpagetime = signcurtime;
	return signpagetime;
}

QDateTime Information::getSignPageDate(QDateTime signdate)
{
	signpagedate = signdate;
	return signpagedate;
}

QString Information::getSignPageShift(QString signshift)
{
	 signpageshift = signshift;
	 return signpageshift;
}

QDateTime Information::getcurDateTimedata()
{	 
	return informationcurDateTime;
}





void Information::on_okButton_clicked()
{
	
	//¼��ʱ��
	informationcurDateTime=QDateTime::currentDateTime();






}



//д����Ҫ�ĸ�������strstarttime�����գ�strendtime����ʱ�䣬startshift���Ӱ�Σ�informationcurDateTime¼��ʱ�䣨�ǳ���Ҫ��



void Information::firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	RECORDSETHANDLE queryFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	

	QString querysqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	
	

	int rowsOfTer = CPS_ORM_RsLoadData(queryFAInfoSetHandle,querysqlFAInfo.toUtf8().data(),dbPoolHandle);


	//������ֻ��֪��д����
	for (int i=0;i<rowsOfTer;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//�¹���Ϣ������char
	    QAbstractItemModel *accidentinfo_model = ui->accidentinfo_tableview->model();
		QModelIndex index = accidentinfo_model->index(i,0);
		QString accidentinfo_name = accidentinfo_model->data(index).toString();
		//qDebug()<<name;
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME����ʱ��timestamp	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//����ʱ�䣬���û��Լ�¼���ʱ�䣬��ô���浽����

		//TYPE�¹�ԭ��
		ObId cbid = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,1);//OBJECTID��
		short fatype = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("����"));
		else
			qstr3.append(tr("��λ"));
		//ȷ����
		//QModelIndex index2 = accidentinfo_model->index(i,6);
		//QString confirm_name = accidentinfo_model->data(index2).toString();
		//qDebug()<<name2;
		//��ע
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



		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	CPS_ORM_RsFreeRecordSet(queryFAInfoSetHandle);

}

void Information::secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��

	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO";
	int queryrowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, queryrowsOfTer-1, 4);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
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


	//������ֻ��֪��д����
	for (int i=0;i<newrowsOfTer;i++)
	{
		
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		
		//�¹���Ϣ������char
	    QAbstractItemModel *accidentinfo_model = ui->accidentinfo_tableview->model();
		QModelIndex index = accidentinfo_model->index(i,0);
		QString accidentinfo_name = accidentinfo_model->data(index).toString();
		//qDebug()<<name;
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//FAULTTIME����ʱ��timestamp	  
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFAInfoSetHandle,i,2);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString start_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//����ʱ�䣬���û��Լ�¼���ʱ�䣬��ô���浽����

		//TYPE�¹�ԭ��
		ObId cbid = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,1);//OBJECTID��
		short fatype = CPS_ORM_RsGetNumberValue(queryFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("����"));
		else
			qstr3.append(tr("��λ"));
		//ȷ����
		//QModelIndex index2 = accidentinfo_model->index(i,6);
		//QString confirm_name = accidentinfo_model->data(index2).toString();
		//qDebug()<<name2;
		//��ע
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



		
	//������һ��cpp���static�������ĺ�����ִ��sql��䣬�����õ�
    //InitConTable::executeSQL(querysqlFAInfo,0);
	InitConTable::executeSQL(writesqlFAInfo,0);
	
	//CPS_ORM_RsFreeRecordSet(writeFAInfoSetHandle);

}

	CPS_ORM_RsFreeRecordSet(queryFAInfoSetHandle);

}





void Information::firstwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	RECORDSETHANDLE queryDPCOptRecordSetHandle = CPS_ORM_RsNewRecordSet();
	

	QString querysqlDPCOptRecord = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020 where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	
	

	int rowsOfTer = CPS_ORM_RsLoadData(queryDPCOptRecordSetHandle,querysqlDPCOptRecord.toUtf8().data(),dbPoolHandle);


	//������ֻ��֪��д����
	for (int i=0;i<rowsOfTer;i++)
	{
		QAbstractItemModel *dpcoptrecord_model = ui->DPCPoint_tableview->model();
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(queryDPCOptRecordSetHandle,i,0);
		//�ҵ�����ң��
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;



		//����
		
		//����,ObId
	    ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		QString area_name = QString::number(subcontrolareaid); 
		//qDebug()<<name;
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		QString station_name = QString::number(pmsstationid); 
		//��·����
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		QString feeder_name = QString::number(feederid); 
		//��������
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		QString switch_name = QString::number(switchid);
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//��������
		QModelIndex index4 = dpcoptrecord_model->index(i,6);
		QString opt_content = dpcoptrecord_model->data(index4).toString();
		//����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(queryDPCOptRecordSetHandle,i,2);//FAULTTIME
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QString opt_time = stroperationtime.toString("yyyy-MM-dd hh:mm:ss");
		//������
		QModelIndex index5 = dpcoptrecord_model->index(i,8);
		QString operatorinfo= dpcoptrecord_model->data(index5).toString();
		//�໤��
		QModelIndex index6 = dpcoptrecord_model->index(i,9);
		QString guarderinfo = dpcoptrecord_model->data(index6).toString();
		//��ע
		QModelIndex index7 = dpcoptrecord_model->index(i,10);
		QString note_name = dpcoptrecord_model->data(index7).toString();
		
		/*
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,acker,note) VALUES ('"\
				
				+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


				
				+QString("%3").arg(qstr3)+"','"\
				+QString("%4").arg(confirm_name)+"','"\
				+QString("%5").arg(note_name)+"')");
				*/

				

		
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

	CPS_ORM_RsFreeRecordSet(queryDPCOptRecordSetHandle);

}


void Information::secondwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��

	RECORDSETHANDLE secondgatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlDPCPoint = "select area,station,feeder,switch,work_day,work_index,opt_content,opt_time,operatorinfo,guarderinfo,note from H_DPC_OPTRECORD";
	int queryrowsOfTer = CPS_ORM_RsLoadData(secondgatherDPCPointSetHandle,secondsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherDPCPointSetHandle, queryrowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherDPCPointSetHandle);


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_DPC_OPTRECORD";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	
	
	
	RECORDSETHANDLE queryDPCOptRecordSetHandle = CPS_ORM_RsNewRecordSet();
	QString querysqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020  where ACTIONTIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int newrowsOfTer = CPS_ORM_RsLoadData(queryDPCOptRecordSetHandle,querysqlDPCPoint.toUtf8().data(),dbPoolHandle);


	//������ֻ��֪��д����
	for (int i=0;i<newrowsOfTer;i++)
	{
		
		QAbstractItemModel *dpcoptrecord_model = ui->DPCPoint_tableview->model();
		//RECORDSETHANDLE writeFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
		ObId dpcpointid = CPS_ORM_RsGetNumberValue(queryDPCOptRecordSetHandle,i,0);
		//�ҵ�����ң��
		ObId dpspointlink = initConTable->getDPSPointLink(dpcpointid);
		//qDebug()<<dpspointlink;
		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(dpspointlink);
		//qDebug()<<psrlink;



		//����
		
		//����,ObId
	    ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		QString area_name = QString::number(subcontrolareaid); 
		//qDebug()<<name;
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		QString station_name = QString::number(pmsstationid); 
		//��·����
		ObId feederid = ToolUtil::findOTypeByObId(psrlink,OT_Feeder,OT_PMSRoot);
		QString feeder_name = QString::number(feederid); 
		//��������
		ObId switchid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		QString switch_name = QString::number(switchid);
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 
		//��������
		QModelIndex index4 = dpcoptrecord_model->index(items+i,6);
		QString opt_content = dpcoptrecord_model->data(index4).toString();
		//����ʱ��
		ORMTimeStamp operationtime = CPS_ORM_RsGetTimeValue(queryDPCOptRecordSetHandle,i,2);
		QDateTime stroperationtime = SetTimeFromDB(operationtime);
		QString opt_time = stroperationtime.toString("yyyy-MM-dd hh:mm:ss");
		//������
		QModelIndex index5 = dpcoptrecord_model->index(items+i,8);
		QString operatorinfo= dpcoptrecord_model->data(index5).toString();
		//�໤��
		QModelIndex index6 = dpcoptrecord_model->index(items+i,9);
		QString guarderinfo = dpcoptrecord_model->data(index6).toString();
		//��ע
		QModelIndex index7 = dpcoptrecord_model->index(items+i,10);
		QString note_name = dpcoptrecord_model->data(index7).toString();
		
		/*
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,acker,note) VALUES ('"\
				
				+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


				
				+QString("%3").arg(qstr3)+"','"\
				+QString("%4").arg(confirm_name)+"','"\
				+QString("%5").arg(note_name)+"')");
				*/

				

		
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

	CPS_ORM_RsFreeRecordSet(queryDPCOptRecordSetHandle);


}













void Information::firstwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	RECORDSETHANDLE queryFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	

	//QString querysqlDPCOptRecord = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020 where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	

	qDebug()<<MVPointlist.size();
	QString strMVPointlist="";
	QString strMVPointlist1="";
	QString strMVPointlist2="";

	QString querysqlFeederOverload;

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
		querysqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_202005 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+")";



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
		querysqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_201909 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


	}












	//QString querysqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_202005 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+")";


	int rowsOfTer = CPS_ORM_RsLoadData(queryFeederOverloadSetHandle,querysqlFeederOverload.toUtf8().data(),dbPoolHandle);


	//������ֻ��֪��д����
	for (int i=0;i<rowsOfTer;i++)
	{
		QAbstractItemModel *feederoverload_model = ui->lineload_tableview->model();
		//�ҵ�ң��
		ObId mvpointid = CPS_ORM_RsGetNumberValue(queryFeederOverloadSetHandle,i,0);


		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);





		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);
		
		QString area_name = QString::number(subcontrolareaid); 
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);
		
		QString station_name = QString::number(pmsstationid); 


		//��·����
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		QString feeder_name = QString::number(MemberOfLineid);

		
	
		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 

		//�����
		QModelIndex index4 = feederoverload_model->index(i,5);
		QString amprating = feederoverload_model->data(index4).toString();

		//������

		QModelIndex index5 = feederoverload_model->index(i,6);
		QString max_current = feederoverload_model->data(index5).toString();
		//����ʱ��
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFeederOverloadSetHandle,i,1);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString occur_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//������
		QModelIndex index6 = feederoverload_model->index(i,8);
		QString overloadrate = feederoverload_model->data(index6).toString();


		//��ע
		QModelIndex index7 = feederoverload_model->index(i,9);
		QString note_name = feederoverload_model->data(index7).toString();
		
		/*
		QString writesqlFAInfo = QString("INSERT INTO H_FA_ACCIDENTINFO (accident_info,work_day,work_index,time,start_time,reason,acker,note) VALUES ('"\
				
				+QString("%1").arg(accidentinfo_name)+"',to_timestamp('"+strstarttimedate+"','yyyy-MM-dd'),"\

				+QString("%2").arg(shift)+",to_timestamp('"+strinformationcurDateTime+"','yyyy-MM-dd hh24:mi:ss'),"\

				+"to_timestamp('"+start_time+"','yyyy-MM-dd hh24:mi:ss'),'"\


				
				+QString("%3").arg(qstr3)+"','"\
				+QString("%4").arg(confirm_name)+"','"\
				+QString("%5").arg(note_name)+"')");
				*/

				

		
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

	CPS_ORM_RsFreeRecordSet(queryFeederOverloadSetHandle);

}


void Information::secondwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime)
{

	//����ֻ�ܲ����µ������ˣ����ܱ�����ձ���ͷ�壬����ô֪���µ����ݴ����￪ʼ�ģ�����

	
	
	//��ȡ��һ��¼��ʱ�䣬��ѯ�ϱ��ʱ��������ʱ��Ϊ��ʱ��

	RECORDSETHANDLE secondgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();



	QString secondsqlFeederOverload = "select area,station,feeder,work_day,work_index,amprating,max_current,occur_time,overloadrate,note from H_FEEDER_OVERLOAD";
	int queryrowsOfTer = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	ORMTimeStamp last_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle, queryrowsOfTer-1, 7);
	QDateTime lasttimedate = SetTimeFromDB(last_time);
	int lasttimedatetimestamp = lasttimedate.toTime_t()+1;//��һ��
	QDateTime lasttimedateplus = QDateTime::fromTime_t(lasttimedatetimestamp);
	QString strlasttime = lasttimedateplus.toString("yyyy-MM-dd hh:mm:ss");
	qDebug()<<strlasttime;
	qDebug()<<strendtime;
	CPS_ORM_RsFreeRecordSet(secondgatherFeederOverloadSetHandle);


	RECORDSETHANDLE isNullSetHandle = CPS_ORM_RsNewRecordSet();
	QString sqlisNull = "select COUNT(*) from H_FEEDER_OVERLOAD";
	int numrowsOfTers = CPS_ORM_RsLoadData(isNullSetHandle,sqlisNull.toUtf8().data(),dbPoolHandle);
	int items = CPS_ORM_RsGetNumberValue(isNullSetHandle,0,0);//����д���ʱ��Ҫ�õ�
	CPS_ORM_RsFreeRecordSet(isNullSetHandle);
	
	
	
	
	
	
	
	RECORDSETHANDLE queryFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();


	//QString querysqlDPCOptRecord = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020 where ACTIONTIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";


	qDebug()<<MVPointlist.size();
	QString strMVPointlist="";
	QString strMVPointlist1="";
	QString strMVPointlist2="";

	QString querysqlFeederOverload;

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
		querysqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_202005 where SDATE between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+")";



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
		querysqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_201909 where SDATE between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


	}





	
	
	
	int newrowsOfTer = CPS_ORM_RsLoadData(queryFeederOverloadSetHandle,querysqlFeederOverload.toUtf8().data(),dbPoolHandle);


	//������ֻ��֪��д����
	for (int i=0;i<newrowsOfTer;i++)
	{
		
		QAbstractItemModel *feederoverload_model = ui->lineload_tableview->model();
		//�ҵ�ң��
		ObId mvpointid = CPS_ORM_RsGetNumberValue(queryFeederOverloadSetHandle,i,0);


		//�ҵ�psrlink
		ObId psrlink = initConTable->getPSRLink(mvpointid);





		//����
		ObId subcontrolareaid = ToolUtil::findOTypeByObId(psrlink,OT_SubControlArea,OT_PMSRoot);

		QString area_name = QString::number(subcontrolareaid); 
		//���վ
		ObId pmsstationid = ToolUtil::findOTypeByObId(psrlink,OT_PMSStation,OT_PMSRoot);

		QString station_name = QString::number(pmsstationid); 


		//��·����
		ObId pmsbreakerid = ToolUtil::findOTypeByObId(psrlink,OT_PMSBreaker,OT_PMSRoot);
		ObId MemberOfLineid = initConTable->getMemberOfLineLink(pmsbreakerid); 
		QString feeder_name = QString::number(MemberOfLineid);



		//������timestamp
		QStringList strstarttimeList = strstarttime.split(" ");
		QString strstarttimedate = strstarttimeList[0];
		//���Ӱ����short
		short shift = startshift.toShort();
		//¼��ʱ��timestamp(�ǳ���Ҫ)
		QString strinformationcurDateTime = informationcurDateTime.toString("yyyy-MM-dd hh:mm:ss"); 

		//�����
		QModelIndex index4 = feederoverload_model->index(items+i,5);
		QString amprating = feederoverload_model->data(index4).toString();

		//������

		QModelIndex index5 = feederoverload_model->index(items+i,6);
		QString max_current = feederoverload_model->data(index5).toString();
		//����ʱ��
		ORMTimeStamp occurtime = CPS_ORM_RsGetTimeValue(queryFeederOverloadSetHandle,i,1);//FAULTTIME
		QDateTime stroccurtime = SetTimeFromDB(occurtime);
		QString occur_time = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");

		//������
		QModelIndex index6 = feederoverload_model->index(items+i,8);
		QString overloadrate = feederoverload_model->data(index6).toString();


		//��ע
		QModelIndex index7 = feederoverload_model->index(items+i,9);
		QString note_name = feederoverload_model->data(index7).toString();




		
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

	CPS_ORM_RsFreeRecordSet(queryFeederOverloadSetHandle);


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
	

	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO order by START_TIME";
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
	QString firstsqlFAInfo = "select FEEDEROBJECTID, OBJECTID, FAULTTIME, TYPE from FA_SIGNAL where MESSAGETIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
	int rowsOfTer2 = CPS_ORM_RsLoadData(firstgatherFAInfoSetHandle,firstsqlFAInfo.toUtf8().data(),dbPoolHandle);
	AT_Name = database->matchAType("Name");
	for (int i=0;i<rowsOfTer2;i++)
	{
		//FEEDEROBJECTID�¹���Ϣ
		ObId feederid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,0);//FEEDEROBJECTID��
		StringData data;
		ToolUtil::databaseRead(feederid, AT_Name, &data);
		OMString str = (OMString)data;
		QString qstr = QString::fromUtf8(str.c_str());
		QStandardItem* newitem1 = new QStandardItem(qstr);

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
		ObId cbid = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,1);//OBJECTID��
		short fatype = CPS_ORM_RsGetNumberValue(firstgatherFAInfoSetHandle,i,3);//TYPE
		StringData data3;
		ToolUtil::databaseRead(cbid, AT_Name, &data3);
		OMString str3 = (OMString)data3;
		QString qstr3 = QString::fromUtf8(str3.c_str());
		if(fatype == 0)
			qstr3.append(tr("����"));
		else
			qstr3.append(tr("��λ"));
		QStandardItem* newitem6 = new QStandardItem(qstr3);

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



void Information::secondshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *DPCPoint_model = new QStandardItemModel();
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
	QString secondsqlDPCPoint = "select area,station,feeder,switch,work_day,work_index,opt_content,opt_time,operatorinfo,guarderinfo,note from H_DPC_OPTRECORD order by OPT_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherDPCPointSetHandle,secondsqlDPCPoint.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//����ObIdתname
		ObId subcontrolareaidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,0);
		StringData subcontrolareadatafromold;
		ToolUtil::databaseRead(subcontrolareaidfromold, AT_Name, &subcontrolareadatafromold);
		OMString strsubcontrolareafromold = (OMString)subcontrolareadatafromold;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//���վObIdתname
		ObId pmsstationidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,1);
		StringData pmsstationdatafromold;
		ToolUtil::databaseRead(pmsstationidfromold, AT_Name, &pmsstationdatafromold);
		OMString strpmsstationdatafromold = (OMString)pmsstationdatafromold;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//��·ObIdתname
		ObId feederidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,2);
		StringData feederdatafromold;
		ToolUtil::databaseRead(feederidfromold, AT_Name, &feederdatafromold);
		OMString strfeederdatafromold = (OMString)feederdatafromold;
		QStandardItem* item3 = new QStandardItem(QString::fromUtf8(strfeederdatafromold.c_str()));
		item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item3->setForeground(QBrush(QColor(0, 255, 0))); 
		//��������ObIdתname
		ObId switchidfromold = CPS_ORM_RsGetNumberValue(secondgatherDPCPointSetHandle,i,3);
		//qDebug()<<switchid;
		StringData switchdatafromold;
		ToolUtil::databaseRead(switchidfromold, AT_Name, &switchdatafromold);
		OMString strswitchdatafromold = (OMString)switchdatafromold;
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
	QString firstsqlDPCPoint = "select OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020 where ACTIONTIME between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss')";
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





		QList<QStandardItem*> item;
		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7<<item8;

		item << newitem1 << newitem2 <<newitem3 <<newitem4 <<newitem5 <<newitem6 <<newitem7 <<newitem8<<newitem9<<newitem10<<newitem11;

		DPCPoint_model->appendRow(item);


	}
	CPS_ORM_RsFreeRecordSet(firstgatherDPCPointSetHandle);
	
}



void Information::secondshowFeederOverload(QString strstarttime, QString strendtime, QString startshift)
{


	QStandardItemModel *FeederOverload_model = new QStandardItemModel();
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
	QString secondsqlFeederOverload = "select area,station,feeder,work_day,work_index,amprating,max_current,occur_time,overloadrate,note from H_FEEDER_OVERLOAD order by OCCUR_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
	{
		//����ObIdתname
		ObId subcontrolareaidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,0);
		StringData subcontrolareadatafromold;
		ToolUtil::databaseRead(subcontrolareaidfromold, AT_Name, &subcontrolareadatafromold);
		OMString strsubcontrolareafromold = (OMString)subcontrolareadatafromold;
		QStandardItem* item1 = new QStandardItem(QString::fromUtf8(strsubcontrolareafromold.c_str()));
		item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item1->setForeground(QBrush(QColor(0, 255, 0))); 
		//���վObIdתname
		ObId pmsstationidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,1);
		StringData pmsstationdatafromold;
		ToolUtil::databaseRead(pmsstationidfromold, AT_Name, &pmsstationdatafromold);
		OMString strpmsstationdatafromold = (OMString)pmsstationdatafromold;
		QStandardItem* item2 = new QStandardItem(QString::fromUtf8(strpmsstationdatafromold.c_str()));
		item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item2->setForeground(QBrush(QColor(0, 255, 0))); 
		//��·ObIdתname
		ObId feederidfromold = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,2);
		StringData feederdatafromold;
		ToolUtil::databaseRead(feederidfromold, AT_Name, &feederdatafromold);
		OMString strfeederdatafromold = (OMString)feederdatafromold;
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
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_202005 where SDATE between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and SOBJECTID in ("+strMVPointlist+") order by SDATE";



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
		firstsqlFeederOverload = "select SOBJECTID, SDATE, SMAXABSVAL, SMAXABSVALTIME from H_HOURMVPOINTSTAT_201909 where SDATE between to_timestamp('"+strlasttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') and (SOBJECTID in ("+strMVPointlist1+") or SOBJECTID in ("+strMVPointlist2+"))";


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



		QList<QStandardItem*> item;
		item << newitem1 << newitem2 <<newitem3 <<newitem4 <<newitem5 <<newitem6<<newitem7<<newitem8<<newitem9<<newitem10;

		//item << item1 << item2 <<item3 <<item4 <<item5 <<item6 <<item7 <<item8<<item9<<item10<<item11;

		FeederOverload_model->appendRow(item);


	}



	CPS_ORM_RsFreeRecordSet(firstgatherFeederOverloadSetHandle);






	
	
	

}



void Information::secondshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift)
{
	QStandardItemModel *MAINTPlan_model = new QStandardItemModel();
	MAINTPlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("station")));
	MAINTPlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("feeder")));
	MAINTPlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("work_day")));
	MAINTPlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("work_index")));
	MAINTPlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("poweroffstart_time")));
	MAINTPlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("poweroffend_time")));
	MAINTPlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("powerofftype")));
	MAINTPlan_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("poweroffreason")));
	MAINTPlan_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("powerofftime")));
	MAINTPlan_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("note")));
	ui->maintenanceplan_tableview->setModel(MAINTPlan_model);
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);


}













