#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>


#include <QCoreApplication>

#include "common.h"
#include "ToolUtil.h"

#ifndef SIGNININFORMATION_H
#define SIGNININFORMATION_H
 
namespace Ui {

class SignInInformation;

}
 
 
class SignInInformation : public QDialog
{
    Q_OBJECT
public:
    explicit SignInInformation(QWidget *parent = 0);
    ~SignInInformation();
	void showsigninFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void showsigninMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void showsigninDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void showsigninFeederOverload(QString strstarttime, QString strendtime, QString startshift);
	



	
	




	QDateTime getSignPageTime(QDateTime signcurtime);
	QDateTime getSignPageDate(QDateTime signdate);
	QString getSignPageShift(QString signshift);
	QDateTime getcurDateTimedata();

	
	
	
	

signals:


 
public slots:

	void on_okButton_clicked();
private slots:

	    
private:


	Ui::SignInInformation *ui;

	QDateTime informationcurDateTime;


	QStandardItemModel* FAInfo_model;
	QStandardItemModel* MaintenancePlan_model;
	QStandardItemModel* DPCPoint_model;
	QStandardItemModel* FeederOverload_model;



	QDateTime signpagetime;

	QDateTime signpagedate;

	QString signpageshift;








	
	

};
 
#endif // SIGNININFORMATION_H