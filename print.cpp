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
	 QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS";
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
	 return to_shitf;
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

	 
	 
	 /*
	 QStringList content;
	 for (int i = 0; i < 100; i++) {
		 QStringList list;
		 for (int j = 0; j < 5; j++) {
			 //QString text = ui->table1->item(i, j)->text();

			 QString text = "abc";

			 list.append(text);
		 }

		 content.append(list.join(";"));
	 }

	 */
	 
	 

	 QList<QString> columnNames;
	 QList<int> columnWidths;



	 frmDataOut::initColumnFAaccidentInfo(columnNames, columnWidths);



	 
	  QStringList content;

	 
	 QStringList list;
	 //���±�����ȥ������������
	RECORDSETHANDLE secondgatherFAInfoSetHandle = CPS_ORM_RsNewRecordSet();
	QString secondsqlFAInfo = "select ACCIDENT_INFO, WORK_DAY, WORK_INDEX, TIME, START_TIME, REASON, NOTE from H_FA_ACCIDENTINFO where TIME between to_timestamp('"+strstarttime+"','yyyy-MM-dd hh24:mi:ss') and to_timestamp('"+strendtime+"','yyyy-MM-dd hh24:mi:ss') order by START_TIME";
	int rowsOfTer = CPS_ORM_RsLoadData(secondgatherFAInfoSetHandle,secondsqlFAInfo.toUtf8().data(),dbPoolHandle);
	for (int i=0;i<rowsOfTer;i++)
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
		


		/*
		//ACKERȷ����
		std::string acker = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,7);
		QStandardItem* item8 = new QStandardItem(QString::fromUtf8(acker.c_str()));
		item8->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item8->setForeground(QBrush(QColor(0, 255, 0))); 
		*/

	 
		//NOTE��ע
		std::string note = CPS_ORM_RsGetStringValue(secondgatherFAInfoSetHandle,i,6);
		QString item9 = QString::fromUtf8(note.c_str());
		



		list.append(item1+";"+item2+";"+item3+";"+item5+";"+item7+";"+item9);
		//list.append(item1);


		//list << item1 <<";"<< item2 <<";"<<item3 <<";"<<item5 <<";"<< item7<<";" <<item9;


		



	}

	//content.append(list);





	 Print::Instance()->print(QObject::tr("accident_info"), "", columnNames, columnWidths, list, true, false, 7, "==", QObject::tr("accident_info"), QPrinter::A4);
	 //Print::Instance()->print("Title", "Subtitle", columnNames, columnWidths, content, false, false, 3, "==", "a");
	 
	 
	 Print::Instance()->print(QObject::tr("accident_info2"), "", columnNames, columnWidths, list, true, false, 7, "==", QObject::tr("accident_info2"), QPrinter::A4);

	 

	 
	 
	 
	 //ǧ��ɲ����ٶ���������������ˣ���ɾղ�����




	
	 


	 toTime_QDateEdit = ui->toTime_QDateEdit->dateTime();

	 QString strtotime = toTime_QDateEdit.toString("yyyy-MM-dd");



	 


	 int to_shitfindex = ui->tocomboBox->currentIndex();
	 QVariant to_shitfV = ui->tocomboBox->itemData(to_shitfindex,Qt::UserRole);
	 int to_shitf_id = to_shitfV.toInt();
	 to_shitf = QString::number(to_shitf_id);





	 



	 //QString strendtime;//��ʼ����ʱ��
	 RECORDSETHANDLE toshiftSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqltoshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX="+to_shitf;
	 int rowsOfTersqltoshift = CPS_ORM_RsLoadData(toshiftSetHandle,sqltoshift.toUtf8().data(),dbPoolHandle);
	 int toHour = CPS_ORM_RsGetNumberValue(toshiftSetHandle,0,0);
	 QString strtoHour = QString::number(toHour);
	 if (strtoHour.length()==1)
		 strendtime = QString("%1 0%2:00:00")
		 .arg(strtotime)
		 .arg(strtoHour);
	 else
		 strendtime = QString("%1 %2:00:00")
		 .arg(strtotime)
		 .arg(strtoHour);
	 //qDebug()<<work_start;
	 //QString strstarttime = strstartdate+' '+strhourminsec;
	 CPS_ORM_RsFreeRecordSet(toshiftSetHandle);



	 
	 qDebug()<<strendtime;


	

	 /*
	
	 QPrinter printer(QPrinter::ScreenResolution);
	 QPrintPreviewDialog preview(&printer);
	 preview.setWindowTitle("prepareprint");
	 bool a = connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printView(QPrinter*)));


	 
	 qDebug()<<a;

	 preview.exec();

	 */








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
	 preview.setWindowTitle("��ӡԤ��");
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




 