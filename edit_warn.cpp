#include "edit_warn.h"
#include "ui_edit_warn.h"
#include <QMessageBox>

edit_warn::edit_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_warn)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("warnline.txt");
    comGen->setTmpFileName("warnline_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    //to show or hide day type listbox
    QString warnType = ui->lst_warnType->currentText();
    showHideDayType(warnType);
}

edit_warn::~edit_warn()
{
    delete ui;
    delete comGen;
}

void edit_warn::retrieveRecord(QString curId)
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

        QString warnName = currentRecord[1];
        QString warnPoint = currentRecord[2];
        QString warnType = currentRecord[3];
        QString warnDayType = currentRecord[4];
        QString warnCat = currentRecord[5];
        QString warnCatName = comGen->convertIdName("category.txt", warnCat);

        ui->txt_name->setText(warnName);
        ui->txt_point->setText(warnPoint);
        ui->lst_warnType->setCurrentText(warnType);
        ui->lst_daytype->setCurrentText(warnDayType);
        ui->lst_cat->setCurrentText(warnCatName);
    }
}

void edit_warn::on_btn_confirm_clicked()
{
    //get user input
    QString warnName = ui->txt_name->text();
    QString warnPoint = ui->txt_point->text();
    QString warnType = ui->lst_warnType->currentText();
    QString warnDayType = ui->lst_daytype->currentText();
    int warn_catId = ui->lst_cat->currentData().toInt();
    QString warnCatId = QString::number(warn_catId);

    //return empty if user didn't select a type
    warnType = comGen->listBoxIsEmpty(warnType);

    //if user didn't select 'day' as the recurring type, then save recurDayType as empty
    if(!comGen->DayTypeString(warnType)){
        warnDayType = "";
    }

    if(!(preCheckUserInput(warnName, warnPoint, warnType))){
        return;
    }

    //data to save into new file
    QStringList passData;
    passData << warnName << warnPoint << warnType << warnDayType << warnCatId;

    //write record into file
    if(comGen->writeNewRecord(passData, this->selectedId)){
        QMessageBox::information(this,"Sucessful","Sucessfully editted the record");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",warnName+" is already exist");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();
    }
}

void edit_warn::on_btn_cancel_clicked()
{
    this->close();
}

void edit_warn::showHideDayType(QString warnType)
{
    if(warnType == "Day"){
        ui->lst_daytype->show();
    }
    else{
        ui->lst_daytype->setCurrentIndex(0);
        ui->lst_daytype->hide();
    }
}

void edit_warn::retrieveCategory()
{
    //call the method to get the records from text file
    QStringList recordsList = comGen->retrieveRecords("category.txt", "");

    QStringList catRecord;
    QString catId;
    QString catName;

    for(int i=0; i<recordsList.size(); i++){
        catRecord = recordsList[i].split("|||||");
        catId = catRecord[0];
        catName = catRecord[1];

        ui->lst_cat->addItem(catName, catId);
    }
}

bool edit_warn::preCheckUserInput(QString warnName, QString warnPoint, QString warnType)
{
    comWarn = new common_warn();
    int returnInt = comWarn->preCheckUserInput(warnName, warnPoint, warnType);

    //if returnInt is
    // -1, name is empty
    // -2, name contain Invalid symbol
    // -3, amount contain Invalid symbol
    // -4, amount is empty
    // -5, amount is not number
    // -6, type is empty
    // 1, all field are enterred correctly

    switch(returnInt){
     case -1:
        QMessageBox::warning(this,"unsuccessful","Please enter a name");
        ui->txt_name->setFocus();

        return false;
        break;
    case -2:
       QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
       ui->txt_name->selectAll();
       ui->txt_name->setFocus();

       return false;
       break;
    case -3:
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_point->selectAll();
        ui->txt_point->setFocus();

        return false;
        break;
    case -4:
        QMessageBox::warning(this,"unsuccessful","Please enter the warning milestone");
        ui->txt_point->selectAll();
        ui->txt_point->setFocus();

        return false;
        break;
    case -5:
        QMessageBox::warning(this,"unsuccessful","Please enter only digits for warning point");
        ui->txt_point->selectAll();
        ui->txt_point->setFocus();

        return false;
        break;
    case -6:
        QMessageBox::warning(this,"unsuccessful","Please select a warning type");
        ui->lst_warnType->setFocus();

        return false;
        break;
    default:
        return true;
        break;
    }
}

void edit_warn::on_lst_warnType_currentIndexChanged(const QString &arg1)
{
    showHideDayType(arg1);
}
