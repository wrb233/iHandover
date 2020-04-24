#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "libUserMS/UserMSInterface.h"
#ifndef SIGNIN_H
#define SIGNIN_H
 
namespace Ui {

class SignIn;

}
 
 
class SignIn : public QDialog
{
    Q_OBJECT //ʹ���ź������Ҫ�ĺ�
public:
    explicit SignIn(QWidget *parent = 0);  //explicit ��ֹ����
    ~SignIn();

	QDateTime getcurDateTimedata();
	QString getsigninnamedata();
	QDateTime getsigninTime_QDateEditdata();
	QString getsignin_shitfdata();


signals:



	public slots:
		//void SignOutlogin();//����Ӱ��¼��ťִ�еĲۺ���
		void on_okButton_clicked();

    
	private slots:
		//void showInformationDialog();
private:

	//����private������Ϣ������������ʹ��
	//1��ȡ��ǰ����ʱ��
	QDateTime signincurDateTime;

	//2��ȡ�û�ѡȡ�Ľ�����
	QString signinname;



	//3��ȡ���Ӱ�ʱ��
	QDateTime signinTime_QDateEdit;

	//4��ȡ���Ӱ����
	QString signin_shitf;

	//�û�
	std::list<UserObjectData>* signinlistUser;

	Ui::SignIn *ui;

};
 
#endif // SIGNIN_H