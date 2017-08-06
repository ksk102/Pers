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
}

edit_cat::~edit_cat()
{
    delete ui;
}

void edit_cat::on_btn_confirm_clicked()
{
    QString catName = ui->txt_catName->text();

    gen = new general();
    int userInputCheck = gen->checkUserInput("category.txt", catName, this->selectedId); //check user input

    switch(userInputCheck){
        case 1:
            QMessageBox::warning(this,"unsuccessful",catName+" is already exist");
            ui->txt_catName->selectAll();
            ui->txt_catName->setFocus();
            break;
        case 2:
            QMessageBox::critical(this,"unsuccessful","Couldn't open the file");
            break;
        case 3: //user input is empty
            QMessageBox::warning(this,"unsuccessful","Please enter a new category to add into system");
            ui->txt_catName->setFocus();
            break;
        case 4:
            QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
            ui->txt_catName->selectAll();
            ui->txt_catName->setFocus();
            break;
        default:
            editCategory(catName, this->selectedId); //edit the record on text file
            this->close();
            break;
    }
}

void edit_cat::on_btn_cancel_clicked()
{
    this->close();
}

void edit_cat::editCategory(QString catName, QString curId)
{
    QString filename = "category";

    gen = new general();
    bool returnbool = gen->editRecord(filename, catName, curId);

    if(returnbool){
        QMessageBox::information(this,"Sucessful","\""+catName+"\" has been sucessfully added into system.");
    }
    else{
        QMessageBox::critical(this,"unsuccessful","Error occur while trying to edit the record");
    }
}

void edit_cat::retrieveRecord(QString curId)
{
    this->selectedId = curId;

    gen = new general();
    QString returnString = gen->retrieveEditRecord("category.txt", curId);
    QStringList currentRecord;

    if(returnString == "|||||-1"){
        QMessageBox::critical(this,"unsuccessful","Unable to read the file");
        return;
    }
    else if(returnString == "|||||-2"){ //unable to find the record
        QMessageBox::critical(this,"unsuccessful","Error occured while trying to get the record");
        return;
    }
    else{
        currentRecord = returnString.split("|||||"); //split the data
        ui->txt_catName->setText(currentRecord[1]);
    }
}
