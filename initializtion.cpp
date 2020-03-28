#include "initializtion.h"
#include "ToolUtil.h"
//#include "KpiScript.h"
//#include "paramanage.h"
//#include "SqlFileCommit.h"
#include "ace/Process_Mutex.h"

#include "float.h"

#ifdef _WIN32
#define isnan _isnan
#endif
extern Database *database;
extern DBPOOLHANDLE dbPoolHandle;
extern bool firstFlag;
extern std::vector<std::string> switchchangeVAR;
extern std::vector<std::string> dpcusageVAR;
extern std::vector<std::string> dpscorrectrateVAR;
extern std::vector<std::string> dpcavailablecountVAR;
extern std::vector<std::string> dpscorrectcountVAR;
extern std::vector<std::string> dpsdeservechangecountVAR;
extern std::vector<std::string> dpcselectsuccessrateVAR;
extern std::vector<std::string> dpcsuccessrateVAR;

extern  QList <Cur2Feeder> cur2feederlist;;
//extern CKpiScript *globalchangecount;
//extern CKpiScript *globaldpcusage;
//extern CKpiScript *globaldpscorrectrate;
//extern CKpiScript *globaldpcavail;
//extern CKpiScript *globaldpscorrectcount;
//extern CKpiScript *globaldpsdeserve;
//extern CKpiScript *globaldpcselectsuccessrate;
//extern CKpiScript *globaldpcsuccessrate;

extern bool configflag;
extern bool disasterflag;

extern QTimer *timer;

extern initializtion *test;

#define MULTIPLE 100

initializtion::initializtion()
{

}
initializtion::~initializtion()
{

}



