#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "initializtion.h"
#include<QStandardItemModel>



extern DBPOOLHANDLE dbPoolHandle;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


	

	showDPCPoint();

    
	connect(ui->actionExport,SIGNAL(triggered()),this,SLOT(exportcsv()));//connect没毛病

	
	


	
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::showDPCPoint()
{
	
	
	
	QStandardItemModel *DPCPoint_model = new QStandardItemModel();
    DPCPoint_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("区域")));
    DPCPoint_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("变电站")));
    DPCPoint_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("线路名称")));
    DPCPoint_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("开关描述")));
    DPCPoint_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("操作内容")));
	DPCPoint_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("操作人")));
	DPCPoint_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("监护人")));
	DPCPoint_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("备注")));
	DPCPoint_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("备注")));
	DPCPoint_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("备注")));
	
	//利用setModel()方法将数据模型与QTableView绑定
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableView平均分配列宽
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
    //ui->DPCPonit_tableview->verticalHeader()->hide(); 



	
 
	
	
	RECORDSETHANDLE gatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	//QString sqlDPCPoint = "select SERVICEID, OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, STATION, AREA, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020";

	QString sqlDPCPoint = "select SERVICEID, OBID from H_DPCPOINT_RECORD_2020";
		int rowsOfTer = CPS_ORM_RsLoadData(gatherDPCPointSetHandle,sqlDPCPoint.toUtf8().data(),dbPoolHandle);
		for (int i=0;i<rowsOfTer;i++)
		{
			ObId serviceid = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,0);
			QStandardItem* item1 = new QStandardItem(QString::number(serviceid));
			
			ObId obid = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,1);
			QStandardItem* item2 = new QStandardItem(QString::number(obid));

			//ObId dpcaction = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,2);
			QStandardItem* item3 = new QStandardItem(QString::number(obid));

			//ObId actiontime = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,3);
			QStandardItem* item4 = new QStandardItem(QString::number(obid));

			//ObId actionresult = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,4);
			QStandardItem* item5 = new QStandardItem(QString::number(obid));

			//ObId station = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,5);
			QStandardItem* item6 = new QStandardItem(QString::number(obid));

			//ObId area = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,6);
			QStandardItem* item7 = new QStandardItem(QString::number(obid));

			//ObId operatorinfo = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,7);
			QStandardItem* item8 = new QStandardItem(QString::number(obid));

			//ObId guarderinfo = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,8);
			QStandardItem* item9 = new QStandardItem(QString::number(obid));

			//ObId obid = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,9);
			QStandardItem* item10 = new QStandardItem("这里是备注");
			
			QList<QStandardItem*> item;
            item << item1 << item2 << item3 << item4 << item5 << item6 << item7 << item8 << item9 << item10;
            DPCPoint_model->appendRow(item);
	
		}
		CPS_ORM_RsFreeRecordSet(gatherDPCPointSetHandle);





}

//这里可以写slots函数
void MainWindow::exportcsv()
{

	
	//判断用户停留在哪个tableview，编号从零开始
	int index = ui->tabWidget->currentIndex();

	if (index == 0)//用户停留在事故信息tableview
	{
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File")," ",tr("file (*.csv)"));
	
	if (fileName.isEmpty())
 return;

 QFile file(fileName);
 
 if (file.open(QIODevice::WriteOnly))
 {
 QTextStream stream(&file);

 QAbstractItemModel* view = ui->accidentinfo_tableview->model();//肯定导出第一个tableview啦

 int cc=view->columnCount();
 
 QStringList list;
 for (int i=0;i<cc;i++)
 {
 list<< view->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
 }
 stream<< list.join(",")<<"\r\n";
 
 
 for (int i=0;i<view->rowCount();i++)
 {
 list.clear();
for (int j=0;j<cc;j++)
{
 list<<view->index(i,j).data().toString();
 }
 stream<< list.join(",")<<"\r\n";
 }
 QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(fileName), tr("确定"));
file.close();
 }
	
	}

	if (index == 1)//用户停留在检修计划tableview
	{
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File")," ",tr("file (*.csv)"));
	
	if (fileName.isEmpty())
 return;

 QFile file(fileName);
 
 if (file.open(QIODevice::WriteOnly))
 {
 QTextStream stream(&file);

 QAbstractItemModel* view = ui->maintenanceplan_tableview->model();

 int cc=view->columnCount();
 
 QStringList list;
 for (int i=0;i<cc;i++)
 {
 list<< view->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
 }
 stream<< list.join(",")<<"\r\n";
 
 
 for (int i=0;i<view->rowCount();i++)
 {
 list.clear();
for (int j=0;j<cc;j++)
{
 list<<view->index(i,j).data().toString();
 }
 stream<< list.join(",")<<"\r\n";
 }
 QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(fileName), tr("确定"));
file.close();
 }
	
	}
	if (index == 2)//用户停留在遥控记录tableview
	{
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File")," ",tr("file (*.csv)"));
	
	if (fileName.isEmpty())
 return;

 QFile file(fileName);
 
 if (file.open(QIODevice::WriteOnly))
 {
 QTextStream stream(&file);

 QAbstractItemModel* view = ui->DPCPoint_tableview->model();

 int cc=view->columnCount();
 
 QStringList list;
 for (int i=0;i<cc;i++)
 {
 list<< view->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
 }
 stream<< list.join(",")<<"\r\n";
 
 
 for (int i=0;i<view->rowCount();i++)
 {
 list.clear();
for (int j=0;j<cc;j++)
{
 list<<view->index(i,j).data().toString();
 }
 stream<< list.join(",")<<"\r\n";
 }


 QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(fileName), tr("确定"));
file.close();
 }
	
	}
	if (index == 3)//用户停留在线路负载tableview
	{
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File")," ",tr("file (*.csv)"));
	
	if (fileName.isEmpty())
 return;

 QFile file(fileName);
 
 if (file.open(QIODevice::WriteOnly))
 {
 QTextStream stream(&file);

 QAbstractItemModel* view = ui->lineload_tableview->model();

 int cc=view->columnCount();
 
 QStringList list;
 for (int i=0;i<cc;i++)
 {
 list<< view->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
 }
 stream<< list.join(",")<<"\r\n";
 
 for (int i=0;i<view->rowCount();i++)
 {
 list.clear();
for (int j=0;j<cc;j++)
{
 list<<view->index(i,j).data().toString();
 }
 stream<< list.join(",")<<"\r\n";
 }
 QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(fileName), tr("确定"));
file.close();
 }
	
	}


	
}

