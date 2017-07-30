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
    cat_mst = new Category_mst(this);
    cat_mst->show();
}

void MainWindow::on_btn_addWarn_clicked()
{
    warning_mst = new warnLine_mst(this);
    warning_mst->show();
}
