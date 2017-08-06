#include "add_warn.h"
#include "ui_add_warn.h"
#include <QMessageBox>

add_warn::add_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_warn)
{
    ui->setupUi(this);

    QString warnType = ui->list_warnType->currentText();

    showHideDayType(warnType);

    if(!(retrieveCategory())){
         QMessageBox::critical(this,"unsuccessful","Unable to retrieve category");
    }

}

add_warn::~add_warn()
{
    delete ui;
}

void add_warn::on_btn_confirm_clicked()
{
    QString warnName = ui->txt_name->text();
    QString warnPoint = ui->txt_point->text();
    QString warnType = ui->list_warnType->currentText();
    QString warnDayType = ui->lst_daytype->currentText();
    int warn_catId = ui->list_cat->currentData().toInt();
    QString warnCatId = QString::number(warn_catId);

    warnType = listBoxIsEmpty(warnType);
    warnDayType = listBoxIsEmpty(warnDayType);

    if(!(preCheckUserInput(warnName, warnPoint, warnType))){
        return;
    }

    QString passData = warnName + "|||||" + warnPoint + "|||||" + warnType + "|||||" + warnDayType + "|||||" + warnCatId;

    int returnString = 0;

    gen = new general();
    int userInputCheck = gen->checkUserInput("warnline.txt", passData, "-1"); //check user input

    switch(userInputCheck){
        case 1:
            QMessageBox::warning(this,"unsuccessful",warnName+" is already exist");
            ui->txt_name->setText("");
            break;
        case 2:
            QMessageBox::critical(this,"unsuccessful","Couldn't open the file");
            break;
        case 3: //user input is empty
            QMessageBox::warning(this,"unsuccessful","Please enter a warning line to add into system");
            ui->txt_name->setFocus();
            break;
        case 4:
            QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
            ui->txt_name->setText("");
            break;
        default:
            returnString = gen->writeNewRecord("warnline.txt", passData); //add into text file
            break;
    }

    switch(returnString){
        case -1:
            QMessageBox::critical(this,"unsuccessful","Couldn't open the file");
        break;
        case 1:
            QMessageBox::information(this,"Sucessful","\""+warnName+"\" has been sucessfully added into system.");
            this->close();
        break;
        default: //is not a result from previous switch case
        break;
    }
}

void add_warn::on_btn_cancel_clicked()
{
    this->close();
}

void add_warn::on_list_warnType_currentIndexChanged(const QString &arg1)
{
    showHideDayType(arg1);
}

void add_warn::showHideDayType(QString warnType)
{
    if(warnType == "Day"){
        ui->lst_daytype->show();
    }
    else{
        ui->lst_daytype->setCurrentIndex(0);
        ui->lst_daytype->hide();
    }
}

bool add_warn::retrieveCategory()
{
    gen = new general();
    QStringList recordsList = gen->retrieveRecords("category.txt", ""); //call the method to get the records from text file

    QStringList catRecord;
    QString catId;
    QString catName;

    for(int i=0; i<recordsList.size(); i++){
        catRecord = recordsList[i].split("|||||");
        catId = catRecord[0];
        catName = catRecord[1];

        showListing(catId, catName);
    }

    return true;
}

void add_warn::showListing(QString catId, QString catName)
{
    ui->list_cat->addItem(catName, catId);
}

QString add_warn::listBoxIsEmpty(QString lstboxText)
{
    if(lstboxText == "<Select one>"){
        return "";
    }
    else{
        return lstboxText;
    }
}

bool add_warn::preCheckUserInput(QString warnName, QString warnPoint, QString warnType)
{
    bool validate;
    double dou_warnPoint = warnPoint.toDouble(&validate);

    if(warnName.contains("|||", Qt::CaseInsensitive)){
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_name->setText("");
        ui->txt_name->setFocus();

        return false;
    }
    else if(warnPoint.contains("|||", Qt::CaseInsensitive)){
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_point->setText("");
        ui->txt_point->setFocus();

        return false;
    }
    else if(warnPoint == ""){
        QMessageBox::warning(this,"unsuccessful","Please enter the warning milestone");
        ui->txt_point->setFocus();

        return false;
    }
    else if(!validate){
        QMessageBox::warning(this,"unsuccessful","Please enter only digits for warning point");
        ui->txt_point->setFocus();

        return false;
    }
    else if(warnType == ""){
        QMessageBox::warning(this,"unsuccessful","Please select a warning type");
        ui->list_warnType->setFocus();

        return false;
    }

    return true;
}
