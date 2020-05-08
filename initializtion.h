#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#define COUNTDOWN			1000
#define SERVICEID			0
#define OBID				1

#define PMSBREAKER          0
#define PMSSTATION          1
#define SUBCONTROLAREA      2
#define TERMINAL            3
//const int 

#include "common.h"
#include "QHash"
#include "QList"
//#define 

typedef struct timeOfrecovery//something about time change
{
	int difference;
	int days;
	int surplushour; 
	int months;
	timeOfrecovery()
	{
	   difference = 0;
	   days = 0;
	   surplushour = 0;
	   months = 0;
	}
}TimeOfRecovery;

typedef struct dpcAnddps//it is used to compute the dpc and dps
{
	float dpcusage;
	float dpscorrectrate;
	float dpcselectsuccessrate;//dpc
	float dpcsuccessrate;//dpc

	int switchchangecount;
	int dpcsuccessinfact;
	int dpcavailablecount;
	int dpscorrectcount;
	int dpsdeservechangecount;

	int dpcselecttotalcount;//dpc
	int dpctotalcount;
	int dpcselectsuccesscount;
	int dpcsuccesscount;
	dpcAnddps()
	{
		dpcusage = 0.0f;
		dpscorrectrate = 0.0f;
		dpcselectsuccessrate = 0.0f;//dpc
		dpcsuccessrate = 0.0f;

		switchchangecount = 0;
		dpcsuccessinfact = 0;
		dpcavailablecount = 0;
		dpscorrectcount = 0;
		dpsdeservechangecount = 0;

		dpcselecttotalcount = 0;//dpc
		dpctotalcount = 0;
		dpcselectsuccesscount = 0;
		dpcsuccesscount = 0;
	}

}DPCANDPS;
typedef struct basepmskpi//the base kpi of pmsbreaker
{
	//obid serviceid;
	ObId pmsbrekerid;
	int switchchangecount;
	int dpcsuccessinfact;
	int dpcfailcount;
	int switchonsiteoptcount;
	int	accidenttripcount;
	int switchreclosingcount;
	int switchartificialcount;
	int switchrefuseactioncount;
	int switchmaloperationcount;

	int dpcselecttotalcount;
	int dpctotalcount;
	int dpcselectsuccesscount;
	int dpcsuccesscount;

	int dpcavailablecount;
	int dpscorrectcount;
	int dpsdeservechangecount;
	float dpcusage;
	float dpscorrectrate;
	ObId uplevel;
	ObId toplevel;

	float dpcselectsuccessrate;
	float dpcsuccessrate;
	//int switchclosedtime;//minutes ,second or hour
	//int switchopentime;
	basepmskpi()
	{
		pmsbrekerid = 0;
		switchchangecount = 0;
		dpcsuccessinfact = 0;
		dpcfailcount = 0;
		switchonsiteoptcount = 0;
		accidenttripcount = 0;
		switchreclosingcount = 0;
		switchartificialcount = 0;
		switchrefuseactioncount = 0;
		switchmaloperationcount = 0;
		
		dpcselecttotalcount = 0;
		dpctotalcount = 0;
		dpcselectsuccesscount = 0;
		dpcsuccesscount = 0;

		dpcselectsuccessrate = 0.0f;
		dpcsuccessrate = 0.0f;

		switchchangecount = 0;
		dpcavailablecount = 0;
	    dpscorrectcount = 0;
		dpsdeservechangecount =0;
		dpcusage = 0.0f;
		dpscorrectrate = 0.0f;
		uplevel = 0;
		toplevel = 0;
		//switchclosedtime = 0;
		//switchopentime = 0;
	}
	QHash <QString,float> createhash()
	{
		QHash <QString,float> VPmap;
		std::string temp = "switchchangecount";
		VPmap.insert("switchchangecount",switchchangecount);
		VPmap.insert("dpcsuccessinfact",dpcsuccessinfact);
		VPmap.insert("dpcfailcount",dpcfailcount);
		VPmap.insert("switchonsiteoptcount",switchonsiteoptcount);
		VPmap.insert("accidenttripcount",accidenttripcount);
		VPmap.insert("switchreclosingcount",switchreclosingcount);
		VPmap.insert("switchartificialcount",switchartificialcount);
		VPmap.insert("switchrefuseactioncount",switchrefuseactioncount);
		VPmap.insert("switchmaloperationcount",switchmaloperationcount);

		VPmap.insert("dpcselecttotalcount",dpcselecttotalcount);
		VPmap.insert("dpctotalcount",dpctotalcount);
		VPmap.insert("dpcselectsuccesscount",dpcselectsuccesscount);
		VPmap.insert("dpcsuccesscount",dpcsuccesscount);
		//VPmap.insert("switchclosedtime",switchclosedtime);
		//VPmap.insert("switchopentime",switchopentime);
		VPmap.insert("dpcselectsuccessrate",dpcselectsuccessrate);
		VPmap.insert("dpcsuccessrate",dpcsuccessrate);



		return VPmap;
	}
}basepmsKPI;
typedef struct cur2feeder
{
	ObId currentMVPoint;
	//ObId outletbreak;
	float amprating;
	ObId feeder;
	ObId station;
	ObId countyarea;
	cur2feeder()
	{
		currentMVPoint = 0;
		//outletbreak = 0;
		amprating = 0.0f;
		feeder = 0;
		station = 0;
		countyarea = 0;
	}
}Cur2Feeder;


class initializtion:public QObject
{
	Q_OBJECT
public:
	initializtion();
	~initializtion();


	void initLineload();
	ObId getMeasurementTypeLink(ObId dmsObjId);
	int BreakerType(ObId dmsObjId);

	
	 

};

//QList <QString> MVPointlist; 
//void updateKPIDB();
//void queryData(QString sqlcheckpoint, QString sqlday, bool daychange);
//void gatherDPCPoint();

#endif