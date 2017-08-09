#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "category_mst.h"
#include "warnline_mst.h"
#include "recur_expenses_mst.h"

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

private:
    Ui::MainWindow *ui;
    Category_mst *categoryMst;
    warnLine_mst *warnLineMst;
    recur_expenses_mst *recurMst;
};

#endif // MAINWINDOW_H
