#include "print.h"
#include "ui_print.h"
#include <QMessageBox>
#include "configuration.h"


#include "qmutex.h"
#include "qprintpreviewdialog.h"
#include "qtextdocument.h"
#include "qtextobject.h"

#include "frmdataout.h"


#include "signout.h"
#include "signininformation.h"
#include "signoutinformation.h"
#include "ui_signininformation.h"
#include "ui_signoutinformation.h"

#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <list>
extern UserMSInterface *g_User;


extern DBPOOLHANDLE dbPoolHandle;

extern  Database *database;

QScopedPointer<Print> Print::self;


Print *Print::Instance()
{
	if (self.isNull()) {
		static QMutex mutex;
		QMutexLocker locker(&mutex);
		if (self.isNull()) {
			self.reset(new Print);
		}
	}

	return self.data();
}

Print::Print(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Print)
{
    

	 ui->setupUi(this);



	 RECORDSETHANDLE startshiftSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlstartshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX=0";//��ǰʱ����Ű�ʱ�����ʱ���
	 int rowsOfTersqlstartshift = CPS_ORM_RsLoadData(startshiftSetHandle,sqlstartshift.toUtf8().data(),dbPoolHandle);
	 int work_start = CPS_ORM_RsGetNumberValue(startshiftSetHandle,0,0);
	 CPS_ORM_RsFreeRecordSet(startshiftSetHandle);
	 QDateTime current_date_time =QDateTime::currentDateTime();
	 int current_date_timetimestamp = current_date_time.toTime_t()-60*60*work_start;//��ǰ������Сʱ
	 QDateTime current_date_timeminus = QDateTime::fromTime_t(current_date_timetimestamp);
	 QString current_date = current_date_timeminus.toString("yyyyMMdd");
	 QDate date = QDate::fromString(current_date,"yyyyMMdd");
	 ui->toTime_QDateEdit->setDate(date);
	 ui->toTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");



	 

	 ui->tocomboBox->clear();
	 
	 RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS order by work_index";
	 int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	 for (int i=0;i<rowsOfWorkName;i++)
	 {
		 std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		 int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		 qDebug()<<QString::fromUtf8(workname.c_str());
		 
		 ui->tocomboBox->addItem(QString::fromUtf8(workname.c_str()));
		 ui->tocomboBox->setItemData(i,workindex,Qt::UserRole);
	 }
	 CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);

	

	 //Ĭ�Ͻ��Ӱ��
	 Configuration configuration;
	 int intDefaultIndex = configuration.whichShift();
	 ui->tocomboBox->setCurrentIndex(intDefaultIndex);

	
    
}
 Print::~Print()

