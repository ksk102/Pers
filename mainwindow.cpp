#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
