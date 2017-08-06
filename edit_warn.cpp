#include "edit_warn.h"
#include "ui_edit_warn.h"
#include <QMessageBox>

edit_warn::edit_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_warn)
{
    ui->setupUi(this);

    QString warnType = ui->list_warnType->currentText();

    showHideDayType(warnType);

    if(!(retrieveCategory())){
         QMessageBox::critical(this,"unsuccessful","Unable to retrieve category");
    }

}

edit_warn::~edit_warn()
{
    delete ui;
}

void edit_warn::retrieveRecord(QString curId)
{
    this->selectedId = curId;

    gen = new general();
    QString returnString = gen->retrieveEditRecord("warnline.txt", curId);
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

        QString warnName = currentRecord[1];
        QString warnPoint = currentRecord[2];
        QString warnType = currentRecord[3];
        QString warnDayType = currentRecord[4];
        QString warnCat = currentRecord[5];
        QString warnCatName = gen->convertIdName("category.txt", warnCat);

        ui->txt_name->setText(warnName);
        ui->txt_point->setText(warnPoint);
        ui->list_warnType->setCurrentText(warnType);
        ui->lst_daytype->setCurrentText(warnDayType);
        ui->list_cat->setCurrentText(warnCatName);
    }
}

void edit_warn::on_btn_confirm_clicked()
{
    QString warnName = ui->txt_name->text();
    QString warnPoint = ui->txt_point->text();
    QString warnType = ui->list_warnType->currentText();
    QString warnDayType = ui->lst_daytype->currentText();
    int warn_catId = ui->list_cat->currentData().toInt();
    QString warnCatId = QString::number(warn_catId);

    comWarn = new common_warn();

    warnType = comWarn->listBoxIsEmpty(warnType);
    warnDayType = comWarn->listBoxIsEmpty(warnDayType);

    if(!(preCheckUserInput(warnName, warnPoint, warnType))){
        return;
    }

    QString passData = warnName + "|||||" + warnPoint + "|||||" + warnType + "|||||" + warnDayType + "|||||" + warnCatId;

    gen = new general();
    int userInputCheck = gen->checkUserInput("warnline.txt", passData, this->selectedId); //check user input

    switch(userInputCheck){
        case 1:
            QMessageBox::warning(this,"unsuccessful",warnName+" is already exist");
            ui->txt_name->selectAll();
            ui->txt_name->setFocus();
            break;
        case 2:
            QMessageBox::critical(this,"unsuccessful","Couldn't open the file");
            break;
        case 3: //user input is empty
            QMessageBox::warning(this,"unsuccessful","Please enter a name");
            ui->txt_name->setFocus();
            break;
        case 4:
            QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
            ui->txt_name->selectAll();
            ui->txt_name->setFocus();
            break;
        default:
            editWarnLine(passData, this->selectedId); //edit the record on text file
            this->close();
            break;
    }
}

void edit_warn::on_btn_cancel_clicked()
{
    this->close();
}

void edit_warn::on_list_warnType_currentIndexChanged(const QString &arg1)
{
    showHideDayType(arg1);
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

bool edit_warn::retrieveCategory()
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

void edit_warn::showListing(QString catId, QString catName)
{
    ui->list_cat->addItem(catName, catId);
}

bool edit_warn::preCheckUserInput(QString warnName, QString warnPoint, QString warnType)
{
    comWarn = new common_warn();
    int returnInt = comWarn->preCheckUserInput(warnName, warnPoint, warnType);

    switch(returnInt){
     case -1:
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();

        return false;
        break;
    case -2:
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_point->selectAll();
        ui->txt_point->setFocus();

        return false;
        break;
    case -3:
        QMessageBox::warning(this,"unsuccessful","Please enter the warning milestone");
        ui->txt_point->setFocus();

        return false;
        break;
    case -4:
        QMessageBox::warning(this,"unsuccessful","Please enter only digits for warning point");
        ui->txt_point->selectAll();
        ui->txt_point->setFocus();

        return false;
        break;
    case -5:
        QMessageBox::warning(this,"unsuccessful","Please select a warning type");
        ui->list_warnType->setFocus();

        return false;
        break;
    default:
        return true;
        break;
    }
}

void edit_warn::editWarnLine(QString passData, QString curId)
{
    QString filename = "warnline";

    gen = new general();
    bool returnbool = gen->editRecord(filename, passData, curId);

    if(returnbool){
        QMessageBox::information(this,"Sucessful","Sucessfully editted the record");
    }
    else{
        QMessageBox::critical(this,"unsuccessful","Error occur while trying to edit the record");
    }
}
