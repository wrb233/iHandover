#ifndef TOOLUTIL_H
#define TOOLUTIL_H

#include "common.h"

extern AType AT_ampRating;
extern AType AT_BreakerType;
extern AType AT_ChangeNum;
extern AType AT_ChannelState;
extern AType AT_ChildrenList;
extern AType AT_CommitSyncEnd;
extern AType AT_ControlResult;
extern AType AT_DeviceProperty;
extern AType AT_DPCPointLink;
extern AType AT_DPSPointLink;
extern AType AT_Energized;
extern AType AT_Factory;
extern AType AT_FaultState;
extern AType AT_FeederLink;
extern AType AT_KeyName;
extern AType AT_Limit;
extern AType AT_LoadState;
extern AType AT_MeasLink;
extern AType AT_MeasurementTypeLink;
extern AType AT_Model;
extern AType AT_MVPointLink;
extern AType AT_Name;
extern AType AT_ObjOrder;
extern AType AT_OnlineStatus;
extern AType AT_OnlineTime;
extern AType AT_ParamMva;
extern AType AT_ParentLink;
extern AType AT_PowerState;
extern AType AT_PSRLink;
extern AType AT_RTEnergized;
extern AType AT_State;
extern AType AT_Value;
extern AType AT_Vstate;
extern AType AT_MemberOfLine;
extern AType AT_UnbalancedState;
extern AType AT_QualifiedState;
extern AType AT_Quality;


extern OType OT_Analog;
extern OType OT_BSCPoint;
extern OType OT_DCSwitch;
extern OType OT_Disconnector;
extern OType OT_Discrete;
extern OType OT_DistributionTransformer;
extern OType OT_DMSCommunicateUnit;
extern OType OT_DPCPoint;
extern OType OT_DPSPoint;
extern OType OT_Feeder;
extern OType OT_Folder;
extern OType OT_IncrementCommit;
extern OType OT_LoadBreakSwitch;
extern OType OT_Machine;
extern OType OT_MVPoint;
extern OType OT_PMSBreaker;
extern OType OT_PMSBusbar;
extern OType OT_PMSDoubleWindingTransformer;
extern OType OT_PMSRoot;
extern OType OT_PMSStation;
extern OType OT_PMSTerminal;
extern OType OT_PMSThreeWindingTransformer;
extern OType OT_SubControlArea;
extern OType OT_TransformerWinding;

extern OType OT_MeasurementType;

class ToolUtil:public QObject
{
	Q_OBJECT
public:
	ToolUtil();
	~ToolUtil();
	
	//获取配置文件ihandover.ini中的配置信息
	static QString getiHandoverConf(QString key);

	//初始化OTYPE和ATYPE
	static bool initOTypeAndAType();

	//连接数据库
	static bool connectDB(int argc, char *argv[], DBPOOLHANDLE &dbPoolHandle);
	
	//重连数据库
	static bool UpdateConnectDB(DBPOOLHANDLE dbPoolHandle);

	//日志输出
	static void myDebug(QString content);

	// 实时库read，加try catch异常统一捕获
	static bool databaseRead(ObId obId, AType aType, Data* data);

	// 实时库read/req，加try catch异常统一捕获
	static bool databaseReadTime(ObId obId, AType aType, Data* data, timespec_t &time);

	//实时库matchOtype，加try catch异常统一捕获
	static OType databaseMatchOType(const char* name);

	//实时库extractOType，加try catch异常统一捕获
	static OType databaseExtractOType(ObId obId);

	//实时库find，加try catch异常统一捕获
	static int databaseFind(OType oType, const Condition* conditions, int numberOfConditions,
		ObId* objects = NULL, int maximumNumberOfObjects = 0);

	// 依据某ObId递归向上查找某OType的ObId
	static ObId findOTypeByObId(ObId obId, OType toFindOType, OType stopOType=OT_PMSRoot);

	static int findOTypeFromChildList(ObId obId, OType toFindOType, ContainerData &obIds);

	// 依据某ObId递归向下查找某OType的ObIds
	static int findOTypeByObId(ObId obId, QList<OType> toFindOTypes, ContainerData &obIds);

	//将timespec格式转换为带有ff6毫秒格式的QDatetime
	static QDateTime convertTimespecToQDatetime(timespec_t ts);

	//将map对象转换为json字符串
	static QString convertQMapToJson(QList<QMap<QString,QString> > list);
	
	

	
};

#endif