#include "print.h"
#include "ui_print.h"
#include <QMessageBox>


#include "signout.h"
#include "information.h"
#include "ui_information.h"

#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"

#include <list>
extern UserMSInterface *g_User;


extern DBPOOLHANDLE dbPoolHandle;

extern  Database *database;

Print::Print(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Print)
{
    

	 ui->setupUi(this);


	 

	 ui->tocomboBox->clear();
	 ui->fromcomboBox->clear();
	 RECORDSETHANDLE WorkNameSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlWorkName = "select WORK_NAME, WORK_INDEX from H_WORK_HOURS";
	 int rowsOfWorkName = CPS_ORM_RsLoadData(WorkNameSetHandle,sqlWorkName.toUtf8().data(),dbPoolHandle);
	 for (int i=0;i<rowsOfWorkName;i++)
	 {
		 std::string workname = CPS_ORM_RsGetStringValue(WorkNameSetHandle,i,0);
		 int workindex = CPS_ORM_RsGetNumberValue(WorkNameSetHandle,i,1);
		 qDebug()<<QString::fromUtf8(workname.c_str());
		 ui->fromcomboBox->addItem(QString::fromUtf8(workname.c_str()));
		 ui->fromcomboBox->setItemData(i,workindex,Qt::UserRole);
		 ui->tocomboBox->addItem(QString::fromUtf8(workname.c_str()));
		 ui->tocomboBox->setItemData(i,workindex,Qt::UserRole);
	 }
	 CPS_ORM_RsFreeRecordSet(WorkNameSetHandle);

	 QDateTime current_date_time =QDateTime::currentDateTime();
	 QString current_date =current_date_time.toString("yyyyMMdd");
	 QDate date = QDate::fromString(current_date,"yyyyMMdd");
	 ui->fromTime_QDateEdit->setDate(date);
	 ui->fromTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");
	 ui->toTime_QDateEdit->setDate(date);
	 ui->toTime_QDateEdit->setDisplayFormat("yyyy/MM/dd");

 



	
    
}
 Print::~Print()

{

    delete ui;

}
 /*
 void Print::showInformationDialog()
 {


	 //测试万金油
	 //QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	 Information information;

	 information.exec();


 }
 */
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

	 //千万可不敢再定义变量数据类型了，造成菊部变量




	 fromTime_QDateEdit = ui->fromTime_QDateEdit->dateTime();

	 QString strfromtime = fromTime_QDateEdit.toString("yyyy-MM-dd");
	 


	 toTime_QDateEdit = ui->toTime_QDateEdit->dateTime();

	 QString strtotime = toTime_QDateEdit.toString("yyyy-MM-dd");



	 int from_shitfindex = ui->fromcomboBox->currentIndex();
	 QVariant from_shitfV = ui->fromcomboBox->itemData(from_shitfindex,Qt::UserRole);
	 int from_shitf_id = from_shitfV.toInt();
	 from_shitf = QString::number(from_shitf_id);


	 int to_shitfindex = ui->tocomboBox->currentIndex();
	 QVariant to_shitfV = ui->tocomboBox->itemData(to_shitfindex,Qt::UserRole);
	 int to_shitf_id = to_shitfV.toInt();
	 to_shitf = QString::number(to_shitf_id);





	 //QString strstarttime;//初始化左时间
	 RECORDSETHANDLE fromshiftSetHandle = CPS_ORM_RsNewRecordSet();
	 QString sqlfromshift = "select WOR_START from H_WORK_HOURS where WORK_INDEX="+from_shitf;
	 int rowsOfTersqlfromshift = CPS_ORM_RsLoadData(fromshiftSetHandle,sqlfromshift.toUtf8().data(),dbPoolHandle);
	 int fromHour = CPS_ORM_RsGetNumberValue(fromshiftSetHandle,0,0);
	 QString strfromHour = QString::number(fromHour);
	 if (strfromHour.length()==1)
		 strstarttime = QString("%1 0%2:00:00")
		 .arg(strfromtime)
		 .arg(strfromHour);
	 else
		 strstarttime = QString("%1 %2:00:00")
		 .arg(strfromtime)
		 .arg(strfromHour);
	 //qDebug()<<work_start;
	 //QString strstarttime = strstartdate+' '+strhourminsec;
	 CPS_ORM_RsFreeRecordSet(fromshiftSetHandle);



	 //QString strendtime;//初始化左时间
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



	 qDebug()<<strstarttime;
	 qDebug()<<strendtime;


	


	
	 QPrinter printer(QPrinter::ScreenResolution);
	 QPrintPreviewDialog preview(&printer);
	 preview.setWindowTitle("prepareprint");
	 bool a = connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printReport(QPrinter*)));

	 qDebug()<<a;

	 preview.exec();










 }



 void Print::printReport(QPrinter* printer)//定义打印模板函数
 {
	 QString strStream,strTitle,strHead,strTime,strLeader,strfromtotime;//定义了tableview内容，默认PDF文件的保存名字，tableview的标题，打印时间，领导签字

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
		 << QString("<title>%1</title>\n").arg(strTitle)//保存的PDF默认文件名
		 << "</head>\n"
		 "<body bgcolor=#ffffff link=#5000A0>\n"
		 //<<"<h1 bgcolor=#ffff00 text-align=center>"<<QString("%1").arg(strHead)<<"</h1>\n"//PDF的标题


		 << QString("<h1 style='text-align:center'><span style='font-size:20.0pt;font-family:宋体'>%1</span></h1>\n")
		 .arg(strHead)
		 //<< QString("<h1 bgcolor=#ffff00>%1</h1>\n").arg(strHead)//PDF的标题

		  //<< QString("<h1 text-align=right>%1</h1>\n").arg(strfromtotime)//打印时间区间
		 << "<table border=1 cellspacing=0 cellpadding=2>\n";
	 // 表头
	 out  << "<thead><tr bgcolor=#f0f0f0>";
	 
	 out  << "</tr></thead>\n";
	 // 表内容
	 
	 out  << "</table>\n"



		 <<QString("<h1>%1</h1>\n").arg(strLeader)

		 <<



		 "</body>\n"
		 "</html>\n";
	 QTextDocument *document = new QTextDocument();
	 document->setHtml(strStream);
	 document->print(printer);
 }




 