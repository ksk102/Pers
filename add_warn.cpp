#include "add_warn.h"
#include "ui_add_warn.h"
#include <QMessageBox>

add_warn::add_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_warn)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("warnline.txt");

    retrieveCategory(); //retrieve category from text file

    //to show or hide day type listbox
    QString warnType = ui->lst_warnType->currentText();
    showHideDayType(warnType);
}

add_warn::~add_warn()
{
    delete ui;
    delete comGen;
}

void add_warn::on_btn_confirm_clicked()
{
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

    //check user input
    if(!(preCheckUserInput(warnName, warnPoint, warnType))){
        return;
    }

    //data to save into new file
    QStringList passData;
    passData << warnName << warnPoint << warnType << warnDayType << warnCatId;

    //write new record into file
    //-1 parameter here is to by editting function,
    //since this is add new,
    //so use -1 to indicate it is not from edit
    if(comGen->writeNewRecord(passData, "-1")){
        QMessageBox::information(this,"Sucessful","\""+warnName+"\" has been sucessfully added into system.");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",warnName+" is already exist");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();
    }
}

void add_warn::on_btn_cancel_clicked()
{
    this->close();
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

void add_warn::retrieveCategory()
{
    QStringList recordsList = comGen->retrieveRecords("category.txt", ""); //call the method to get the records from text file

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

bool add_warn::preCheckUserInput(QString name, QString point, QString type)
{
    comWarn = new common_warn();
    int returnInt = comWarn->preCheckUserInput(name, point, type);

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
            QMessageBox::warning(this,"unsuccessful","Please enter the warning amount");
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

void add_warn::on_lst_warnType_currentIndexChanged(const QString &arg1)
{
    showHideDayType(arg1);
}
