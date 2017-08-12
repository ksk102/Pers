#ifndef EDIT_EXPENSE_H
#define EDIT_EXPENSE_H

#include <QDialog>
#include <QStringList>
#include "common_general.h"
#include "common_expenses.h"

namespace Ui {
class edit_expense;
}

class edit_expense : public QDialog
{
    Q_OBJECT

public:
    explicit edit_expense(QWidget *parent = 0);
    ~edit_expense();

    void retrieveRecord(QString); //get the data into the textbox

private slots:
    void on_lst_month_currentIndexChanged(const QString &arg1);

    void on_lst_year_currentIndexChanged(const QString &arg1);

    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::edit_expense *ui;
    common_general *comGen;
    common_expenses *comExp;

    QString selectedId; //current selected record

    bool preCheckUserInput(QString, QString, QString); //check user input
    void retrieveCategory(); //retrieve category from text file
    void initialDateSelection(); //get the date selection in dropdown list
    void initialTimeSelection(); //get the time selection in dropdown list
    void identifyDayinMonth(int, QString); //identify the day in a month
    QStringList convertDate(QString); //convert from yyyyMMdd to dd/MM/yyyy
    QStringList convertTime(QString); //convert from hhmmAP to hh:mm AP
};

#endif // EDIT_EXPENSE_H
