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

#include <QCoreApplication>

extern DBPOOLHANDLE dbPoolHandle;




Information::Information(QWidget *parent) :
    QDialog(parent),
	
	ui(new Ui::Information)
{
    

	 ui->setupUi(this);
	 
    //setWindowTitle("signout");


	 

	
	 
	

	 showFAaccidentInfo();
	 showMaintenancePlan();
	 showDPCOptRecord();
	 showFeederOverload();

	 


	 /*
	 SignOut *signout = new SignOut;
	 connect(signout,SIGNAL(sendsignoutcurDateTimeData(QDateTime)),this,SLOT(receivesignoutcurDateTimeData(QDateTime)));
	 bool a = connect(signout,SIGNAL(sendsignoutnameData(QString)),this,SLOT(receivesignoutnameData(QString)));
	 if(a)
		 QMessageBox::information(this,"",QString::number(2222));
	 else
		 QMessageBox::information(this,"","2");
	 connect(signout,SIGNAL(sendsignoutTime_QDateEditData(QDateTime)),this,SLOT(receivesignoutTime_QDateEditData(QDateTime)));
	 connect(signout,SIGNAL(sendsignout_shiftData(QString)),this,SLOT(receivesignout_shiftData(QString)));

	 */
	 

	 

	 

	 

	
	
    
}
 Information::~Information()

{

    delete ui;

}



 void Information::showFAaccidentInfo()
{


	

	QStandardItemModel *FAInfo_model = new QStandardItemModel();
	FAInfo_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("������Ϣ")));
    FAInfo_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("����ʱ��")));
    FAInfo_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("�¹���Ϣ")));
    FAInfo_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("��ע")));
	ui->accidentinfo_tableview->setModel(FAInfo_model);
	ui->accidentinfo_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
		
      }

void Information::showMaintenancePlan()
{
	QStandardItemModel *MAINTPlan_model = new QStandardItemModel();
	MAINTPlan_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("���վ")));
    MAINTPlan_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("��·����")));
    MAINTPlan_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("ͣ�翪ʼʱ��")));
    MAINTPlan_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("ͣ�����ʱ��")));
    MAINTPlan_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("ͣ������")));
	MAINTPlan_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("ͣ��ԭ��")));
	MAINTPlan_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("ͣ��ʱ��")));
	ui->maintenanceplan_tableview->setModel(MAINTPlan_model);
	ui->maintenanceplan_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	
}

void Information::showDPCOptRecord()
{
	QStandardItemModel *DPCPoint_model = new QStandardItemModel();
	DPCPoint_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("����")));
	DPCPoint_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("���վ")));
	DPCPoint_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("��·����")));
	DPCPoint_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("��������")));
	DPCPoint_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("��������")));
	DPCPoint_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("������")));
	DPCPoint_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("�໤��")));
	DPCPoint_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("��ע")));
	DPCPoint_model->setHorizontalHeaderItem(8, new QStandardItem(QObject::tr("��ע")));
	DPCPoint_model->setHorizontalHeaderItem(9, new QStandardItem(QObject::tr("��ע")));

	//����setModel()����������ģ����QTableView��
	ui->DPCPoint_tableview->setModel(DPCPoint_model);
	//QTableViewƽ�������п�
	ui->DPCPoint_tableview->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//Ĭ����ʾ��ͷ���������ò����۵Ļ������ǿ��Խ�����        
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
		std::string note = "�����Ǳ�ע";
		QStandardItem* item10 = new QStandardItem(QString::fromUtf8(note.c_str()));

		

		QList<QStandardItem*> item;
		item << item1 << item2 << item3 << item4 << item5 << item6 << item7 << item8 << item9 << item10;
		DPCPoint_model->appendRow(item);

	}
	CPS_ORM_RsFreeRecordSet(gatherDPCPointSetHandle);

}

void Information::showFeederOverload()
{

}



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


//���ϱ��������һ�죬��һ����ε���Ϣ�������е��ֶθ��������
void Information::firstShowTableViewsFromOldTables(QDateTime signdate,QString signshift)
{
	QDateTime AAA = signdate;
	QString BBB = signshift;
}














