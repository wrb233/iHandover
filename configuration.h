#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>

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

private:
    Ui::Configuration *ui;
};

#endif // CONFIGURATION_H
