#ifndef ADD_EXPENSE_H
#define ADD_EXPENSE_H

#include <QDialog>
#include "common_general.h"
#include "common_expenses.h"
#include "spending_limit.h"

namespace Ui {
class add_expense;
}

class add_expense : public QDialog
{
    Q_OBJECT

public:
    explicit add_expense(QWidget *parent = 0);
    ~add_expense();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

    void on_lst_year_currentIndexChanged(const QString &arg1);

    void on_lst_month_currentIndexChanged(const QString &arg1);

private:
    Ui::add_expense *ui;
    common_general *comGen;
    common_expenses *comExp;
    spending_limit *spendLimit;

    void retrieveCategory(); //retrieve category from text file
    void initialDateSelection(); //get the date selection in dropdown list
    void initialTimeSelection(); //get the time selection in dropdown list
    void identifyDayinMonth(int, QString); //identify the day in a month
    bool preCheckUserInput(QString, QString, QString); //check user input
};

#endif // ADD_EXPENSE_H
