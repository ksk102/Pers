#include "add_warn.h"
#include "ui_add_warn.h"

add_warn::add_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_warn)
{
    ui->setupUi(this);
}

add_warn::~add_warn()
{
    delete ui;
}
