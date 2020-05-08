#include "oms/qt_application.h"
class InitConTable: public QtApplication
{
public:

	
	InitConTable(int &argc, char **argv,
		SignalHandler sigtermHandler,
		const OptionList& optionList,
		EnvironmentScope environmentScope);

	
	~InitConTable();

	bool initAllTables();
	bool initAccidentInfoTable();
	bool initMaintenancePlanTable();
	bool initDPCPointTable();
	bool initLineLoadTable();
	bool initSignInAndOutTable();
    bool initWorkHoursTable();
	
	void initType();
	ObId findOTypeByObId(ObId obId, OType toFindOType, OType stopOType);


	ObId getDPSPointLink(ObId dmsObjId);
	ObId getPSRLink(ObId dmsObjId);

	ObId getMemberOfLineLink(ObId dmsObjId);
	
	



	static bool executeSQL(QString sql, ObId temp);

	
	

	OType OT_PMSBreaker;
	OType OT_PMSRoot;
	OType OT_PMSStation;
	OType OT_Root;
	OType OT_SubControlArea;
	OType OT_Partition;
	OType OT_DMSCommunicateUnit;
	OType OT_Folder;
	
	AType AT_ParentLink;
	AType AT_TerminalLink;
	AType AT_DeviceProperty;
	AType AT_FeederLink;
	AType AT_Name;
	AType AT_Description;
	AType AT_DPSPointLink;
	AType AT_PSRLink;
};