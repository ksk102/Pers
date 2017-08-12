#include "add_expense.h"
#include "ui_add_expense.h"
#include <QDate>
#include <QTime>
#include <QMessageBox>

add_expense::add_expense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_expense)
{
    ui->setupUi(this);

    comGen = new common_general();
    comExp = new common_expenses();
    comGen->setFileName("expenses.txt");

    retrieveCategory(); //retrieve category from text file

    initialDateSelection(); //get the date selection in dropdown list
    initialTimeSelection(); //get the time selection in dropdown list
}

add_expense::~add_expense()
{
    delete ui;
    delete comGen;
    delete comExp;
}

void add_expense::retrieveCategory()
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

void add_expense::initialDateSelection()
{
    //current date
    QDate date = QDate::currentDate();
    int year = date.toString("yyyy").toInt();
    QString month = date.toString("MM");
    QString dayS = date.toString("dd");

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

    //set current date
    QString yearS = QString::number(year);

    ui->lst_year->setCurrentText(yearS);
    ui->lst_month->setCurrentText(month);
    ui->lst_day->setCurrentText(dayS);
}

void add_expense::initialTimeSelection()
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

    //set current time
    QTime currentTime = QTime::currentTime();
    QString hours = currentTime.toString("hh|||||AP");
    QString minutes = currentTime.toString("mm");
    QString AP = currentTime.toString("AP");

    QStringList hoursList = hours.split("|||||");
    hours = hoursList[0];

    ui->lst_hour->setCurrentText(hours);
    ui->lst_minute->setCurrentText(minutes);
    ui->lst_AP->setCurrentText(AP);
}

void add_expense::identifyDayinMonth(int year, QString month)
{
    ui->lst_day->clear();
    int day_in_month = comExp->identifyDayinMonth(year, month);

    for(int i=1;i<=day_in_month;i++){
        QString iDay = QString::number(i);
        if(i<10){
            iDay = "0" + iDay;
        }
        ui->lst_day->addItem(iDay,iDay);
    }
}

bool add_expense::preCheckUserInput(QString name, QString amount, QString other)
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

void add_expense::on_btn_confirm_clicked()
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
    //-1 parameter here is to by editting function,
    //since this is add new,
    //so use -1 to indicate it is not from edit
    if(comGen->writeNewRecord(passData, "-1")){
        QMessageBox::information(this,"Sucessful","\""+name+"\" has been sucessfully added into system.");
        this->close();
    }
    else{
        QMessageBox::warning(this,"Unsuccessful",name+" is already exist");
        ui->txt_name->selectAll();
        ui->txt_name->setFocus();
    }
}

void add_expense::on_btn_cancel_clicked()
{
    this->close();
}

void add_expense::on_lst_year_currentIndexChanged(const QString &arg1)
{
    int year = arg1.toInt();
    identifyDayinMonth(year, ui->lst_month->currentText());
}

void add_expense::on_lst_month_currentIndexChanged(const QString &arg1)
{
    int year = ui->lst_year->currentText().toInt();
    identifyDayinMonth(year, arg1);
}
