#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>


#include <QCoreApplication>

#ifndef INFORMATION_H
#define INFORMATION_H
 
namespace Ui {

class Information;

}
 
 
class Information : public QDialog
{
    Q_OBJECT
public:
    explicit Information(QWidget *parent = 0);
    ~Information();
	void showsigninFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void showsigninMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void showsigninDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void showsigninFeederOverload(QString strstarttime, QString strendtime, QString startshift);
	



	
	void firstshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void firstshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);
	
	
	void secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void secondshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void secondshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void secondshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);




	QDateTime getSignPageTime(QDateTime signcurtime);
	QDateTime getSignPageDate(QDateTime signdate);
	QString getSignPageShift(QString signshift);
	QDateTime getcurDateTimedata();

	
	
	void firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	
	
	void secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteDPCOptRecordTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteFeederOverloadTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);

	
	

signals:


 
public slots:

	void on_okButton_clicked();
private slots:

	    
private:
	QDateTime informationcurDateTime;

	QDateTime signpagetime;

	QDateTime signpagedate;

	QString signpageshift;








	Ui::Information *ui;

	

};
 
#endif // INFORMATION_H