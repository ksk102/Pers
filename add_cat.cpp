#include "add_cat.h"
#include "ui_add_cat.h"

add_cat::add_cat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_cat)
{
    ui->setupUi(this);
}

add_cat::~add_cat()
{
    delete ui;
}
