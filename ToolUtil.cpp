#include "ToolUtil.h"


extern Database *database;//实时数据库指针外部全局变量

extern DBPOOLHANDLE dbPoolHandle;//历史库外部全局变量
extern Logger iHandoverLog;//log4c日志



extern  bool isActiveFault;


ToolUtil::ToolUtil():QObject()
{

}

ToolUtil::~ToolUtil()
{

}



/************************************************************************/
/*      //连接历史库                                                                */
/************************************************************************/
bool ToolUtil::connectDB(int argc, char *argv[], DBPOOLHANDLE &dbPoolHandle)
{
	std::string user,password,dataserver;

	for(int i=0;i<argc;i++)
	{
		if(qstricmp(argv[i],"-u") == 0)
		{
			while (*(argv[i+1])!='\0')
			{
				user += *(argv[i+1]++);
			}
		}

		if(qstricmp(argv[i],"-p") == 0)
		{
			while (*(argv[i+1])!='\0')
			{
				password += *(argv[i+1]++);
			}
		}

		if(qstricmp(argv[i],"-d") == 0)
		{
			while (*(argv[i+1])!='\0')
			{
				dataserver += *(argv[i+1]++);
			}
		}
	}

	if(user == "")
		user = "hds";
	if(password == "")
		password = "hds";
	if(dataserver == "")
		dataserver = "hstserver1+hstserver2";
	QStringList dbList = QString::fromStdString(dataserver).split("+");
	CPS_ORM_Initialize();

	DBPoolConfig cfg;
	cfg.m_bAutoCommit=true;
	cfg.m_nPoolSize=dbList.size();
	cfg.m_strUser=user;
	cfg.m_strPassword=password;	
	
	for(int i=0;i<dbList.size();i++)
	{
		cfg.m_strTNS[i] = dbList.at(i).toStdString();	
	}
	dbPoolHandle = CPS_ORM_NewDBPoolbyConfig(cfg);
	std::cout<<"connect to history database...";
	if(CPS_ORM_Connect(dbPoolHandle))
	{
		CPS_ORM_SetAutoCommit(dbPoolHandle,true);
		CPS_ORM_SetStreamAutoCommit(dbPoolHandle,true);
		return true;
	}
	return false;
}

/************************************************************************/
/*      //校验数据库连接，若失败重新连接历史库                                                                */
/************************************************************************/
bool ToolUtil::UpdateConnectDB(DBPOOLHANDLE dbPoolHandle)
{
	//检查数据库连接状态
	CPS_ORM_UpdatePool(dbPoolHandle);
	if(!CPS_ORM_GetDBPoolState(dbPoolHandle))
	{
		if(!CPS_ORM_Connect(dbPoolHandle))
		{
			CPS_ORM_SetAutoCommit(dbPoolHandle,true);
			CPS_ORM_SetStreamAutoCommit(dbPoolHandle,true);
			return false;
		}

		if(0==CPS_ORM_GetDBPoolState(dbPoolHandle))
		{
			LOG4CPLUS_ERROR(iHandoverLog, "Database dbPoolHandle State Error");
			return false;
		}
	}
	return true;
}



