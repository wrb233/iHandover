#include "frmdataout.h"
//#include "ui_frmdataout.h"
//#include "head.h"

frmDataOut::frmDataOut(QWidget *parent) : QWidget(parent)
{
    //ui->setupUi(this);
    this->initForm();
}

frmDataOut::~frmDataOut()
{
    //delete ui;
}

void frmDataOut::initForm()
{
    //ui->tabWidget->setCurrentIndex(0);
}

int frmDataOut::getCount()
{

	int a =7;

	return a;
}

void frmDataOut::initTable(QTableView *table)
{
    
}

void frmDataOut::initTable1(QTableWidget *table)
{
   
}

void frmDataOut::loadTable1(QTableWidget *table, int maxCount)
{
    
}

void frmDataOut::loadTable2(QTableView *table)
{
    
}

void frmDataOut::loadTable3(QTableView *table)
{
    
}

void frmDataOut::initColumnFAaccidentInfo(QList<QString> &columnNames, QList<int> &columnWidths)
{
    columnNames.clear();
    columnNames << QObject::tr("accident_info") << QObject::tr("work_day") << QObject::tr("work_index") << QObject::tr("start_time") << QObject::tr("reason") << QObject::tr("note");

    columnWidths.clear();
    columnWidths << 180 << 180 << 180 << 180 << 280 << 180 ;
}

void frmDataOut::initColumnMaintenancePlan(QList<QString> &columnNames, QList<int> &columnWidths)
{
    columnNames.clear();
    columnNames << QObject::tr("station") << QObject::tr("feeder") << QObject::tr("work_day") << QObject::tr("work_index") << QObject::tr("start_time") << QObject::tr("end_time") << QObject::tr("type")<< QObject::tr("reason")<<QObject::tr("powerofftime")<<QObject::tr("note");

    columnWidths.clear();
    columnWidths << 70 << 100 << 70 << 70 << 70 << 140 << 150<< 70 << 70 << 70;
}

void frmDataOut::initColumnDPCOptRecord(QList<QString> &columnNames, QList<int> &columnWidths)
{
    columnNames.clear();
    columnNames << QObject::tr("area") << QObject::tr("station") << QObject::tr("feeder") << QObject::tr("switch")<<QObject::tr("work_day") << QObject::tr("work_index")<< QObject::tr("opt_content")<< QObject::tr("opt_time")<< QObject::tr("operatorinfo")<< QObject::tr("guarderinfo")<< QObject::tr("note");

    columnWidths.clear();
    columnWidths << 70 << 100 << 70 << 70 << 70 << 140 << 150<< 70 << 70 << 70<<70;
}

void frmDataOut::initColumnFeederOverload(QList<QString> &columnNames, QList<int> &columnWidths)
{
    columnNames.clear();
    columnNames << QObject::tr("area") << QObject::tr("station") << QObject::tr("feeder") <<QObject::tr("work_day") << QObject::tr("work_index")<< QObject::tr("amprating")<< QObject::tr("max_current")<< QObject::tr("occur_time")<< QObject::tr("overloadrate")<< QObject::tr("note");

    columnWidths.clear();
    columnWidths << 70 << 100 << 70 << 70 << 70 << 140 << 150<< 70 << 70 << 70;
}
