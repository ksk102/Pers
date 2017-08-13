#include "edit_cat.h"
#include "ui_edit_cat.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

edit_cat::edit_cat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_cat)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("category.txt");
    comGen->setTmpFileName("category_tmp.txt");
}

edit_cat::~edit_cat()
{
    delete ui;
    delete comGen;
}

void edit_cat::on_btn_confirm_clicked()
{
    QString catName = ui->txt_catName->text();

    if(!(preCheckUserInput(catName))){
        return;
    }

    QStringList passData;
    passData << catName;

    if(comGen->writeNewRecord(passData, this->selectedId)){
        QMessageBox::information(this,"Sucessful","\""+catName+"\" has been sucessfully added into system.");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",catName+" is already exist");
        ui->txt_catName->selectAll();
        ui->txt_catName->setFocus();
    }
}

void edit_cat::on_btn_cancel_clicked()
{
    this->close();
}

bool edit_cat::preCheckUserInput(QString name)
{
    if(name == ""){
        QMessageBox::warning(this,"Unsuccessful","Please enter a name");
        ui->txt_catName->setFocus();

        return false;
    }
    else if(name.contains("|||", Qt::CaseInsensitive)){
        QMessageBox::warning(this,"Unsuccessful","Invalid symbol \"|||\"");
        ui->txt_catName->selectAll();
        ui->txt_catName->setFocus();

        return false;
    }
    else{
        return true;
    }
}

void edit_cat::retrieveRecord(QString curId)
{
    this->selectedId = curId;

    QString returnString = comGen->retrieveEditRecord("", curId);
    QStringList currentRecord;

    if(returnString == "|||||-2"){ //unable to find the record
        QMessageBox::critical(this,"unsuccessful","Error occured while trying to get the record");
        return;
    }
    else{
        currentRecord = returnString.split("|||||"); //split the data
        ui->txt_catName->setText(currentRecord[1]);
    }
}
