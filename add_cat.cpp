#include "add_cat.h"
#include "ui_add_cat.h"
#include <QMessageBox>
#include <QStringList>

add_cat::add_cat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_cat)
{
    ui->setupUi(this);
    comGen = new common_general();
    comGen->setFileName("category.txt");
}

add_cat::~add_cat()
{
    delete ui;
    delete comGen;
}

void add_cat::on_btn_confirm_clicked()
{
    QString catName = ui->txt_catName->text();

    if(!(preCheckUserInput(catName))){
        return;
    }

    QStringList passData;
    passData << catName;

    if(comGen->writeNewRecord(passData, "-1")){
        QMessageBox::information(this,"Sucessful","\""+catName+"\" has been sucessfully added into system.");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",catName+" is already exist");
        ui->txt_catName->selectAll();
        ui->txt_catName->setFocus();
    }
}

void add_cat::on_btn_cancel_clicked()
{
    this->close();
}

bool add_cat::preCheckUserInput(QString name)
{
    if(name == ""){
        QMessageBox::warning(this,"unsuccessful","Please enter a name");
        ui->txt_catName->setFocus();

        return false;
    }
    else if(name.contains("|||", Qt::CaseInsensitive)){
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_catName->selectAll();
        ui->txt_catName->setFocus();

        return false;
    }
    else{
        return true;
    }
}
