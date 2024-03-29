#include "edit_recur.h"
#include "ui_edit_recur.h"
#include <QMessageBox>

edit_recur::edit_recur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_recur)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("recurring.txt");
    comGen->setTmpFileName("recurring_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    //to show or hide day type listbox
    QString recurType = ui->lst_recurType->currentText();
    showHideDayType(recurType);
}

edit_recur::~edit_recur()
{
    delete ui;
    delete comGen;
}

void edit_recur::showHideDayType(QString type)
{
    if(type == "Daily"){
        ui->lst_daytype->show();
    }
    else{
        ui->lst_daytype->setCurrentIndex(0);
        ui->lst_daytype->hide();
    }
}

void edit_recur::retrieveCategory()
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

void edit_recur::on_btn_cancel_clicked()
{
    this->close();
}

void edit_recur::retrieveRecord(QString curId)
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

        QString recurName = currentRecord[1];
        QString recurAmt = currentRecord[2];
        QString recurCat = currentRecord[3];
        QString recurCatName = comGen->convertIdName("category.txt", recurCat);
        QString recurType = currentRecord[4];
        QString recurDayType = currentRecord[5];

        ui->txt_name->setText(recurName);
        ui->txt_amount->setText(recurAmt);
        ui->lst_cat->setCurrentText(recurCatName);
        ui->lst_recurType->setCurrentText(recurType);
        ui->lst_daytype->setCurrentText(recurDayType);
    }
}

void edit_recur::on_btn_confirm_clicked()
{
    //get user input
    QString recurName = ui->txt_name->text();
    QString recurAmt = ui->txt_amount->text();
    int recur_catId = ui->lst_cat->currentData().toInt();
    QString recurCatId = QString::number(recur_catId);
    QString recurType = ui->lst_recurType->currentText();
    QString recurDayType = ui->lst_daytype->currentText();

    //return empty if user didn't select a type
    recurType = comGen->listBoxIsEmpty(recurType);

    //if user didn't select 'daily' as the recurring type, then save recurDayType as empty
    if(!comGen->DayTypeString(recurType)){
        recurDayType = "";
    }

    //check user input
    if(!(preCheckUserInput(recurName, recurAmt, recurType))){
        return;
    }

    //data to save into new file
    QStringList passData;
    passData << recurName << recurAmt << recurCatId << recurType << recurDayType;

    //write record into file
    if(comGen->writeNewRecord(passData, this->selectedId)){
        QMessageBox::information(this,"Sucessful","Sucessfully editted the record");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",recurName+" is already exist");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();
    }
}

bool edit_recur::preCheckUserInput(QString name, QString amount, QString type)
{
    commonRecur = new common_recur();
    int returnInt = commonRecur->preCheckUserInput(name, amount, type);

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
        ui->txt_amount->selectAll();
        ui->txt_amount->setFocus();

        return false;
        break;
    case -4:
        QMessageBox::warning(this,"unsuccessful","Please enter the recurring amount");
        ui->txt_amount->selectAll();
        ui->txt_amount->setFocus();

        return false;
        break;
    case -5:
        QMessageBox::warning(this,"unsuccessful","Please enter only digits for amount");
        ui->txt_amount->selectAll();
        ui->txt_amount->setFocus();

        return false;
        break;
    case -6:
        QMessageBox::warning(this,"unsuccessful","Please select a recurring type");
        ui->lst_recurType->setFocus();

        return false;
        break;
    default:
        return true;
        break;
    }
}

void edit_recur::on_lst_recurType_currentIndexChanged(const QString &arg1)
{
    showHideDayType(arg1);
}
