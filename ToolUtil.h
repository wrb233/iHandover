#ifndef TOOLUTIL_H
#define TOOLUTIL_H

#include "common.h"


class ToolUtil:public QObject
{
	Q_OBJECT
public:
	ToolUtil();
	~ToolUtil();
	
	//��ȡ�����ļ�ihandover.ini�е�������Ϣ
	static QString getHisRecordConf(QString key);

	//�������ݿ�
	static bool connectDB(int argc, char *argv[], DBPOOLHANDLE &dbPoolHandle);
	
	//�������ݿ�
	static bool UpdateConnectDB(DBPOOLHANDLE dbPoolHandle);
	
	

	
};

#endif