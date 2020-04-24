#include "initConTable.h"
#include "oms/database_application.h"
#include "ToolUtil.h"
#include "CreateTable.h"
extern Database * database;
extern DBPOOLHANDLE dbPoolHandle;

//init the device config table 
InitConTable::InitConTable(int &argc, char **argv,SignalHandler sigtermHandler,const OptionList& optionList,
	EnvironmentScope environmentScope) : QtApplication(argc, argv, sigtermHandler, optionList, environmentScope)
{
	database = getDatabase();
	//_database = createDatabase();
}
InitConTable::~InitConTable()
{
	
}
//函数声明,6张表
bool initAccidentInfoTable();
bool initMaintenancePlanTable();
bool initDPCPointTable();
bool initLineLoadTable();
bool initSignInAndOutTable();
bool initWorkHoursTable();

bool InitConTable::initAllTables()
{

	initAccidentInfoTable();//初始化事故信息表
	initMaintenancePlanTable();//初始化检修计划表
	initDPCPointTable();//初始化遥控记录表
	initLineLoadTable();//初始化线路重载表

	initSignInAndOutTable();//初始化交接班交接记录总表
	initWorkHoursTable();//初始化各地班工作时段配置表
	return true;
}




bool InitConTable::initAccidentInfoTable()
{
	if (!searchANDCreateTable("H_FA_ACCIDENTINFO"))//事故信息表的表名
	{
		qDebug()<<"searchANDCreateTable() H_FA_ACCIDENTINFO ERROR";
		return false;
	}
	//QString H_FA_ACCIDENTINFO = "delete from H_FA_ACCIDENTINFO";
	//executeSQL(H_FA_ACCIDENTINFO,0);//清空表
	return true;
}
bool InitConTable::initMaintenancePlanTable()
{
	if (!searchANDCreateTable("H_MAINTENANCE_PLAN"))//检修计划表的表名
	{
		qDebug()<<"searchANDCreateTable() H_MAINTENANCE_PLAN ERROR";
		return false;
	}
	//QString H_MAINTENANCE_PLAN_tableview = "delete from H_MAINTENANCE_PLAN";
	//executeSQL(H_MAINTENANCE_PLAN_tableview,0);//清空表	
	return true;
}
bool InitConTable::initDPCPointTable()
{
	if (!searchANDCreateTable("H_DPC_OPTRECORD"))//遥控记录表的表名
	{
		qDebug()<<"searchANDCreateTable() H_DPC_OPTRECORD ERROR";
		return false;
	}
	//QString H_DPC_OPTRECORD_tableview = "delete from H_DPC_OPTRECORD";
	//executeSQL(H_DPC_OPTRECORD_tableview,0);//清空表	
	return true;
}
bool InitConTable::initLineLoadTable()
{
	if (!searchANDCreateTable("H_FEEDER_OVERLOAD"))//线路重载表的表名
	{
		qDebug()<<"searchANDCreateTable() H_FEEDER_OVERLOAD ERROR";
		return false;
	}
	//QString H_FEEDER_OVERLOAD_tableview = "delete from H_FEEDER_OVERLOAD";
	//executeSQL(H_FEEDER_OVERLOAD_tableview,0);//清空表	
	return true;
}
bool InitConTable::initSignInAndOutTable()
{
	if (!searchANDCreateTable("H_SIGNINANDOUT"))//线路重载表的表名
	{
		qDebug()<<"searchANDCreateTable() H_SIGNINANDOUT ERROR";
		return false;
	}
	//QString H_SIGNINANDOUT_tableview = "delete from H_SIGNINANDOUT";
	//executeSQL(H_SIGNINANDOUT_tableview,0);//清空表	
	return true;
}
bool InitConTable::initWorkHoursTable()
{
	if (!searchANDCreateTable("H_WORK_HOURS"))//线路重载表的表名
	{
		qDebug()<<"searchANDCreateTable() H_WORK_HOURS ERROR";
		return false;
	}
	//QString H_WORK_HOURS_tableview = "delete from H_WORK_HOURS";
	//executeSQL(H_WORK_HOURS_tableview,0);//清空表	
	return true;
}


ObId InitConTable::findOTypeByObId(ObId obId, OType toFindOType, OType stopOType)
{
	ObId toFindObId = 0;
	LinkData parentLinkData = 0;
	try
	{	
		database->read(obId, AT_ParentLink, &parentLinkData);
	}
	catch (Exception  &e)
	{
		return 0;
	}
	if (parentLinkData!=0)
	{
		OType oType = -1; 
		while(stopOType != oType)
		{
			oType = database->extractOType((ObId)parentLinkData);
			if (oType==toFindOType)
			{
				//toFindObId = (ObId)parentLinkData;
				return parentLinkData;
			}
			try
			{	
				database->read(parentLinkData, AT_ParentLink, &parentLinkData);
			}
			catch (Exception  &e)
			{
				return 0;
			}
		}	
	}
	return 0;
}
void InitConTable::initType()
{
	OT_PMSBreaker = database->matchOType("PMSBreaker");//69
	OT_PMSStation = database->matchOType("PMSStation");//59
	OT_PMSRoot = database->matchOType("PMSRoot");//56
	OT_Root = database->matchOType("Root");//10
	OT_SubControlArea =database->matchOType("SubControlArea");//264
	OT_Partition = database->matchOType("Partition");//327
	OT_DMSCommunicateUnit = database->matchOType("DMSCommunicateUnit");//399
	OT_Folder = database->matchOType("Folder");//11

	AT_ParentLink= database->matchAType("ParentLink");//9
	AT_TerminalLink = database->matchAType("TerminalLink");//2476
	AT_DeviceProperty = database->matchAType("DeviceProperty");//2545
	AT_FeederLink = database->matchAType("FeederLink");//2583
	AT_Name = database->matchAType("Name");//1
	AT_Description = database->matchAType("Description");//25
	AT_DPSPointLink = database->matchAType("DPSPointLink");//149
	AT_PSRLink = database->matchAType("PSRLink");//1813
}
bool InitConTable::executeSQL(QString sql, ObId temp)
{
	try
	{
		if(CPS_ORM_DirectExecuteSql(dbPoolHandle,sql.toUtf8().data())>=0)
		{
			//qDebug()<<sql;
			//qDebug()<<" insert"<<objectid<< "IN DATABASE !";
			return true;
		}
	}
	catch (Exception& e)
	{
		qDebug()<<"init BREAKER FAILURE Exception!";
	}
	return false;
}

