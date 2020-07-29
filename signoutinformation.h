#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>


#include <QCoreApplication>

#include "common.h"
#include "ToolUtil.h"

#ifndef SIGNOUTINFORMATION_H
#define SIGNOUTINFORMATION_H
 
namespace Ui {

class SignOutInformation;

}
 
 
class SignOutInformation : public QDialog
{
    Q_OBJECT
public:
    explicit SignOutInformation(QWidget *parent = 0);
    ~SignOutInformation();
	
	
	
	int getsuccessTimes(QString strstarttime, QString strendtime, QString startshift);
	int getfailTimes(QString strstarttime, QString strendtime, QString startshift);

	
	void firstshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void firstshowMaintenancePlan(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift);
	void firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);
	
	
	void secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void secondshowMaintenancePlan(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift);
	void secondshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void secondshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);




	QDateTime getSignPageTime(QDateTime signcurtime);
	QDateTime getSignPageDate(QDateTime signdate);
	QString getSignPageShift(QString signshift);
	QDateTime getcurDateTimedata();

	
	
	void firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime);
	
	void secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteMaintenancePlanTable(QString strstarttime, QString strendtime, QDateTime startdate, QString startshift, QDateTime informationcurDateTime);
	
	

signals:


 
public slots:

	void on_okButton_clicked();


	void on_btn_Add_clicked();
	void on_btn_Del_clicked();
private slots:

	    
private:

	







	Ui::SignOutInformation *ui;

	QStandardItemModel* FAInfo_model;
	QStandardItemModel* MaintenancePlan_model;
	QStandardItemModel* DPCPoint_model;
	QStandardItemModel* FeederOverload_model;


	QDateTime informationcurDateTime;

	QDateTime signpagetime;

	QDateTime signpagedate;

	QString signpageshift;


	int successTimes;
	int failTimes;


	

};
 
#endif // SIGNOUTINFORMATION_H