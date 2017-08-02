#include "warnline_mst.h"
#include "ui_warnline_mst.h"

warnLine_mst::warnLine_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warnLine_mst)
{
    ui->setupUi(this);
}

warnLine_mst::~warnLine_mst()
{
    delete ui;
}

void warnLine_mst::on_pushButton_clicked()
{
    addWarn = new add_warn(this);
    //addWarn->show();
    addWarn->setModal(true);
    addWarn->exec();
}
