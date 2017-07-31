#include "add_cat.h"
#include "ui_add_cat.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>

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

    if(catName == ""){
        QMessageBox::warning(this,"Unsucessful","Please enter a new category to add into system");

        ui->txt_catName->setFocus();
    }
    else{
        writeNewCategory(catName);
        this->close();
    }
}

void add_cat::on_btn_cancel_clicked()
{
    this->close();
}

void add_cat::writeNewCategory(QString catName)
{
    //open file
    QFile file("category.txt");

    //append new entry into file
    if(!file.open(QFile::Append | QFile::Text))
    {
        //show unsucessful alert box
        QMessageBox::critical(this,"Unsucessful","Error occur while adding \""+catName+"\" into the system");
        return;
    }

    //writing new entry into file
    QTextStream out(&file);
    out << catName << endl;
    file.flush();
    file.close();

    //show sucessful alert box
    QMessageBox::information(this,"Sucessful","\""+catName+"\" have been sucessfully added into system.");
}
