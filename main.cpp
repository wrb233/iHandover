#include "version.h"
#include "ToolUtil.h"
#include "initializtion.h"
#include "ace/Process_Mutex.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

DBPOOLHANDLE dbPoolHandle;
Database *database = NULL;
Logger iHandoverLog = Logger::getInstance(LOG4CPLUS_TEXT("iHandoverLog"));

//initializtion* test =NULL;

//收集检修信息



//收集遥信信息
/*
void gatherDPCPoint()
{
	RECORDSETHANDLE gatherDPCPointSetHandle = CPS_ORM_RsNewRecordSet();
	
	
	QString sqlDPCPoint = "select SERVICEID, OBID, DPCACTION, ACTIONTIME, ACTIONRESULT, STATION, AREA, OPERATORINFO, GUARDERINFO from H_DPCPOINT_RECORD_2020";
	ORMTimeStamp actiontime;
		int rowsOfDPCPoint = CPS_ORM_RsLoadData(gatherDPCPointSetHandle,sqlDPCPoint.toUtf8().data(),dbPoolHandle);
		for (int i=0;i<rowsOfDPCPoint;i++)
		{
			ObId serviceid = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,0);
			ObId obid = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,1);
			int dpcaction = (int)CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,2);
			actiontime = CPS_ORM_RsGetTimeValue(gatherDPCPointSetHandle,i,3);
            int actionresult = (int)CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,4);
			ObId station = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,5);
			ObId area = CPS_ORM_RsGetNumberValue(gatherDPCPointSetHandle,i,6);
			QString operatorinfo = QString::fromStdString(CPS_ORM_RsGetStringValue(gatherDPCPointSetHandle, i,7));
			QString guarderinfo = QString::fromStdString(CPS_ORM_RsGetStringValue(gatherDPCPointSetHandle, i,8));

		}
		CPS_ORM_RsFreeRecordSet(gatherDPCPointSetHandle);

}
*/



int main(int argc, char *argv[])
{
	if (echoVersion(argc, argv)) 
	{
		return 0;
	}



	if (!ToolUtil::connectDB(argc, argv, dbPoolHandle))
	{
		std::cout<<"connect hdsdatabase failed"<<std::endl;
	}else
	{
         qDebug()<<"hdsdatabase connected ";
	} 
	

	//test = new initializtion();

	//test->gatherDPCPoint();
	

    QApplication a(argc, argv);

	

	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();
    

    




    return a.exec();
	
}
