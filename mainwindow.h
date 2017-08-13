#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "common_general.h"
#include "category_mst.h"
#include "warnline_mst.h"
#include "recur_expenses_mst.h"
#include "add_expense.h"
#include "expenses_mst.h"
#include "recurring_expenses.h"
#include "spending_limit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_addCat_clicked();

    void on_btn_addWarn_clicked();

    void on_btn_addRecur_clicked();

    void on_btn_addExp_clicked();

    void on_btn_viewExp_clicked();

private:
    Ui::MainWindow *ui;
    common_general *comGen;
    Category_mst *categoryMst;
    warnLine_mst *warnLineMst;
    recur_expenses_mst *recurMst;
    add_expense *addExp;
    expenses_mst *expMst;
    recurring_expenses *recurring;
    spending_limit *spendLimit;

    QString getTodayDay();
    QString getTodayMonth();
    QString getTodayYear();
    void showTodayExpenses();
    void showExceedWarning();

};

#endif // MAINWINDOW_H
