#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>

#ifndef CONFIRM_H
#define CONFIRM_H
 
namespace Ui {

class Confirm;

}
 
 
class Confirm : public QDialog
{
    Q_OBJECT //ʹ���ź������Ҫ�ĺ�
public:
    explicit Confirm(QWidget *parent = 0);  //explicit ��ֹ����
    ~Confirm();

	QDateTime getcurDateTimedata();
	QString getsigninnamedata();
	QString getsignoutnamedata();
	QDateTime getsigninandoutTime_QDateEditdata();
	QString getsigninandout_shitfdata();
	QString getsigninandoutNote_QTextEditdata();
	QString getworkplacenamedata();

signals:
 
public slots:

	 void on_okButton_clicked();
	private slots:
		//void insertSignInAndOutTable();
private:
	
	//����private������Ϣ������������ʹ��
	//1��ȡ��ǰ����ʱ��
	QDateTime curDateTime;

	//2��ȡ�û�ѡȡ�Ľ�����
	QString signoutname;

	//3��ȡ�û�ѡȡ�ĽӰ���
	QString signinname;

	//4��ȡ���Ӱ�ʱ��
	QDateTime signinandoutTime_QDateEdit;

	//5��ȡ���Ӱ����
	QString signinandout_shitf;

	//6��ȡ���ӱ�ע����
	QString signinandoutNote_QTextEdit;

	//7��ȡ�û�ѡȡ���ϰ�ص�
	QString workplacename;




	Ui::Confirm *ui;

};
 
#endif // CONFIRM_H