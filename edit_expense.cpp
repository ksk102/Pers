#include "edit_expense.h"
#include "ui_edit_expense.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>

edit_expense::edit_expense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_expense)
{
    ui->setupUi(this);

    comGen = new common_general();
    comExp = new common_expenses();
    comGen->setFileName("expenses.txt");
    comGen->setTmpFileName("expenses_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    initialDateSelection(); //get the date selection in dropdown list
    initialTimeSelection(); //get the time selection in dropdown list
}

edit_expense::~edit_expense()
{
    delete ui;
    delete comGen;
    delete comExp;
    delete spendLimit;
}

void edit_expense::retrieveRecord(QString curId)
{
    this->selectedId = curId;

    QString returnString = comGen->retrieveEditRecord("", curId);
    QStringList currentRecord;

    if(returnString == "|||||-2"){ //unable to find the record
        QMessageBox::critical(this,"unsuccessful","Error occured while trying to get the record");
        return;
    }
    else{
        //split from file
        currentRecord = returnString.split("|||||"); //split the data

        QString expName = currentRecord[1];
        QString expAmt = currentRecord[2];
        QString expCat = currentRecord[3];
        QString expCatName = comGen->convertIdName("category.txt", expCat);
        QString expDesc = currentRecord[4];
        QString expDate = currentRecord[5];
        QString expTime = currentRecord[6];

        //convert to desired format
        QStringList dateList = convertDate(expDate);
        QStringList timeList = convertTime(expTime);

        QString day = dateList[0];
        QString month = dateList[1];
        QString year = dateList[2];

        QString hours = timeList[0];
        QString minutes = timeList[1];
        QString ap = timeList[2];

        //line spacing
        expDesc = expDesc.replace("<br>","\n");

        //write onto fields
        ui->txt_name->setText(expName);
        ui->txt_amount->setText(expAmt);
        ui->lst_cat->setCurrentText(expCatName);
        ui->txt_other->setPlainText(expDesc);

        //identify the day in a month
        int yearI = year.toInt();
        identifyDayinMonth(yearI, month);

        //set date and hour according to file
        ui->lst_month->setCurrentText(month);
        ui->lst_year->setCurrentText(year);
        ui->lst_day->setCurrentText(day);
        ui->lst_hour->setCurrentText(hours);
        ui->lst_minute->setCurrentText(minutes);
        ui->lst_AP->setCurrentText(ap);
    }
}

void edit_expense::retrieveCategory()
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

void edit_expense::initialDateSelection()
{
    //current date
    QDate date = QDate::currentDate();
    int year = date.toString("yyyy").toInt();
    QString month = date.toString("MM");

    //starting year in the selection
    int startYear = 2000;

    //identify the day in a month
    identifyDayinMonth(year, month);

    //set the date selection
    for(int i=startYear;i<=year;i++){
        QString iYear = QString::number(i);
        if(i<10){
            iYear = "0" + iYear;
        }
        ui->lst_year->addItem(iYear, iYear);
    }

    for(int i=1;i<=12;i++){
        QString iMonth = QString::number(i);
        if(i<10){
            iMonth = "0" + iMonth;
        }
        ui->lst_month->addItem(iMonth,iMonth);
    }
}

void edit_expense::initialTimeSelection()
{
    //time selection
    for(int i=1;i<=12;i++){
        QString iHour = QString::number(i);
        if(i<10){
            iHour = "0" + iHour;
        }
        ui->lst_hour->addItem(iHour,iHour);
    }
    for(int i=0;i<=59;i++){
        QString iMinute = QString::number(i);
        if(i<10){
            iMinute = "0" + iMinute;
        }
        ui->lst_minute->addItem(iMinute,iMinute);
    }

    ui->lst_AP->addItem("AM","AM");
    ui->lst_AP->addItem("PM","PM");
}

void edit_expense::identifyDayinMonth(int year, QString month)
{
    ui->lst_day->clear();
    int day_in_month_from = comExp->identifyDayinMonth(year, month);
    for(int i=1;i<=day_in_month_from;i++){
        QString iDay = QString::number(i);
        if(i<10){
            iDay = "0" + iDay;
        }
        ui->lst_day->addItem(iDay,iDay);
    }
}

QStringList edit_expense::convertDate(QString date)
{
    QDate dateD = QDate::fromString(date, "yyyyMMdd");
    QString day = dateD.toString("dd");
    QString month = dateD.toString("MM");
    QString year = dateD.toString("yyyy");

    QStringList returnString;
    returnString << day << month << year;

    return returnString;
}

QStringList edit_expense::convertTime(QString time)
{
    QTime timeT = QTime::fromString(time, "hhmmAP");
    QString hours = timeT.toString("hh|||||AP");
    QString minutes = timeT.toString("mm");
    QString ap = timeT.toString("AP");

    QStringList hoursList = hours.split("|||||");
    hours = hoursList[0];

    QStringList returnString;
    returnString << hours << minutes << ap;

    return returnString;
}

void edit_expense::on_lst_month_currentIndexChanged(const QString &arg1)
{
    int year = ui->lst_year->currentText().toInt();
    identifyDayinMonth(year, arg1);
}

void edit_expense::on_lst_year_currentIndexChanged(const QString &arg1)
{
    int year = arg1.toInt();
    identifyDayinMonth(year, ui->lst_month->currentText());
}

bool edit_expense::preCheckUserInput(QString name, QString amount, QString other)
{
    int returnInt = comExp->preCheckUserInput(name, amount, other);

    //if returnInt is
    // -1, name is empty
    // -2, name contain Invalid symbol
    // -3, amount contain Invalid symbol
    // -4, amount is empty
    // -5, amount is not number
    // -6, description contain Invalid symbol
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
        QMessageBox::warning(this,"unsuccessful","Please enter the amount");
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
        QMessageBox::warning(this,"unsuccessful","Invalid symbol \"|||\"");
        ui->txt_other->selectAll();
        ui->txt_other->setFocus();

        return false;
        break;
    default:
        return true;
        break;
    }
}

void edit_expense::on_btn_confirm_clicked()
{
    //get user input
    QString date = ui->lst_year->currentText() + ui->lst_month->currentText() + ui->lst_day->currentText();
    QString time = ui->lst_hour->currentText() + ui->lst_minute->currentText() + ui->lst_AP->currentText();
    QString name = ui->txt_name->text();
    QString amt = ui->txt_amount->text();
    int catId = ui->lst_cat->currentData().toInt();
    QString catIdS = QString::number(catId);
    QString other = ui->txt_other->toPlainText();

    other = other.replace("\n", "<br>");

    //check user input
    if(!(preCheckUserInput(name, amt, other))){
        return;
    }

    //data to save into new file
    QStringList passData;
    passData << name << amt << catIdS << other << date << time;

    //write new record into file
    if(comGen->writeNewRecord(passData, this->selectedId, false)){
        QMessageBox::information(this,"Sucessful","\""+name+"\" has been sucessfully added into system.");

        //warnline function
        spendLimit = new spending_limit(catIdS, date);
        QStringList overspendList = spendLimit->checkWarnLine();

        foreach(QString data, overspendList){
            QMessageBox::warning(this,"Warning", data);
        }

        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",name+" is already exist");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();
    }
}

void edit_expense::on_btn_cancel_clicked()
{
    this->close();
}
