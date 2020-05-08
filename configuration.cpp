#include "configuration.h"
#include "ui_configuration.h"

#include "confirm.h"
#include "ui_confirm.h"
#include "signin.h"
#include "signout.h"
#include "information.h"
#include "mainwindow.h"
#include "initializtion.h"
#include "initConTable.h"
#include "ToolUtil.h"
#include <QString>
#include <QDebug>
#include <QList>
#include <QMessageBox>


extern DBPOOLHANDLE dbPoolHandle;
extern UserMSInterface *g_User;
extern  Database *database;





Configuration::Configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);
	 ui->tableWidgetConfiguration->setHorizontalHeaderLabels(QStringList()<<QObject::tr("workshift_index")<<QObject::tr("workshift_name")<<QObject::tr("wor_start")<<QObject::tr("wor_end"));

	 ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
	 ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
	 ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
	 ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);

	 ui->tableWidgetConfiguration->setSelectionBehavior(QAbstractItemView::SelectRows); 
	 connect(ui->Add,SIGNAL(clicked()),this,SLOT(on_btn_Add_clicked()));

	 connect(ui->Del,SIGNAL(clicked()),this,SLOT(on_btn_Del_clicked()));

	 connect(ui->okButton,SIGNAL(clicked()),this,SLOT(on_btn_okButton_clicked()));


}

Configuration::~Configuration()
{
    delete ui;
}





void Configuration::on_btn_Add_clicked()
{
	//    QAbstractItemModel *model = ui->tableWidget->model();
	//     model->insertRow(model->rowCount());
	int cols=ui->tableWidgetConfiguration->columnCount();
	int rows=ui->tableWidgetConfiguration->rowCount();
	//qDebug()<<rows;
	ui->tableWidgetConfiguration->insertRow(rows);
	for(int i=0;i<cols;i++)
	{
		//ui->tableWidgetConfiguration->setItem(rows,i,new QTableWidgetItem("new"+QString::number(rows)));
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
	
	//int cols=ui->tableWidgetConfiguration->columnCount();

	//RECORDSETHANDLE ConfigurationbeforeSetHandle = CPS_ORM_RsNewRecordSet();
	QString tableWidgetConfiguration = "delete from H_WORK_HOURS";
	InitConTable::executeSQL(tableWidgetConfiguration,0);
	//CPS_ORM_RsFreeRecordSet(ConfigurationbeforeSetHandle);

	int rows=ui->tableWidgetConfiguration->rowCount();

	
	for (int i=0;i<rows;i++)
	{



		QString work_index = ui->tableWidgetConfiguration->item(i,0)->text();//取出班序号

		QString work_name = ui->tableWidgetConfiguration->item(i,1)->text();//取出班名称

		QString wor_start = ui->tableWidgetConfiguration->item(i,2)->text();//取出班开始时段(24小时制)

		QString wor_end = ui->tableWidgetConfiguration->item(i,3)->text();//取出班结束时段(24小时制)





		

		

		//RECORDSETHANDLE ConfigurationSetHandle = CPS_ORM_RsNewRecordSet();
		//拼接交接班交接记录总表的sql语句
		QString configurationsql = QString("INSERT INTO H_WORK_HOURS (WORK_INDEX,WORK_NAME,WOR_START,WOR_END) VALUES (%1,'%2',%3,%4)")
			.arg(work_index)
			.arg(work_name)
			.arg(wor_start)
			.arg(wor_end);
		//调用另一个cpp里的static声明过的函数，执行sql语句，经常用到
		InitConTable::executeSQL(configurationsql,0);
		//CPS_ORM_RsFreeRecordSet(ConfigurationSetHandle);


		









	}

	
	
	
}







