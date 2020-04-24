#include "version.h"
#include "ToolUtil.h"
#include "initializtion.h"
#include "ace/Process_Mutex.h"
#include "mainwindow.h"
#include "initConTable.h"
#include "CreateTable.h"
#include "signout.h"
#include "signin.h"
#include "information.h"
#include "confirm.h"
#include <QApplication>
#include <QTextCodec>
#include <QDialog>



DBPOOLHANDLE dbPoolHandle;
Database *database = NULL;

UserMSInterface *g_User=NULL;

Logger iHandoverLog = Logger::getInstance(LOG4CPLUS_TEXT("iHandoverLog"));
InitConTable *initConTable = NULL;
QSettings *configIniRead = NULL;//应用级配置文件信息


//initializtion* test =NULL;

//收集检修信息



int main(int argc, char *argv[])
{



	OptionList optionList;
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


	/*
	//初始化OType和AType
	if (!ToolUtil::initOTypeAndAType())
	{
		ToolUtil::myDebug("Init OType And AType error");
		LOG4CPLUS_ERROR(iHandoverLog, "Init OType And AType error");
		std::exit(0);
	}
	*/
	
	initConTable = new InitConTable(argc, argv, NULL,optionList,RUNTIME_SCOPE);
	initConTable->initType();
	qDebug()<<"initAllTables()";
	initConTable->initAllTables();


	g_User = UserMSInterface::UserMS_Instance();
	if (g_User)
	{
		g_User->initUserMSData(database,RUNTIME_ENVIRONMENT);
	}







    QApplication a(argc, argv);

	QTranslator translator;
	translator.load("iHandover_zh_CN.qm");
	a.installTranslator(&translator);

	

	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();



	/*
	SignOut s1;
	s1.show();

	SignIn s2;
	s2.show();


	Information i1;
	i1.show();
    
	*/
    
	

	


    return a.exec();


	
	
}
