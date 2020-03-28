#ifndef TOOLUTIL_H
#define TOOLUTIL_H

#include "common.h"


class ToolUtil:public QObject
{
	Q_OBJECT
public:
	ToolUtil();
	~ToolUtil();
	
	//获取配置文件ihandover.ini中的配置信息
	static QString getHisRecordConf(QString key);

	//连接数据库
	static bool connectDB(int argc, char *argv[], DBPOOLHANDLE &dbPoolHandle);
	
	//重连数据库
	static bool UpdateConnectDB(DBPOOLHANDLE dbPoolHandle);
	
	

	
};

#endif