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
    Q_OBJECT //ʹ���ź������Ҫ�ĺ�
public:
    explicit Information(QWidget *parent = 0);  //explicit ��ֹ����
    ~Information();


	void showFAaccidentInfo();
	void showMaintenancePlan();
	void showDPCOptRecord();
	void showFeederOverload();


	void firstShowTableViewsFromOldTables(QDateTime signoutdate,QString signoutshift);



	


	

signals:


 
public slots:


private slots:
		void receivesignoutcurDateTimeData(QDateTime data);


		void receivesignoutnameData(QString data);

		void receivesignoutTime_QDateEditData(QDateTime data);

		void receivesignout_shiftData(QString data);


    
private:


	Ui::Information *ui;

	

};
 
#endif // INFORMATION_H