#include "version.h"
#include "ToolUtil.h"
#include "initializtion.h"
#include "ace/Process_Mutex.h"
#include "mainwindow.h"
#include "initConTable.h"
#include "CreateTable.h"
#include "signout.h"
#include "signin.h"
#include "signininformation.h"
#include "signoutinformation.h"
#include "confirm.h"
#include <QApplication>
#include <QTextCodec>
#include <QDialog>



DBPOOLHANDLE dbPoolHandle;
Database *database = NULL;

UserMSInterface *g_User=NULL;
QList<QString> MVPointlist;

//Information* information = NULL;

Logger iHandoverLog = Logger::getInstance(LOG4CPLUS_TEXT("iHandoverLog"));
InitConTable *initConTable = NULL;
QSettings *configIniRead = NULL;//应用级配置文件信息


initializtion* test =NULL;

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
	

	
	//information = new Information(argc, argv, NULL,optionList,RUNTIME_SCOPE);
	
	//初始化OType和AType
	
	
	
	
	
	initConTable = new InitConTable(argc, argv, NULL,optionList,RUNTIME_SCOPE);
	//initConTable->initType();
	qDebug()<<"initAllTables()";
	initConTable->initAllTables();


	g_User = UserMSInterface::UserMS_Instance();
	if (g_User)
	{
		g_User->initUserMSData(database,RUNTIME_ENVIRONMENT);
	}


	
	if (!ToolUtil::initOTypeAndAType())
	{
		ToolUtil::myDebug("Init OType And AType error");
		LOG4CPLUS_ERROR(iHandoverLog, "Init OType And AType error");
		std::exit(0);
	}
	
	test = new initializtion();
	test->initLineload();





   // QApplication a(argc, argv);

	/*
	char* cpsenv = getenv("CPS_ENV");
	QString transPath = QDir::fromNativeSeparators(QString::fromUtf8(cpsenv)) + "/resource/"; 
	QTranslator translator;
	translator.load(transPath+"iHandover_zh_CN.qm");
	*/
	QTranslator translator;
	translator.load("iHandover_zh_CN.qm");

	

	initConTable->installTranslator(&translator);

	

	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
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
    
	

	
	initConTable->svc();
    return 1;


	
	
}




/*
void handleMVPoint()
{
	OT_MVPoint = database->matchOType("MVPoint");
	Condition* conditions = NULL;
	int numberOfConditions = 0;
	int numElements = 0;
	numElements = database->find(OT_MVPoint,conditions,numberOfConditions);
	ObId* objects = new ObId[numElements];
	database->find(OT_MVPoint, conditions, numberOfConditions, objects, numElements);

	QList<QMap<QString,QString> >list;

	for( int i=0;i<numElements;i++)
	{


		//找到量测类型关联
		ObId measurementtypelink = initConTable->getMeasurementTypeLink(objects[i]);


		QString MVPoint_id = QString::number(objects[i]); 

		StringData data;
		ToolUtil::databaseRead(objects[i], AT_Name, &data);
		OMString str = (OMString)data;
		QString name = QString::fromUtf8(str.c_str());

		ObId DeviceProperty_id = DeviceProperty(objects[i]); 
		QString DevicePropertyString_id = QString::number(DeviceProperty_id);

		ObId Feeder_id = getFeederLink(objects[i]); 
		QString FeederString_id = QString::number(Feeder_id);

		QMap<QString,QString> map;
		map.insert("id",DMSCommunicateUnit_id);
		map.insert("terminal_name",name);
		map.insert("terminal_type",DevicePropertyString_id);
		map.insert("source_id",FeederString_id);

		list.append(map);		
	}
	QString json = ToolUtil::convertQMapToJson(list);
	ToolUtil::writeJsonFileByInfo(json,"dmsterminal");
}
*/