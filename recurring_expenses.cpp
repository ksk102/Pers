#include "recurring_expenses.h"
#include <QDate>
#include <QFile>
#include <QTextStream>

recurring_expenses::recurring_expenses()
{
    comGen = new common_general();
    comGen->setFileName("lastlogin.txt");

    addRecurringExpenses(); //add recurring expenses into file
    recordLoginDate(); //record down the login date
}

recurring_expenses::~recurring_expenses()
{
    delete comGen;
}

void recurring_expenses::recordLoginDate()
{
    QDate today = QDate::currentDate();
    QString todayS = today.toString("yyyyMMdd");

    writeLoginDate(todayS); //write into file
}

void recurring_expenses::writeLoginDate(QString date)
{
    //open file
    QFile file(comGen->getFileName());
    file.open(QFile::WriteOnly | QFile::Text);

    QTextStream out(&file);
    out << date << endl;

    file.flush();
    file.close();
}

void recurring_expenses::addRecurringExpenses()
{
    if(isFirstTimeToday()){ //is today the user first time login?

        QStringList lastDate = lastLoginDate(); //get last login date
        int lastday = lastDate[0].toInt();
        int lastmonth = lastDate[1].toInt();
        int lastyear = lastDate[2].toInt();

        QStringList todaydate = todayDate(); //get today's date
        int todayday = todaydate[0].toInt();
        int todaymonth = todaydate[1].toInt();
        int todayyear = todaydate[2].toInt();

        //add daily recurring expenses
        addDailyRecurExpenses(lastday, lastmonth, lastyear, todayday, todaymonth, todayyear);
    }
}

bool recurring_expenses::isFirstTimeToday()
{
    QString date = getlastDate(); //get last login date

    QDate today = QDate::currentDate();
    QString todayS = today.toString("yyyyMMdd");

    if(date != todayS){
        return true;
    }
    else{
        return false;
    }
}

QString recurring_expenses::getlastDate()
{
    QString date;

    //open file
    QFile file(comGen->getFileName());
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream in(&file);
    in >> date;

    file.close();

    return date;
}

QStringList recurring_expenses::lastLoginDate()
{
     QString lastdate = getlastDate();
     QDate lastdateD = QDate::fromString(lastdate, "yyyyMMdd");

     QString lastday = lastdateD.toString("dd");
     QString lastmonth = lastdateD.toString("MM");
     QString lastyear = lastdateD.toString("yyyy");

     QStringList lastlogindate;
     lastlogindate << lastday << lastmonth << lastyear;

     return lastlogindate;
}

QStringList recurring_expenses::todayDate()
{
    QDate today = QDate::currentDate();

    QString day = today.toString("dd");
    QString month = today.toString("MM");
    QString year = today.toString("yyyy");

    QStringList todaydate;
    todaydate << day << month << year;

    return todaydate;
}

