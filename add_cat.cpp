#include "add_cat.h"
#include "ui_add_cat.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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

void add_cat::on_btn_confirm_clicked()
{
    QString catName = ui->txt_catName->text();
    int returnString = 0;

    gen = new general();
    int userInputCheck = gen->checkUserInput("category.txt", catName, "-1"); //check user input

    switch(userInputCheck){
        case 1:
            QMessageBox::warning(this,"unsuccessful",catName+" is already exist");
            ui->txt_catName->setText("");
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
            ui->txt_catName->setText("");
            break;
        default:
            returnString = gen->writeNewRecord("category.txt", catName); //add into text file
            break;
    }

    switch(returnString){
        case -1:
            QMessageBox::critical(this,"unsuccessful","Couldn't open the file");
        break;
        case 1:
            QMessageBox::information(this,"Sucessful","\""+catName+"\" has been sucessfully added into system.");
            this->close();
        break;
        default: //is not a result from previous switch case
        break;
    }
}

void add_cat::on_btn_cancel_clicked()
{
    this->close();
}
