#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>

#include <QWidget>
#include <QItemDelegate>  
#include <QLineEdit>
#include <QDateTime>

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = 0);
    ~Configuration();

	
public slots:

		
	void on_btn_Add_clicked();
	void on_btn_Del_clicked();
	void on_btn_okButton_clicked();
	void initConfiguration();



	int whichShift();

	bool isTimeScale(int startTimeHour, int endTimeHour);

	QString leftTime(QDateTime startdate,QString work_index);

	QString rightTime(QDateTime enddate,QString work_index);

private:
    Ui::Configuration *ui;
	
};

#endif // CONFIGURATION_H
