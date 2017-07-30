#include "category_mst.h"
#include "ui_category_mst.h"

Category_mst::Category_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Category_mst)
{
    ui->setupUi(this);
}

Category_mst::~Category_mst()
{
    delete ui;
}

void Category_mst::on_pushButton_clicked()
{
    addcat = new add_cat(this);
    addcat->show();
}
