#include "CreateTable.h"
extern Database *database;
extern DBPOOLHANDLE dbPoolHandle;

bool searchANDCreateTable(QString tablename)
{
	if (CPS_ORM_ExistTable(dbPoolHandle,tablename.toStdString()) == -1)//数据库连接有问题
	{
		qDebug()<<"SOMETHING WRONG IN DB, CHECK THE CONNECT";
		return false;
	}
	 else if (CPS_ORM_ExistTable(dbPoolHandle,tablename.toStdString())==0)//数据库没有这张表
	{
	     bool result = createTable(tablename);
		 return result;
	}else
	{		
        qDebug()<<"THE TABLE "+tablename+" IS CREATED ALREADY!!! THE PROGRAM WILL GOING ON. ";//数据库有这张表了没必要建了
		return true;
	}
}
ObId executeSQL(QString sql)
{
	try
	{
		if(CPS_ORM_DirectExecuteSql(dbPoolHandle,sql.toUtf8().data())>=0)
		{
			qDebug()<<sql;
			//qDebug()<<" insert"<<objectid<< "IN DATABASE !";
			return true;
		}
	}
	catch (Exception& e)
	{
		qDebug()<<"Create Table Failure Exception!";
	}
	return false;
}
bool createTable(QString tablename)
{
	if (tablename.startsWith("H_FA_ACCIDENTINFO"))//创建FA事故信息表
	{
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[9];
		newTableConfig[0].m_strName = "ACCIDENT_INFO";//事故信息，非空
		newTableConfig[0].m_strType = "char(1024)";
		newTableConfig[0].m_bNullOK = 0;
		newTableConfig[0].m_strDefault = "0";
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "WORK_DAY";//交接日，交接的工作日期
		newTableConfig[1].m_strType = "timestamp";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "WORK_INDEX";//交接班序号，交接的班次编号
		newTableConfig[2].m_strType = "short";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "TIME";//录入时间，非空，界面不显示，插入数据库的时间
		newTableConfig[3].m_strType = "timestamp";
		newTableConfig[3].m_bNullOK = 0;
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;

		newTableConfig[4].m_strName = "START_TIME";//发生时间，事故发生时间
		newTableConfig[4].m_strType = "timestamp";
		newTableConfig[4].m_bNullOK = 1;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;

		newTableConfig[5].m_strName = "END_TIME";//结束时间，事故结束时间，预留
		newTableConfig[5].m_strType = "timestamp";
		newTableConfig[5].m_bNullOK = 1;
		newTableConfig[5].m_bKey = 0;
		newTableConfig[5].m_bUnique = 0;
		newTableConfig[5].m_bIndexed = 0;

		newTableConfig[6].m_strName = "REASON";//事故原因，事故发生原因
		newTableConfig[6].m_strType = "char(1024)";
		newTableConfig[6].m_bNullOK = 1;
		newTableConfig[6].m_strDefault = "0";
		newTableConfig[6].m_bKey = 0;
		newTableConfig[6].m_bUnique = 0;
		newTableConfig[6].m_bIndexed = 0;

		newTableConfig[7].m_strName = "ACKER";//确认人，预留
		newTableConfig[7].m_strType = "char(1024)";
		newTableConfig[7].m_bNullOK = 1;
		newTableConfig[7].m_strDefault = "0";
		newTableConfig[7].m_bKey = 0;
		newTableConfig[7].m_bUnique = 0;
		newTableConfig[7].m_bIndexed = 0;

		newTableConfig[8].m_strName = "NOTE";//备注，用户手动输入
		newTableConfig[8].m_strType = "char(1024)";
		newTableConfig[8].m_bNullOK = 1;
		newTableConfig[8].m_strDefault = "0";
		newTableConfig[8].m_bKey = 0;
		newTableConfig[8].m_bUnique = 0;
		newTableConfig[8].m_bIndexed = 0;
		

		for(int i=0;i<9;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
	} 
	else if(tablename.startsWith("H_MAINTENANCE_PLAN"))//创建检修计划表
	{                            
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[11];
		

		newTableConfig[0].m_strName = "STATION";//变电站，用户手动输入
		newTableConfig[0].m_strType = "char(1024)";
		newTableConfig[0].m_bNullOK = 1;
		newTableConfig[0].m_strDefault = "0";
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "FEEDER";//线路名称，用户手动输入
		newTableConfig[1].m_strType = "char(1024)";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_strDefault = "0";
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "WORK_DAY";//交接日，交接的工作日期
		newTableConfig[2].m_strType = "timestamp";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "WORK_INDEX";//交接班序号，交接的班次编号
		newTableConfig[3].m_strType = "short";
		newTableConfig[3].m_bNullOK = 1;
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;

		newTableConfig[4].m_strName = "TIME";//录入时间，非空，界面不显示，插入数据库的时间
		newTableConfig[4].m_strType = "timestamp";
		newTableConfig[4].m_bNullOK = 0;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;

		newTableConfig[5].m_strName = "START_TIME";//停电开始时间，用户手动输入
		newTableConfig[5].m_strType = "timestamp";
		newTableConfig[5].m_bNullOK = 1;
		newTableConfig[5].m_bKey = 0;
		newTableConfig[5].m_bUnique = 0;
		newTableConfig[5].m_bIndexed = 0;

		newTableConfig[6].m_strName = "END_TIME";//停电结束时间，用户手动输入
		newTableConfig[6].m_strType = "timestamp";
		newTableConfig[6].m_bNullOK = 1;
		newTableConfig[6].m_bKey = 0;
		newTableConfig[6].m_bUnique = 0;
		newTableConfig[6].m_bIndexed = 0;

		newTableConfig[7].m_strName = "TYPE";//停电性质，用户手动输入
		newTableConfig[7].m_strType = "char(256)";
		newTableConfig[7].m_bNullOK = 1;
		newTableConfig[7].m_strDefault = "0";
		newTableConfig[7].m_bKey = 0;
		newTableConfig[7].m_bUnique = 0;
		newTableConfig[7].m_bIndexed = 0;

		newTableConfig[8].m_strName = "REASON";//停电原因，用户手动输入
		newTableConfig[8].m_strType = "char(1024)";
		newTableConfig[8].m_bNullOK = 1;
		newTableConfig[8].m_strDefault = "0";
		newTableConfig[8].m_bKey = 0;
		newTableConfig[8].m_bUnique = 0;
		newTableConfig[8].m_bIndexed = 0;

		newTableConfig[9].m_strName = "POWEROFFTIME";//停电时长（小时），用户手动输入
		newTableConfig[9].m_strType = "int";
		newTableConfig[9].m_bNullOK = 1;
		newTableConfig[9].m_bKey = 0;
		newTableConfig[9].m_bUnique = 0;
		newTableConfig[9].m_bIndexed = 0;

		newTableConfig[10].m_strName = "NOTE";//备注，用户手动输入
		newTableConfig[10].m_strType = "char(1024)";
		newTableConfig[10].m_bNullOK = 1;
		newTableConfig[10].m_strDefault = "0";
		newTableConfig[10].m_bKey = 0;
		newTableConfig[10].m_bUnique = 0;
		newTableConfig[10].m_bIndexed = 0;



		for(int i=0;i<11;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
    

    }
	else if(tablename.startsWith("H_DPC_OPTRECORD"))//创建遥控记录表
	{                            
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[12];
		

		newTableConfig[0].m_strName = "AREA";//区域，开关所属区域
		newTableConfig[0].m_strType = "bigint(19)";
		newTableConfig[0].m_bNullOK = 1;
		newTableConfig[0].m_strDefault = "0";
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "STATION";//变电站，开关所属变电站
		newTableConfig[1].m_strType = "bigint(19)";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_strDefault = "0";
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "FEEDER";//线路名称，开关所属线路
		newTableConfig[2].m_strType = "bigint(19)";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_strDefault = "0";
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "SWITCH";//开关名称
		newTableConfig[3].m_strType = "bigint(19)";
		newTableConfig[3].m_bNullOK = 1;
		newTableConfig[3].m_strDefault = "0";
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;

		newTableConfig[4].m_strName = "WORK_DAY";//交接日，交接的工作日期
		newTableConfig[4].m_strType = "timestamp";
		newTableConfig[4].m_bNullOK = 1;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;

		newTableConfig[5].m_strName = "WORK_INDEX";//交接班序号，交接的班次编号
		newTableConfig[5].m_strType = "short";
		newTableConfig[5].m_bNullOK = 1;
		newTableConfig[5].m_bKey = 0;
		newTableConfig[5].m_bUnique = 0;
		newTableConfig[5].m_bIndexed = 0;

		newTableConfig[6].m_strName = "TIME";//录入时间，界面不显示，插入数据库的时间
		newTableConfig[6].m_strType = "timestamp";
		newTableConfig[6].m_bNullOK = 0;
		newTableConfig[6].m_bKey = 0;
		newTableConfig[6].m_bUnique = 0;
		newTableConfig[6].m_bIndexed = 0;

		newTableConfig[7].m_strName = "OPT_CONTENT";//操作内容，遥控操作内容
		newTableConfig[7].m_strType = "char(1024)";
		newTableConfig[7].m_bNullOK = 1;
		newTableConfig[7].m_strDefault = "0";
		newTableConfig[7].m_bKey = 0;
		newTableConfig[7].m_bUnique = 0;
		newTableConfig[7].m_bIndexed = 0;

		newTableConfig[8].m_strName = "OPT_TIME";//操作时间
		newTableConfig[8].m_strType = "timestamp";
		newTableConfig[8].m_bNullOK = 1;
		newTableConfig[8].m_bKey = 0;
		newTableConfig[8].m_bUnique = 0;
		newTableConfig[8].m_bIndexed = 0;

		newTableConfig[9].m_strName = "OPERATORINFO";//操作人
		newTableConfig[9].m_strType = "char(1024)";
		newTableConfig[9].m_bNullOK = 1;
		newTableConfig[9].m_strDefault = "0";
		newTableConfig[9].m_bKey = 0;
		newTableConfig[9].m_bUnique = 0;
		newTableConfig[9].m_bIndexed = 0;

		newTableConfig[10].m_strName = "GUARDERINFO";//监护人
		newTableConfig[10].m_strType = "char(1024)";
		newTableConfig[10].m_bNullOK = 1;
		newTableConfig[10].m_strDefault = "0";
		newTableConfig[10].m_bKey = 0;
		newTableConfig[10].m_bUnique = 0;
		newTableConfig[10].m_bIndexed = 0;

		newTableConfig[11].m_strName = "NOTE";//备注，用户手动输入
		newTableConfig[11].m_strType = "char(1024)";
		newTableConfig[11].m_bNullOK = 1;
		newTableConfig[11].m_strDefault = "0";
		newTableConfig[11].m_bKey = 0;
		newTableConfig[11].m_bUnique = 0;
		newTableConfig[11].m_bIndexed = 0;



		for(int i=0;i<12;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
    

    }
	else if(tablename.startsWith("H_FEEDER_OVERLOAD"))//创建线路负载表
	{                            
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[11];
		

		newTableConfig[0].m_strName = "AREA";//区域，线路所属区域
		newTableConfig[0].m_strType = "bigint(19)";
		newTableConfig[0].m_bNullOK = 1;
		newTableConfig[0].m_strDefault = "0";
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "STATION";//变电站，线路所属变电站
		newTableConfig[1].m_strType = "bigint(19)";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_strDefault = "0";
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "FEEDER";//线路名称
		newTableConfig[2].m_strType = "bigint(19)";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_strDefault = "0";
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "WORK_DAY";//交接日，交接的工作日期
		newTableConfig[3].m_strType = "timestamp";
		newTableConfig[3].m_bNullOK = 1;
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;

		newTableConfig[4].m_strName = "WORK_INDEX";//交接班序号，交接的班次编号
		newTableConfig[4].m_strType = "short";
		newTableConfig[4].m_bNullOK = 1;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;

		newTableConfig[5].m_strName = "TIME";//录入时间，非空，界面不显示，插入数据库的时间
		newTableConfig[5].m_strType = "timestamp";
		newTableConfig[5].m_bNullOK = 0;
		newTableConfig[5].m_bKey = 0;
		newTableConfig[5].m_bUnique = 0;
		newTableConfig[5].m_bIndexed = 0;

		newTableConfig[6].m_strName = "AMPRATING";//额定电流（A）
		newTableConfig[6].m_strType = "double";
		newTableConfig[6].m_bNullOK = 1;
		newTableConfig[6].m_bKey = 0;
		newTableConfig[6].m_bUnique = 0;
		newTableConfig[6].m_bIndexed = 0;

		newTableConfig[7].m_strName = "MAX_CURRENT";//最大电流，线路当班出现的最大电流
		newTableConfig[7].m_strType = "double";
		newTableConfig[7].m_bNullOK = 1;
		newTableConfig[7].m_bKey = 0;
		newTableConfig[7].m_bUnique = 0;
		newTableConfig[7].m_bIndexed = 0;

		newTableConfig[8].m_strName = "OCCUR_TIME";//出现时间，最大电流出现时间
		newTableConfig[8].m_strType = "timestamp";
		newTableConfig[8].m_bNullOK = 1;
		newTableConfig[8].m_bKey = 0;
		newTableConfig[8].m_bUnique = 0;
		newTableConfig[8].m_bIndexed = 0;

		newTableConfig[9].m_strName = "OVERLOADRATE";//负载率，最大电流与额定电流比值
		newTableConfig[9].m_strType = "double";
		newTableConfig[9].m_bNullOK = 1;
		newTableConfig[9].m_bKey = 0;
		newTableConfig[9].m_bUnique = 0;
		newTableConfig[9].m_bIndexed = 0;

		newTableConfig[10].m_strName = "NOTE";//备注，用户手动输入
		newTableConfig[10].m_strType = "char(1024)";
		newTableConfig[10].m_bNullOK = 1;
		newTableConfig[10].m_strDefault = "0";
		newTableConfig[10].m_bKey = 0;
		newTableConfig[10].m_bUnique = 0;
		newTableConfig[10].m_bIndexed = 0;



		for(int i=0;i<11;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
    

    }

	else if(tablename.startsWith("H_SIGNINANDOUT"))//创建交接班交接记录总表
	{                            
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[7];
		

		newTableConfig[0].m_strName = "TIME";//交接确认时间，非空
		newTableConfig[0].m_strType = "timestamp";
		newTableConfig[0].m_bNullOK = 0;
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "HANDOVER";//交班人
		newTableConfig[1].m_strType = "bigint(19)";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "SUCCESSOR";//接班人
		newTableConfig[2].m_strType = "bigint(19)";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "WORK_DAY";//交接日，交接的工作日期
		newTableConfig[3].m_strType = "timestamp";
		newTableConfig[3].m_bNullOK = 1;
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;

		newTableConfig[4].m_strName = "WORK_INDEX";//交接班序号，交接的班次编号
		newTableConfig[4].m_strType = "short";
		newTableConfig[4].m_bNullOK = 1;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;
		
		newTableConfig[5].m_strName = "NOTE";//交接备注内容，用户手动输入
		newTableConfig[5].m_strType = "char(1024)";
		newTableConfig[5].m_bNullOK = 1;
		newTableConfig[5].m_strDefault = "0";
		newTableConfig[5].m_bKey = 0;
		newTableConfig[5].m_bUnique = 0;
		newTableConfig[5].m_bIndexed = 0;

		newTableConfig[6].m_strName = "WORK_PLACE";//上班地点，工作地点
		newTableConfig[6].m_strType = "bigint(19)";
		newTableConfig[6].m_bNullOK = 1;
		newTableConfig[6].m_bKey = 0;
		newTableConfig[6].m_bUnique = 0;
		newTableConfig[6].m_bIndexed = 0;

		for(int i=0;i<7;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
    

    }

	else if(tablename.startsWith("H_WORK_HOURS"))//创建各地班工作时段配置表
	{                            
		ORMColumnDescVector descVector;
		ORM_ColumnDesc_St newTableConfig[5];
		

		
		newTableConfig[0].m_strName = "WORK_INDEX";//班序号，交接的班次编号，对应早/中/晚班
		newTableConfig[0].m_strType = "short";
		newTableConfig[0].m_bNullOK = 1;
		newTableConfig[0].m_bKey = 0;
		newTableConfig[0].m_bUnique = 0;
		newTableConfig[0].m_bIndexed = 0;

		newTableConfig[1].m_strName = "WORK_NAME";//班名称，对应班名称
		newTableConfig[1].m_strType = "char(256)";
		newTableConfig[1].m_bNullOK = 1;
		newTableConfig[1].m_bKey = 0;
		newTableConfig[1].m_bUnique = 0;
		newTableConfig[1].m_bIndexed = 0;

		newTableConfig[2].m_strName = "WOR_START";//班开始时段，某班的工作开始时间
		newTableConfig[2].m_strType = "int";
		newTableConfig[2].m_bNullOK = 1;
		newTableConfig[2].m_bKey = 0;
		newTableConfig[2].m_bUnique = 0;
		newTableConfig[2].m_bIndexed = 0;

		newTableConfig[3].m_strName = "WOR_END";//班结束时段，某班的工作结束时间
		newTableConfig[3].m_strType = "int";
		newTableConfig[3].m_bNullOK = 1;
		newTableConfig[3].m_bKey = 0;
		newTableConfig[3].m_bUnique = 0;
		newTableConfig[3].m_bIndexed = 0;
		
		newTableConfig[4].m_strName = "WORK_PLACE";//上班地点，工作地点
		newTableConfig[4].m_strType = "bigint(19)";
		newTableConfig[4].m_bNullOK = 1;
		newTableConfig[4].m_bKey = 0;
		newTableConfig[4].m_bUnique = 0;
		newTableConfig[4].m_bIndexed = 0;

		for(int i=0;i<5;i++)
		{
			descVector.push_back(newTableConfig[i]);
		}

		int result = CPS_ORM_CreateTable(dbPoolHandle,tablename.toStdString(),descVector,2);

		if(result<0)
		{
			qDebug()<<"CREATE TABLE "<<tablename<<" FAILED!!!";
			return false;
		}else{
			qDebug()<<"CREATE TABLE "<<tablename<<" SUCCESSFULLY!!!";
		}
    

    }
	return true;
}
bool searchTable(QString tablename)
{
	return CPS_ORM_ExistTable(dbPoolHandle,tablename.toStdString());
}