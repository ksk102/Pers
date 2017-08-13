#ifndef EXPENSES_MST_H
#define EXPENSES_MST_H

#include <QDialog>
#include <QTreeWidget>
#include "common_general.h"
#include "common_expenses.h"
#include "add_expense.h"
#include "edit_expense.h"

namespace Ui {
class expenses_mst;
}

class expenses_mst : public QDialog
{
    Q_OBJECT

public:
    explicit expenses_mst(QWidget *parent = 0);
    ~expenses_mst();

private slots:
    void on_btn_add_clicked();

    void on_btn_edit_clicked();

    void on_tw_expList_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_btn_delete_clicked();

    void on_btn_search_clicked();

    void on_lst_month_from_currentIndexChanged(const QString &arg1);

    void on_lst_year_from_currentIndexChanged(const QString &arg1);

    void on_lst_month_to_currentIndexChanged(const QString &arg1);

    void on_lst_year_to_currentIndexChanged(const QString &arg1);

private:
    QStringList searchString;

    Ui::expenses_mst *ui;
    common_general *comGen;
    common_expenses *comExp;
    add_expense *addExp;
    edit_expense *editExp;

    void retrieveCategory(); //retrieve category from text file
    void setSearchString(); //get searchString from userinputs
    void setColumnInfo();
    void retrieveExpensesList(); //retrieve records from file
    QStringList retrieveRecords(); //retrieve listing from file
    void showListing(QString, QString, QString, QString, QString, QString); //show listing on the treewidget
    void initialDateSelection(); //get the date selection in dropdown list
    void identifyDayinMonthFrom(int, QString); //identify the day in a month
    void identifyDayinMonthTo(int, QString); //identify the day in a month
};

#endif // EXPENSES_MST_H
