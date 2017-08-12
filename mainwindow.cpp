#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recurring = new recurring_expenses();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete recurring;
}

void MainWindow::on_btn_addCat_clicked()
{
    categoryMst = new Category_mst(this);
    //categoryMst->show();
    categoryMst->setModal(true);
    categoryMst->exec();
}

void MainWindow::on_btn_addWarn_clicked()
{
    warnLineMst = new warnLine_mst(this);
    //warnLineMst->show();
    warnLineMst->setModal(true);
    warnLineMst->exec();
}

void MainWindow::on_btn_addRecur_clicked()
{
    recurMst = new recur_expenses_mst(this);
    recurMst->setModal(true);
    recurMst->exec();
}

void MainWindow::on_btn_addExp_clicked()
{
    addExp = new add_expense(this);
    addExp->setModal(true);
    addExp->exec();
}

void MainWindow::on_btn_viewExp_clicked()
{
    expMst = new expenses_mst(this);
    expMst->setModal(true);
    expMst->exec();
}
