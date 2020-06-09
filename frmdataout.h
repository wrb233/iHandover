#ifndef FRMDATAOUT_H
#define FRMDATAOUT_H

#define RowCount 100
#define MaxCount1 5000
#define MaxCount2 10000
#define MaxCount3 10000

#include <QWidget>

class QTableView;
class QTableWidget;

namespace Ui {
class frmDataOut;
}

class frmDataOut : public QWidget
{
    Q_OBJECT

public:
    explicit frmDataOut(QWidget *parent = 0);
    ~frmDataOut();

public:
    static int getCount();
    static void initTable(QTableView *table);
    static void initTable1(QTableWidget *table);
    static void loadTable1(QTableWidget *table, int maxCount);
    static void loadTable2(QTableView *table);
    static void loadTable3(QTableView *table);
    static void initColumnFAaccidentInfo(QList<QString> &columnNames, QList<int> &columnWidths);
    static void initColumnMaintenancePlan(QList<QString> &columnNames, QList<int> &columnWidths);
    static void initColumnDPCOptRecord(QList<QString> &columnNames, QList<int> &columnWidths);
    static void initColumnFeederOverload(QList<QString> &columnNames, QList<int> &columnWidths);

private:
    Ui::frmDataOut *ui;

private slots:
    void initForm();
};

#endif // FRMDATAOUT_H
