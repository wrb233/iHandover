#include "datedelegate.h"
#include <QDateTimeEdit>
#include <QDateEdit>

DateDelegate::DateDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateTimeEdit *editer = new QDateTimeEdit(parent);

	QDateTime current_date_time =QDateTime::currentDateTime();
	//QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	//QDate date = QDate::fromString(current_date,"yyyyMMddhhmmss");
	editer->setDateTime(current_date_time);
	
    editer->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    editer->setCalendarPopup(true);
    editer->installEventFilter(const_cast<DateDelegate*>(this));
    return editer;
}

void DateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString dateStr = index.model()->data(index).toString();
    QDate date = QDate::fromString(dateStr,Qt::ISODate);
    QDateTimeEdit *edit = static_cast<QDateTimeEdit *>(editor);
    edit->setDate(date);
}

void DateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *edit = static_cast<QDateTimeEdit *>(editor);
    QDate date = edit->date();
    model->setData(index,QVariant(date.toString(Qt::ISODate)));
}

void DateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
