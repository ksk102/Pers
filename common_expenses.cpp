#include "common_expenses.h"
#include <QDate>
#include <QTime>

common_expenses::common_expenses()
{

}

int common_expenses::preCheckUserInput(QString name, QString amt, QString other)
{
    bool validate;
    double dou_amount = amt.toDouble(&validate);

    if(name == ""){
        return -1; //name is empty
    }
    else if(name.contains("|||", Qt::CaseInsensitive)){
        return -2; //name contain Invalid symbol
    }
    else if(amt.contains("|||", Qt::CaseInsensitive)){
        return -3; //amount contain Invalid symbol
    }
    else if(amt == ""){
        return -4; //amount is empty
    }
    else if(!validate){
        return -5; //amount is not number
    }
    else if(other.contains("|||", Qt::CaseInsensitive)){
        return -6; //description contain Invalid symbol
    }

    return 1;
}

int common_expenses::identifyDayinMonth(int year, QString month)
{
    int day_in_month = 0;

    if(month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12"){
            day_in_month = 31;
    }
    else if(month == "02"){
        day_in_month = 28;

        if(year % 4 == 0){
            day_in_month = 29;
        }
    }
    else{
        day_in_month = 30;
    }

    return day_in_month;
}

QString common_expenses::convertDate(QString date)
{
    QDate dateD = QDate::fromString(date, "yyyyMMdd");
    QString dateS = dateD.toString("dd/MM/yyyy");

    return dateS;
}

QString common_expenses::convertTime(QString time)
{
    QTime timeT = QTime::fromString(time, "hhmmAP");
    QString timeS = timeT.toString("hh:mm AP");

    return timeS;
}
