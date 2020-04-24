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
    Q_OBJECT //使用信号与槽需要的宏
public:
    explicit Information(QWidget *parent = 0);  //explicit 防止歧义
    ~Information();


	void firstshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void firstshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void firstshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void firstshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);
	void firstShowTableViewsFromOldTables(QString strstarttime, QString strendtime, QString startshift);
	
	void secondShowTableViewsFromOldTables(QString strstarttime, QString strendtime, QString startshift);
	void secondshowFAaccidentInfo(QString strstarttime, QString strendtime, QString startshift);
	void secondshowMaintenancePlan(QString strstarttime, QString strendtime, QString startshift);
	void secondshowDPCOptRecord(QString strstarttime, QString strendtime, QString startshift);
	void secondshowFeederOverload(QString strstarttime, QString strendtime, QString startshift);

	QDateTime getSignPageTime(QDateTime signcurtime);
	QDateTime getSignPageDate(QDateTime signdate);
	QString getSignPageShift(QString signshift);



	void firstwriteTables(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void firstwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);

	void secondwriteTables(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	void secondwriteFAaccidentTable(QString strstarttime, QString strendtime, QString startshift, QDateTime informationcurDateTime);
	
	
public:
	QDateTime getcurDateTimedata();
	

	

signals:


 
public slots:

	void on_okButton_clicked();
private slots:

	    /*
		void receivesignoutcurDateTimeData(QDateTime data);


		void receivesignoutnameData(QString data);

		void receivesignoutTime_QDateEditData(QDateTime data);

		void receivesignout_shiftData(QString data);

		//void writeTables(QString strstarttime, QString strendtime, QString startshift);
        */
private:
	QDateTime informationcurDateTime;










	Ui::Information *ui;

	

};
 
#endif // INFORMATION_H