//get recur expenses from file
QStringList recurring_expenses::retrieveRecurExpenses(QString searchString)
{
    QString filename = "recurring.txt";

    QString recurType = searchString;

    //open file
    QFile file(filename);

    QString line;
    QString recordType;

    QStringList lineData;
    QStringList linesList;

    //check if file exists
    if(!file.exists()){
        return linesList;
    }

    //open file for read only
    file.open(QFile::ReadOnly | QFile::Text);

    //read the file line by line
    QTextStream in(&file);

    while (!in.atEnd()) {
       line = in.readLine();

       lineData = line.split("|||||");
       recordType = lineData[4];

       if(recurType == "" || recurType == recordType){
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

void recurring_expenses::addDailyRecurExpenses(int lastday,int lastmonth, int lastyear, int todayday, int todaymonth, int todayyear)
{
    QStringList recordsList = retrieveRecurExpenses("Daily");

    //get each recurring expenses' data
    for(int i=0; i<recordsList.size(); i++){
        QStringList recurRecord = recordsList[i].split("|||||");
        QString recurName = recurRecord[1];
        QString recurAmt = recurRecord[2];
        QString recurCat = recurRecord[3];
        QString recurDayType = recurRecord[5];

        //loop from last login year till this year
        for(int y=lastyear;y<=todayyear;y++){

            //if last login year equal to current loop's year, start the month from last login month
            if(lastyear == y){
                for(int m=lastmonth;m<=12;m++){
                    addDailyRecurExpensesSubMonth(recurName, recurAmt, recurCat,lastmonth, todaymonth,
                                                  lastday, todayday, lastyear, todayyear, y, m, recurDayType);

                    //if this month and year equal to looping's month and year, end the loop
                    if(todaymonth == m && y == todayyear){
                        break;
                    }
                }
            }
            else{
                for(int m=1;m<=12;m++){
                    addDailyRecurExpensesSubMonth(recurName, recurAmt, recurCat,lastmonth, todaymonth,
                                                  lastday, todayday, lastyear, todayyear, y, m, recurDayType);

                    if(todaymonth == m && y == todayyear){
                        break;
                    }
                }
            }
        }
    }
}

void recurring_expenses::addDailyRecurExpensesSubMonth(QString recurName, QString recurAmt, QString recurCat, int lastmonth, int todaymonth, int lastday, int todayday, int lastyear, int todayyear, int y, int m, QString recurDayType)
{
    //add an 0 in front of the months less than 10
    QString runningMonth = (m<10) ? "0"+QString::number(m) : QString::number(m);

    int day_in_month = comExp->identifyDayinMonth(y, runningMonth);

    //if last login year and month equal to current looping's year and month,
    //start the day from last login day
    if(lastyear == y && lastmonth == m){
        for(int d=lastday+1;d<=day_in_month;d++){

            //regenerate date format
            QString runningday = (d<10) ? "0"+QString::number(d) : QString::number(d);
            QString runningMonth = (m<10) ? "0"+QString::number(m) : QString::number(m);
            QString runningdate = QString::number(y) + runningMonth + runningday;
            QDate loopingDate = QDate::fromString(runningdate,"yyyyMMdd");

            if(recurDayType == "Every Weekday"){
                if(loopingDate.dayOfWeek() != 6 && loopingDate.dayOfWeek() != 7){
                    addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
                }
            }
            else if(recurDayType == "Every Weekend"){
                if(loopingDate.dayOfWeek() == 6 || loopingDate.dayOfWeek() == 7){
                    addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
                }
            }
            else{
                addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
            }

            //if this day, month and year equal to looping's day, month and year, end the loop
            if(todaymonth == m && d == todayday && y == todayyear){
                break;
            }
        }
    }
    else{
        for(int d=1;d<=day_in_month;d++){

            //regenerate date format
            QString runningday = (d<10) ? "0"+QString::number(d) : QString::number(d);
            QString runningMonth = (m<10) ? "0"+QString::number(m) : QString::number(m);
            QString runningdate = QString::number(y) + runningMonth + runningday;
            QDate loopingDate = QDate::fromString(runningdate,"yyyyMMdd");

            if(recurDayType == "Every Weekday"){
                if(loopingDate.dayOfWeek() != 6 && loopingDate.dayOfWeek() != 7){
                    addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
                }
            }
            else if(recurDayType == "Every Weekend"){
                if(loopingDate.dayOfWeek() == 6 || loopingDate.dayOfWeek() == 7){
                    addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
                }
            }
            else{
                addDailyRecurExpensesSubMonthSubDay(runningdate, recurName, recurAmt, recurCat);
            }

            if(todaymonth == m && d == todayday && y == todayyear){
                break;
            }
        }
    }
}

void recurring_expenses::addDailyRecurExpensesSubMonthSubDay(QString runningdate, QString recurName,
                                                             QString recurAmt, QString recurCat)
{
    QStringList passData;

    //data to write into file
    passData << recurName + " @ " +  runningdate << recurAmt << recurCat << "" << runningdate << "1200AM";

    //write new record into file
    comGen->writeIntoFile(passData, "expenses.txt");
}
