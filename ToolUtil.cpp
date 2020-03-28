#include "ToolUtil.h"


extern Database *database;//ʵʱ���ݿ�ָ���ⲿȫ�ֱ���

extern DBPOOLHANDLE dbPoolHandle;//��ʷ���ⲿȫ�ֱ���
extern Logger iHandoverLog;//log4c��־



extern  bool isActiveFault;


ToolUtil::ToolUtil():QObject()
{

}

ToolUtil::~ToolUtil()
{

}



/************************************************************************/
/*      //������ʷ��                                                                */
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
/*      //У�����ݿ����ӣ���ʧ������������ʷ��                                                                */
/************************************************************************/
bool ToolUtil::UpdateConnectDB(DBPOOLHANDLE dbPoolHandle)
{
	//������ݿ�����״̬
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



