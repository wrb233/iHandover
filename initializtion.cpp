#include "initializtion.h"
#include "ToolUtil.h"
//#include "KpiScript.h"
//#include "paramanage.h"
//#include "SqlFileCommit.h"
#include "ace/Process_Mutex.h"

#include "float.h"

#include <QList>

#ifdef _WIN32
#define isnan _isnan
#endif
extern Database *database;
extern DBPOOLHANDLE dbPoolHandle;

extern QList<QString> MVPointlist;
extern initializtion *test;

initializtion::initializtion()
{

}
initializtion::~initializtion()
{

}



void initializtion::initLineload()
{
	
	//找A相电流 
	ObId Ia = 0;
	StringData keynameIa = "I_A";
	Condition searchIa[1];
	searchIa[0].set(AT_KeyName,EQ,&keynameIa);
	database->find(OT_MeasurementType,searchIa,1,&Ia,1);
	qDebug()<<QString::number(Ia);//1163936137269

    //这次通过PMSBreaker来找
	ObId *obIds1;
	Condition outletbreaker[1];
	IntegerData breakType = 6;
	outletbreaker[0].set(AT_BreakerType,EQ,&breakType);
	int numpmsbreaker = database->findNumberOfObjects(OT_PMSBreaker,outletbreaker,1);//278个PMSBreaker
	if (numpmsbreaker <= 0) 
	{
		qDebug()<<"no pmsbreaker in modle";
		return ;
	}
	obIds1 = new ObId[numpmsbreaker];
	database->find(OT_PMSBreaker, outletbreaker, 1, obIds1, numpmsbreaker);
	
	for (int i = 0;i<numpmsbreaker;i++)//遍历278个PMSBreaker

	{
		ObId pmsbreaker = obIds1[i];




		ContainerData breakChildren;
		database->read(pmsbreaker,AT_ChildrenList,&breakChildren);
		int numChildren = breakChildren.getNumberOfElements();
		const ObId *children = breakChildren.getObIds();

		for (int i=0;i<numChildren;i++)//每一个PMSBreaker有几个孩子包含PMSTerminal
		{
			ContainerData tempchildren;
			database->read(children[i],AT_ChildrenList,&tempchildren);
			//qDebug()<<"ssssssssss";
			//qDebug()<<children[i];
			if ((database->extractOType(children[i]) ==OT_PMSTerminal)&&(tempchildren.getNumberOfElements()!=0))
			{


				//qDebug()<<"这里是我想要的哈哈哈ssssssssss";

				//qDebug()<<children[i];//OT_PMSTerminal找到了




				ContainerData analogChildren;
				database->read(children[i],AT_ChildrenList,&analogChildren);
				int numChildren2 = analogChildren.getNumberOfElements();
				const ObId *children2 = analogChildren.getObIds();

				for (int i=0;i<numChildren2;i++)//每一个PMSTerminal有几个孩子
				{
					ContainerData tempchildren;
					database->read(children2[i],AT_ChildrenList,&tempchildren);
					//qDebug()<<"ssssssssss";
					//qDebug()<<children2[i];
					if ((database->extractOType(children2[i]) ==OT_Analog)&&(tempchildren.getNumberOfElements()!=0))
					{


						qDebug()<<"THIS IS Analog";

						qDebug()<<children2[i];//OT_Analog找到了553个


						



						LinkData measurementtypelink;
						ToolUtil::databaseRead(children2[i], AT_MeasurementTypeLink, &measurementtypelink);



						if(measurementtypelink == Ia)//判断是否是A相电流，39个
						{

							LinkData measlink;
							ToolUtil::databaseRead(children2[i], AT_MeasLink, &measlink);//

							



							QString MVPoint_id = QString::number((ObId)measlink);
							MVPointlist.append(MVPoint_id);








						}




						/*
						LinkData measurementtypelink = Ia;
						

						Condition searchmeasurementtypelink[1];
						searchmeasurementtypelink[0].set(AT_MeasurementTypeLink,EQ,&measurementtypelink);
						

						int numElements = 0;
						numElements = database->find(OT_Analog,searchmeasurementtypelink,1);
						ObId* objects = new ObId[numElements];
						database->find(OT_Analog,searchmeasurementtypelink,1, objects, numElements);

						
						for( int i=0;i<numElements;i++)
						{


							QString MVPoint_id = QString::number(children2[i]);
							MVPointlist.append(MVPoint_id);








						}*/










					}
				}

			}
		}


	}

}




				/*
				const ObId *Analog = tempchildren.getObIds();
				//三相总有功功率
				for (int n=0;n<tempchildren.getNumberOfElements();n++)
				{
					LinkData measurementlink;
					database->read(Analog[n],AT_MeasurementTypeLink,&measurementlink);
					if (measurementlink == Ia)

					{

					}


				}*/

				/*
				const ObId *Analog = tempchildren.getObIds();

				for (int n=0;n<tempchildren.getNumberOfElements();n++)
				{
					LinkData measurementlink;
					database->read(Analog[n],AT_MeasurementTypeLink,&measurementlink);
					if (measurementlink == Ia)
					{




					}

			}*/


		

	



	/*
		ContainerData analogChildren;
		int numanalog=ToolUtil::findOTypeFromChildList(obIds1[i], OT_Analog, analogChildren);//每一个pmsbreaker有几个analog


		for (int j = 0;j<numanalog;j++)
		{
			LinkData measurementtypelink = Ia;
			Condition searchmeasurementtypelink[1];
			searchmeasurementtypelink[0].set(AT_MeasurementTypeLink,EQ,&measurementtypelink);
			

			int numElements = 0;
			numElements = database->find(OT_MVPoint,searchmeasurementtypelink,1);
			ObId* objects = new ObId[numElements];
			database->find(OT_MVPoint,searchmeasurementtypelink,1, objects, numElements);



		}

	}

	*/
		/*
		ContainerData breakChildren;
		database->read(pmsbreaker,AT_ChildrenList,&breakChildren);
		int numChildren = breakChildren.getNumberOfElements();
		const ObId *children = breakChildren.getObIds();
		for (int i=0;i<numChildren;i++)
		{
			ContainerData tempchildren;
			database->read(children[i],AT_ChildrenList,&tempchildren);
			if ((database->extractOType(children[i]) ==OT_PMSTerminal)&&(tempchildren.getNumberOfElements()!=0))
			{}


		}
	}*/

	
	/*
	OT_MVPoint = database->matchOType("MVPoint");

	//量测类型关联
	//关联一次设备两个条件
	LinkData measurementtypelink = Ia;
	LinkData psrlink = NULL;

	Condition searchmeasurementtypeandpsrlink[2];
	searchmeasurementtypeandpsrlink[0].set(AT_MeasurementTypeLink,EQ,&measurementtypelink);
	searchmeasurementtypeandpsrlink[1].set(AT_PSRLink,NE,&psrlink);

	int numElements = 0;
	numElements = database->find(OT_MVPoint,searchmeasurementtypeandpsrlink,2);
	ObId* objects = new ObId[numElements];
	database->find(OT_MVPoint,searchmeasurementtypeandpsrlink,2, objects, numElements);







	for( int i=0;i<numElements;i++)
	{
		
			
			QString MVPoint_id = QString::number(objects[i]);
			MVPointlist.append(MVPoint_id);
		
		


			



	}

	*/
	


ObId initializtion::getMeasurementTypeLink(ObId dmsObjId)
{
	AType at_measurementtypelink;
	try
	{
		at_measurementtypelink= database->matchAType("MeasurementTypeLink");
	}
	catch(Exception& e)
	{
		ToolUtil::myDebug(QString::number(dmsObjId)+": DATABASE Extract OT ERROR");
		return 0;
	}
	LinkData measurementtypelink;
	try
	{
		database->read(dmsObjId,at_measurementtypelink,&measurementtypelink);
	}
	catch(Exception& e)
	{
		ToolUtil::myDebug(QString::number(dmsObjId)+": DATABASE Extract OT ERROR");
		return 0;
	}
	return (ObId)measurementtypelink;
}


int initializtion::BreakerType(ObId dmsObjId) //get PMSBreaker BreakerType
{
	AType at_BreakerType;
	try
	{
		at_BreakerType= database->matchAType("BreakerType");
	}
	catch(Exception& e)
	{
		return -1;
	}
	ChoiceData typeData;
	try
	{
		database->read(dmsObjId,at_BreakerType,&typeData);
	}
	catch(Exception& e)
	{
		return -1;
	}
	return (int)typeData;
}