{

    delete ui;

}
 
 QDateTime Print::getfromDateTimedata()
 {	 
	 return fromTime_QDateEdit;
 }
 QString Print::getfromshiftdata()
 {	 
	 return from_shitf;
 }

 QDateTime Print::gettoDateTimedata()
 {
	 return toTime_QDateEdit;
 }
 QString Print::gettoshiftdata()
 {	 
	 return to_shift;
 }

 QString Print::getstrstarttimedata()
 {	 
	 return strstarttime;
 }

 QString Print::getstrendtimedata()
 {	 
	 return strendtime;
 }





 void Print::on_okButton_clicked()
 {
	//ǧ��ɲ����ٶ���������������ˣ���ɾղ�����
	toTime_QDateEdit = ui->toTime_QDateEdit->dateTime();

	int to_shiftindex = ui->tocomboBox->currentIndex();
	QVariant to_shiftV = ui->tocomboBox->itemData(to_shiftindex,Qt::UserRole);
	int to_shift_id = to_shiftV.toInt();
	to_shift = QString::number(to_shift_id);

	Configuration configuration;
	strstarttime = configuration.leftTime(toTime_QDateEdit,to_shift);//��ʱ��
	strendtime = configuration.rightTime(toTime_QDateEdit,to_shift);//��ʱ��

	//��ӡ�¹���Ϣ����Ҫ�Ĳ���
	QList<QString> columnNamesFAaccidentInfo;
	QList<int> columnWidthsFAaccidentInfo;
	frmDataOut::initColumnFAaccidentInfo(columnNamesFAaccidentInfo, columnWidthsFAaccidentInfo);
	QStringList contentFAaccidentInfo;
	QStringList listFAaccidentInfo;
	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by START_TIME";
	int rowsOfTerFAaccidentInfo = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTerFAaccidentInfo;i++)
	{
		//ACCIDENT_INFO�¹���Ϣ
		std::string accident_info = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,0);
		QString item1 = QString::fromUtf8(accident_info.c_str());
		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle,i,1);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QString item2 = strwork_day.toString("yyyy-MM-dd");
		//WORK_INDEX���Ӱ����
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherFAInfoSetHandle,i,2);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QString item3 = QString::fromUtf8(strstartshift.c_str());
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);
		//¼��ʱ��
		//START_TIME����ʱ��
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 4);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QString item5 = strstarttime.toString("yyyy-MM-dd hh:mm:ss");
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
		QString item7 = QString::fromUtf8(reason.c_str());
		//ACKERȷ����
		//std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		//QString item8 = QString::fromUtf8(acker.c_str());
		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		QString item9 = QString::fromUtf8(note.c_str());
		listFAaccidentInfo.append(item1+";"+item2+";"+item3+";"+item5+";"+item7+";"+item9);
	}
	CPS_ORM_RsFreeRecordSet(secondgatherFAInfoSetHandle);
	Print::Instance()->print(QObject::tr("accident_info"), "", columnNamesFAaccidentInfo, columnWidthsFAaccidentInfo, listFAaccidentInfo, true, false, 0, "==", QObject::tr("accident_info"), QPrinter::A4);
	 
	//��ӡ���޼ƻ�����Ҫ�Ĳ���
	QList<QString> columnNamesMaintenancePlan;
	QList<int> columnWidthsMaintenancePlan;
	frmDataOut::initColumnMaintenancePlan(columnNamesMaintenancePlan, columnWidthsMaintenancePlan);
	QStringList contentMaintenancePlan;
	QStringList listMaintenancePlan;
	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherMaintenancePlanSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlMaintenancePlan = "select STATION, FEEDER, WORK_DAY, WORK_INDEX, TIME, START_TIME, END_TIME, TYPE, REASON, POWEROFFTIME, NOTE from H_MAINTENANCE_PLAN where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	int rowsOfTerMaintenancePlan = CPS_ORM_RsLoadData(secondgatherMaintenancePlanSetHandle,secondsqlMaintenancePlan.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTerMaintenancePlan;i++)
	{
		//���վ��Ϣ
		std::string station_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,0);
		QString item1 = QString::fromUtf8(station_info.c_str());
		//��·��Ϣ
		std::string feeder_info = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,1);
		QString item2 = QString::fromUtf8(feeder_info.c_str());
		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle,i,2);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QString item3 = strwork_day.toString("yyyy-MM-dd");
		//WORK_INDEX���Ӱ����
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherMaintenancePlanSetHandle,i,3);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QString item4 = QString::fromUtf8(strstartshift.c_str());
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);
		//¼��ʱ��
		//START_TIMEͣ�翪ʼʱ��
		ORMTimeStamp start_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 5);
		QDateTime strstarttime = SetTimeFromDB(start_time);
		QString item5 = strstarttime.toString("yyyy-MM-dd hh:mm:ss");


		//START_TIMEͣ�����ʱ��
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherMaintenancePlanSetHandle, i, 6);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QString item6 = strendtime.toString("yyyy-MM-dd hh:mm:ss");
		/*
		//END_TIME����ʱ��,ϵͳĬ��ʱ��
		ORMTimeStamp end_time = CPS_ORM_RsGetTimeValue(secondgatherFAInfoSetHandle, i, 5);
		QDateTime strendtime = SetTimeFromDB(end_time);
		QStandardItem* item6 = new QStandardItem(strendtime.toString("yyyy-MM-dd hh:mm:ss"));
		item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item6->setForeground(QBrush(QColor(0, 255, 0))); 
		*/
		//type
		std::string type = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,7);
		QString item7 = QString::fromUtf8(type.c_str());


		//REASON�¹�ԭ��
		std::string reason = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,8);
		QString item8 = QString::fromUtf8(reason.c_str());
		//ACKERȷ����
		//std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		//QString item8 = QString::fromUtf8(acker.c_str());

		//ͣ��ʱ��
		int hour = CPS_ORM_RsGetNumberValue(secondgatherMaintenancePlanSetHandle,i,9);
		QString item9 = QString::number(hour);

		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherMaintenancePlanSetHandle,i,10);
		QString item10 = QString::fromUtf8(note.c_str());


		listMaintenancePlan.append(item1+";"+item2+";"+item3+";"+item4+";"+item5+";"+item6+";"+item7+";"+item8+";"+item9+";"+item10);
	}
	CPS_ORM_RsFreeRecordSet(secondgatherMaintenancePlanSetHandle);
	Print::Instance()->print(QObject::tr("MaintenancePlan"), "", columnNamesMaintenancePlan, columnWidthsMaintenancePlan, listMaintenancePlan, true, false, 0, "==", QObject::tr("MaintenancePlan"), QPrinter::A4);
	 





	//��ӡң����Ϣ����Ҫ�Ĳ���
	QList<QString> columnNamesDPCOptRecord;
	QList<int> columnWidthsDPCOptRecord;
	frmDataOut::initColumnDPCOptRecord(columnNamesDPCOptRecord, columnWidthsDPCOptRecord);
	QStringList contentDPCOptRecord;
	QStringList listDPCOptRecord;
	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherDPCOptRecordSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlDPCOptRecord = "select AREA, STATION, FEEDER, SWITCH, WORK_DAY, WORK_INDEX, TIME, OPT_CONTENT, OPT_TIME, OPERATORINFO, GUARDERINFO, NOTE from H_DPC_OPTRECORD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	int rowsOfTerDPCOptRecord = CPS_ORM_RsLoadData(secondgatherDPCOptRecordSetHandle,secondsqlDPCOptRecord.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTerDPCOptRecord;i++)
	{
		//AREA
		std::string area_info = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,0);
		QString item1 = QString::fromUtf8(area_info.c_str());

		//station
		std::string station_info = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,1);
		QString item2 = QString::fromUtf8(station_info.c_str());
		//feeder
		std::string feeder_info = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,2);
		QString item3 = QString::fromUtf8(feeder_info.c_str());
		//switch
		std::string switch_info = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,3);
		QString item4 = QString::fromUtf8(switch_info.c_str());

		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherDPCOptRecordSetHandle,i,4);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QString item5 = strwork_day.toString("yyyy-MM-dd");
		//WORK_INDEX���Ӱ����
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherDPCOptRecordSetHandle,i,5);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QString item6 = QString::fromUtf8(strstartshift.c_str());
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);
		//¼��ʱ��

		//OPT_CONTENT
		std::string opt_content_info = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,7);
		QString item7 = QString::fromUtf8(opt_content_info.c_str());


		//OPT_TIME
		ORMTimeStamp opt_time = CPS_ORM_RsGetTimeValue(secondgatherDPCOptRecordSetHandle,i,8);
		QDateTime stropttime = SetTimeFromDB(opt_time);
		QString item8 = stropttime.toString("yyyy-MM-dd hh:mm:ss");
		


		//������
		std::string operatorinfo = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,9);
		QString item9 = QString::fromUtf8(operatorinfo.c_str());

		//�໤��
		std::string guarderinfo = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,10);
		QString item10 = QString::fromUtf8(guarderinfo.c_str());
		
		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherDPCOptRecordSetHandle,i,11);
		QString item11 = QString::fromUtf8(note.c_str());



		listDPCOptRecord.append(item1+";"+item2+";"+item3+";"+item4+";"+item5+";"+item6+";"+item7+";"+item8+";"+item9+";"+item10+";"+item11);
	}
	CPS_ORM_RsFreeRecordSet(secondgatherDPCOptRecordSetHandle);
	Print::Instance()->print(QObject::tr("DPCOptRecord"), "", columnNamesDPCOptRecord, columnWidthsDPCOptRecord, listDPCOptRecord, true, false, 0, "==", QObject::tr("DPCOptRecord"), QPrinter::A4);
	 







	//��ӡ��·���ر���Ҫ�Ĳ���
	QList<QString> columnNamesFeederOverload;
	QList<int> columnWidthsFeederOverload;
	frmDataOut::initColumnFeederOverload(columnNamesFeederOverload, columnWidthsFeederOverload);
	QStringList contentFeederOverload;
	QStringList listFeederOverload;
	//���±�����ȥ������������
	RECORDSETHANDLE secondgatherFeederOverloadSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFeederOverload = "select AREA, STATION, FEEDER, WORK_DAY, WORK_INDEX, TIME, AMPRATING, MAX_CURRENT, OCCUR_TIME, OVERLOADRATE, NOTE from H_FEEDER_OVERLOAD where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by TIME";
	int rowsOfTerFeederOverload = CPS_ORM_RsLoadData(secondgatherFeederOverloadSetHandle,secondsqlFeederOverload.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTerFeederOverload;i++)
	{
		//AREA
		std::string area_info = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,0);
		QString item1 = QString::fromUtf8(area_info.c_str());

		//STATION
		std::string station_info = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,1);
		QString item2 = QString::fromUtf8(station_info.c_str());


		//FEEDER
		std::string feeder_info = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,2);
		QString item3 = QString::fromUtf8(feeder_info.c_str());


		//WORK_DAY������	  
		ORMTimeStamp work_day = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle,i,3);
		QDateTime strwork_day = SetTimeFromDB(work_day);
		QString item4 = strwork_day.toString("yyyy-MM-dd");
		//WORK_INDEX���Ӱ����
		int work_index = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,4);
		QString strwork_index = QString::number(work_index);
		RECORDSETHANDLE shiftSetHandle = CPS_ORM_RsNewRecordSet();
		QString strsqlstartshift = "select WORK_NAME from H_WORK_HOURS where WORK_INDEX="+strwork_index;
		int rowsOfTers = CPS_ORM_RsLoadData(shiftSetHandle,strsqlstartshift.toUtf8().data(),dbPoolHandle);
		std::string strstartshift = CPS_ORM_RsGetStringValue(shiftSetHandle,0,0);
		QString item5 = QString::fromUtf8(strstartshift.c_str());
		CPS_ORM_RsFreeRecordSet(shiftSetHandle);
		//¼��ʱ��

		//�����
		int amprating = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,6);
		QString item6 = QString::number(amprating);


		//������
		int max_current = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,7);
		QString item7 = QString::number(max_current);

		//����ʱ��
		ORMTimeStamp occur_time = CPS_ORM_RsGetTimeValue(secondgatherFeederOverloadSetHandle, i, 8);
		QDateTime stroccurtime = SetTimeFromDB(occur_time);
		QString item8 = stroccurtime.toString("yyyy-MM-dd hh:mm:ss");
		
		//������
		double overloadrate = CPS_ORM_RsGetNumberValue(secondgatherFeederOverloadSetHandle,i,9);
		QString item9 = QString::number(overloadrate);
		
		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFeederOverloadSetHandle,i,10);
		QString item10 = QString::fromUtf8(note.c_str());
		listFeederOverload.append(item1+";"+item2+";"+item3+";"+item4+";"+item5+";"+item6+";"+item7+";"+item8+";"+item9+";"+item10);
	}
	CPS_ORM_RsFreeRecordSet(secondgatherFeederOverloadSetHandle);
	Print::Instance()->print(QObject::tr("FeederOverload"), "", columnNamesFeederOverload, columnWidthsFeederOverload, listFeederOverload, true, false, 0, "==", QObject::tr("FeederOverload"), QPrinter::A4);
	 
	 
	 
	 
	 









 }



 void Print::printView(QPrinter* printer)//�����ӡģ�庯��
 {
	 
	 
	 
	 
	 
	 /*
	 QString strStream,strTitle,strHead,strTime,strLeader,strfromtotime;//������tableview���ݣ�Ĭ��PDF�ļ��ı������֣�tableview�ı��⣬��ӡʱ�䣬�쵼ǩ��

	 QTextStream out(&strStream);



	 QDateTime dateTime(QDateTime::currentDateTime());
	 QString time = dateTime.toString("_yyyyMMddhhmmss");

	 strTitle=tr("iHandoverRecord")+time;

	 strHead=tr("iHandoverRecord");

	 strfromtotime = tr("printfrom")+strstarttime+tr("printto")+strendtime;

	 strLeader=tr("Leader");

	 //const int rowCount = ui->DPCPoint_tableview->model()->rowCount();
	 //const int columnCount = ui->DPCPoint_tableview->model()->columnCount();
	 out  << "<html>\n"
		 "<head>\n"
		 "<meta Content=\"Textml; charset=Windows-1251\">\n"
		 << QString("<title>%1</title>\n").arg(strTitle)//�����PDFĬ���ļ���
		 << "</head>\n"
		 "<body bgcolor=#ffffff link=#5000A0>\n"
		 //<<"<h1 bgcolor=#ffff00 text-align=center>"<<QString("%1").arg(strHead)<<"</h1>\n"//PDF�ı���


		 << QString("<h1 style='text-align:center'><span style='font-size:20.0pt;font-family:����'>%1</span></h1>\n")
		 .arg(strHead)
		 //<< QString("<h1 bgcolor=#ffff00>%1</h1>\n").arg(strHead)//PDF�ı���

		  //<< QString("<h1 text-align=right>%1</h1>\n").arg(strfromtotime)//��ӡʱ������
		 << "<table border=1 cellspacing=0 cellpadding=2>\n";
	 // ��ͷ
	 out  << "<thead><tr bgcolor=#f0f0f0>";
	 
	 out  << "</tr></thead>\n";
	 // ������
	 
	 out  << "</table>\n"



		 <<QString("<h1 style='text-align:center'>%1</h1>\n").arg(strLeader)

		 <<



		 "</body>\n"
		 "</html>\n";
	 QTextDocument *document = new QTextDocument();
	 document->setHtml(strStream);
	 document->print(printer);


	 */


	 QTextDocument textDocument;
	 textDocument.setHtml(html.join(""));
	 textDocument.setPageSize(printer->pageRect().size());
	 textDocument.print(printer);
 }












 void Print::print(const QString &title, const QString &subTitle, const QList<QString> &columnNames,
	 const QList<int> &columnWidths, const QStringList &content, bool landscape, bool check,
	 int checkColumn, const QString &checkType, const QString &checkValue, const QPrinter::PageSize &pageSize)
 {
	 //������������
	 int columnCount = columnNames.count();
	 int rowCount = content.count();

	 //���ԭ������,ȷ��ÿ�ζ����µ�����
	 html.clear();

	 //���ʼ
	 html.append("<table border='0.5' cellspacing='0' cellpadding='3'>");

	 //����ռһ��,������ʾ
	 if (title.length() > 0) {
		 html.append(QString("<tr><td align='center' style='vertical-align:middle;font-weight:bold;' colspan='%1'>").arg(columnCount));
		 html.append(title);
		 html.append("</td></tr>");
	 }

	 //������ռһ��,�������ʾ
	 if (subTitle.length() > 0) {
		 html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>").arg(columnCount));
		 html.append(subTitle);
		 html.append("</td></tr>");
	 }

	 //ѭ��д���ֶ���,�ֶ���ռһ��,������ʾ
	 if (columnCount > 0) {
		 html.append("<tr>");

		 for (int i = 0; i < columnCount; i++) {
			 html.append(QString("<td width='%1' bgcolor='lightgray' align='center' style='vertical-align:middle;'>").arg(columnWidths.at(i)));
			 html.append(columnNames.at(i));
			 html.append("</td>");
		 }

		 html.append("</tr>");
	 }

	 qDebug()<<html;

	 //ѭ��һ���й�������
	 for (int i = 0; i < rowCount; i++) {
		 QStringList value = content.at(i).split(";");
		 html.append("<tr>");

		 //��������,��������˹�������,�򽫷�������������ͻ����ɫ��ʾ
		 bool existCheck = false;

		 if (check) {
			 if (checkType == "==") {
				 if (value.at(checkColumn) == checkValue) {
					 existCheck = true;
				 }
			 } else if (checkType == ">") {
				 if (value.at(checkColumn) > checkValue) {
					 existCheck = true;
				 }
			 } else if (checkType == ">=") {
				 if (value.at(checkColumn) >= checkValue) {
					 existCheck = true;
				 }
			 } else if (checkType == "<") {
				 if (value.at(checkColumn) < checkValue) {
					 existCheck = true;
				 }
			 } else if (checkType == "<=") {
				 if (value.at(checkColumn) <= checkValue) {
					 existCheck = true;
				 }
			 } else if (checkType == "!=") {
				 if (value.at(checkColumn) != checkValue) {
					 existCheck = true;
				 }
			 }
		 }

		 if (existCheck) {
			 for (int j = 0; j < columnCount; j++) {
				 html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'><font color='red'>").arg(columnWidths.at(j)));
				 html.append(value.at(j));
				 html.append("</font></td>");
			 }
		 } else {
			 for (int j = 0; j < columnCount; j++) {
				 html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'>").arg(columnWidths.at(j)));
				 html.append(value.at(j));
				 html.append("</td>");
			 }
		 }

		 html.append("</tr>");
	 }

	 html.append("</table>");

	 //���ô�ӡ����ӡ
	 QPrinter printer;
	 //���������ʽ
	 printer.setOutputFormat(QPrinter::NativeFormat);
	 //����ֽ�Ź��
	 printer.setPageSize(pageSize);

	 //���ú�������ҳ�߾�
	 if (landscape) {
		 printer.setOrientation(QPrinter::Landscape);
		 printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);
	 } else {
		 printer.setOrientation(QPrinter::Portrait);
		 printer.setPageMargins(10, 10, 10, 11, QPrinter::Millimeter);
	 }

	 QPrintPreviewDialog preview(&printer);
	 preview.setStyleSheet("QToolButton{background:none;margin:2px;padding:0px;border-width:0px;border-radius:0px;}QLineEdit{border-width:0px;}");
	 preview.setWindowTitle(QObject::tr("PrintPreview"));
	 preview.setWindowFlags(Qt::WindowStaysOnTopHint);
	 connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printView(QPrinter *)));
	 preview.showMaximized();
	 preview.exec();
 }

 void Print::print(const QString &title, const QList<QString> &columnNames, const QList<int> &columnWidths,
	 const QStringList &subTitle1, const QStringList &subTitle2,
	 const QStringList &content, bool landscape, bool check,
	 int checkColumn, const QString &checkType, const QString &checkValue, const QPrinter::PageSize &pageSize)
 {
	 //��������
	 int columnCount = columnNames.count();

	 //���ԭ������,ȷ��ÿ�ζ����µ�����
	 html.clear();

	 //���ʼ
	 html.append("<table border='0.5' cellspacing='0' cellpadding='3'>");

	 //����ռһ��,������ʾ
	 if (title.length() > 0) {
		 html.append(QString("<tr><td align='center' style='vertical-align:middle;font-weight:bold;' colspan='%1'>")
			 .arg(columnCount));
		 html.append(title);
		 html.append("</td></tr>");
	 }

	 //ѭ����Ӹ�����/�ֶ���/����
	 int count = content.count();

	 for (int i = 0; i < count; i++) {
		 //�Ӹ����и���
		 html.append(QString("<tr><td colspan='%1'>").arg(columnCount));
		 html.append("</td></tr>");

		 //������1
		 if (subTitle1.count() > 0 && subTitle1.count() > i) {
			 if (subTitle1.at(i).length() > 0) {
				 html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>")
					 .arg(columnCount));
				 html.append(subTitle1.at(i));
				 html.append("</td></tr>");
			 }
		 }

		 //������2
		 if (subTitle2.count() > 0 && subTitle2.count() > i) {
			 if (subTitle2.at(i).length() > 0) {
				 html.append(QString("<tr><td align='left' style='vertical-align:middle;' colspan='%1'>")
					 .arg(columnCount));
				 html.append(subTitle2.at(i));
				 html.append("</td></tr>");
			 }
		 }

		 //�������ֶ�����
		 if (columnCount > 0) {
			 html.append("<tr>");

			 for (int i = 0; i < columnCount; i++) {
				 html.append(QString("<td width='%1' bgcolor='lightgray' align='center' style='vertical-align:middle;'>")
					 .arg(columnWidths.at(i)));
				 html.append(columnNames.at(i));
				 html.append("</td>");
			 }

			 html.append("</tr>");
		 }

		 QStringList list = content.at(i).split(";");

		 //����������
		 int rowCount = list.count();

		 for (int j = 0; j < rowCount; j++) {
			 html.append("<tr>");

			 QString temp = list.at(j);
			 QStringList value = temp.split("|");
			 int valueCount = value.count();

			 //��������,��������˹�������,�򽫷�������������ͻ����ɫ��ʾ
			 bool existCheck = false;

			 if (check) {
				 if (checkType == "==") {
					 if (value.at(checkColumn) == checkValue) {
						 existCheck = true;
					 }
				 } else if (checkType == ">") {
					 if (value.at(checkColumn) > checkValue) {
						 existCheck = true;
					 }
				 } else if (checkType == ">=") {
					 if (value.at(checkColumn) >= checkValue) {
						 existCheck = true;
					 }
				 } else if (checkType == "<") {
					 if (value.at(checkColumn) < checkValue) {
						 existCheck = true;
					 }
				 } else if (checkType == "<=") {
					 if (value.at(checkColumn) <= checkValue) {
						 existCheck = true;
					 }
				 } else if (checkType == "!=") {
					 if (value.at(checkColumn) != checkValue) {
						 existCheck = true;
					 }
				 }
			 }

			 if (existCheck) {
				 for (int k = 0; k < valueCount - 1; k++) {
					 html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'><font color='red'>").arg(columnWidths.at(k)));
					 html.append(value.at(k));
					 html.append("</font></td>");
				 }
			 } else {
				 for (int k = 0; k < valueCount; k++) {
					 html.append(QString("<td width='%1' align='center' style='vertical-align:middle;'>").arg(columnWidths.at(k)));
					 html.append(value.at(k));
					 html.append("</td>");
				 }
			 }

			 html.append("</tr>");
		 }
	 }

	 html.append("</table>");

	 //���ô�ӡ����ӡ
	 QPrinter printer;
	 //���������ʽ
	 printer.setOutputFormat(QPrinter::NativeFormat);
	 //����ֽ�Ź��
	 printer.setPageSize(pageSize);

	 //���ú�������ҳ�߾�
	 if (landscape) {
		 printer.setOrientation(QPrinter::Landscape);
		 printer.setPageMargins(10, 10, 10, 10, QPrinter::Millimeter);
	 } else {
		 printer.setOrientation(QPrinter::Portrait);
		 printer.setPageMargins(10, 10, 10, 11, QPrinter::Millimeter);
	 }

	 QPrintPreviewDialog preview(&printer);
	 preview.setStyleSheet("QToolButton{background:none;margin:2px;padding:0px;border-width:0px;border-radius:0px;}QLineEdit{border-width:0px;}");
	 preview.setWindowTitle(QObject::tr("print preview"));
	 preview.setWindowFlags(Qt::WindowStaysOnTopHint);
	 connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printView(QPrinter *)));
	 preview.showMaximized();
	 preview.exec();
 }




 