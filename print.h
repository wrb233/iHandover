#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef PRINT_H
#define PRINT_H


#include <QObject>
#include <QStringList>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtPrintSupport>
#else
#include <QPrinter>
#endif
 
namespace Ui {

class Print;

}
 
 
class Print : public QDialog
{
    Q_OBJECT //使用信号与槽需要的宏
public:

	static Print *Instance();
    explicit Print(QWidget *parent = 0);  //explicit 防止歧义
    ~Print();

	
	QDateTime getfromDateTimedata();
	QString getfromshiftdata();
	QDateTime gettoDateTimedata();
	QString gettoshiftdata();

	QString getstrstarttimedata();
	QString getstrendtimedata();

signals:



	public slots:
		//void SignOutlogin();//点击接班登录按钮执行的槽函数
		void on_okButton_clicked();

		
		void print(const QString &title, const QString &subTitle,
			const QList<QString> &columnNames, const QList<int> &columnWidths,
			const QStringList &content, bool landscape = false, bool check = false,
			int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
			const QPrinter::PageSize &pageSize = QPrinter::A4);

		//打印数据,分段形式,带子标题
		void print(const QString &title, const QList<QString> &columnNames, const QList<int> &columnWidths,
			const QStringList &subTitle1, const QStringList &subTitle2,
			const QStringList &content, bool landscape = false, bool check = false,
			int checkColumn = 0, const QString &checkType = "==", const QString &checkValue = "0",
			const QPrinter::PageSize &pageSize = QPrinter::A4);

    
	private slots:
		//void showInformationDialog();

		//void printReport(QPrinter* printer);
		void printView(QPrinter* printer);



		//void initForm();


private:

	static QScopedPointer<Print> self;

	QStringList html;

	QDateTime fromTime_QDateEdit;

	
	QString from_shitf;

	QDateTime toTime_QDateEdit;


	QString to_shitf;


	QString strstarttime;
	QString strendtime;
	

	Ui::Print *ui;

};
 
#endif // PRINT_H