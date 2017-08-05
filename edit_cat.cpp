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
    int userInputCheck = gen->checkUserInput("category.txt", catName, this->selectedRow); //check user input

    switch(userInputCheck){
        case 1:
            QMessageBox::warning(this,"Unsucessful",catName+" is already exist");
            ui->txt_catName->setText("");
            break;
        case 2:
            QMessageBox::critical(this,"Unsucessful","Couldn't open the file");
            break;
        case 3: //user input is empty
            QMessageBox::warning(this,"Unsucessful","Please enter a new category to add into system");
            ui->txt_catName->setFocus();
            break;
        case 4:
            QMessageBox::warning(this,"Unsucessful","Invalid symbol \"|||\"");
            ui->txt_catName->setText("");
            break;
        default:
            editCategory(catName, this->selectedRow); //edit the record on text file
            this->close();
            break;
    }
}

void edit_cat::on_btn_cancel_clicked()
{
    this->close();
}

void edit_cat::editCategory(QString catName, int currentRow)
{
    QString filename = "category";
    QString ori_filename = filename + ".txt";
    QString tmp_filename = filename +"_tmp.txt";

    //open file
    QFile file(ori_filename);
    QFile file_tmp(tmp_filename);

    //open file for write only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,"Unsucessful","Error occur while trying to edit the record");
    }
    if(!file_tmp.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this,"Unsucessful","Error occur while trying to edit the record");
    }

    QTextStream in(&file);
    QTextStream out(&file_tmp);
    QString line;

    int counter = 0;
    while (!in.atEnd()) {

       line = in.readLine();

       if(counter != currentRow){
           out << line << endl;
       }
       else{
           out << catName << endl;
       }
       counter++;

    }
    file.close();
    file_tmp.flush();
    file_tmp.close();

    QFile::remove(ori_filename);
    QFile::rename(tmp_filename,ori_filename);

    //show sucessful alert box
    QMessageBox::information(this,"Sucessful","\""+catName+"\" has been sucessfully added into system.");
}

void edit_cat::retrieveRecord(int selectedRow)
{
    this->selectedRow = selectedRow;

    //open file
    QFile file("category.txt");

    //open file for write only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,"Unsucessful","Unable to read the file");
        return;
    }

    QTextStream in(&file);
    QString line;

    int counter = 0;
    while (!in.atEnd()) {

       line = in.readLine();

       if(counter == selectedRow){
           ui->txt_catName->setText(line);
           break;
       }
       counter++;

    }

    file.close();
}
