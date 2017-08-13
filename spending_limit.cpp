#include "spending_limit.h"
#include <QFile>
#include <QTextStream>
#include <QFile>
#include <QDate>

QStringList spending_limit::retrieveWarnExp(QString filename)
{
    //open file
    QFile file(filename);

    QString line;

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
       linesList << line;
    }
    file.close();

    return linesList;
}

QStringList spending_limit::checkWarnLine()
{
    QStringList returnString;

    double total_in_day = 0;
    double total_in_day_cat = 0;
    double total_in_month = 0;
    double total_in_month_cat = 0;
    double total_in_year = 0;
    double total_in_year_cat = 0;
    double total_in_life = 0;
    double total_in_life_cat = 0;

    QStringList expList = retrieveWarnExp("expenses.txt");
    for(int i=0; i<expList.size(); i++){

        QStringList expRecord = expList[i].split("|||||");

        double expAmt = expRecord[2].toDouble();
        QString expCatId = expRecord[3];
        QString expDate = expRecord[5];

        QDate expDateD = QDate::fromString(expDate, "yyyyMMdd");
        QDate newDateD = QDate::fromString(this->new_date, "yyyyMMdd");
        QString expMonth = expDateD.toString("yyyyMM");
        QString newMonth = newDateD.toString("yyyyMM");
        QString expYear = expDateD.toString("yyyy");
        QString newYear = newDateD.toString("yyyy");

        if(expDate == this->new_date){
            total_in_day += expAmt;
        }
        if((expCatId == this->new_category || this->new_category == "") && (expDate == this->new_date)){
            total_in_day_cat += expAmt;
        }
        if(expMonth == newMonth){
            total_in_month += expAmt;
        }
        if((expCatId == this->new_category || this->new_category == "") && (expMonth == newMonth)){
            total_in_month_cat += expAmt;
        }
        if(expYear == newYear){
            total_in_year += expAmt;
        }
        if((expCatId == this->new_category || this->new_category == "") && (expYear == newYear)){
            total_in_year_cat += expAmt;
        }
        if((expCatId == this->new_category || this->new_category == "")){
            total_in_life_cat += expAmt;
        }

        total_in_life += expAmt;

    }

    QStringList warnList = retrieveWarnExp("warnline.txt");
    for(int i=0; i<warnList.size(); i++){

        QStringList warnRecord = warnList[i].split("|||||");

        QString warnName = warnRecord[1];
        double warnAmt = warnRecord[2].toDouble();
        QString warnType = warnRecord[3];
        QString warnCatId = warnRecord[4];

        if(total_in_life >= warnAmt && warnType == "Lifetime"
                && warnCatId == "0"){

            double exceed = total_in_life - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) + ") you set";
        }

        if(total_in_life_cat >= warnAmt && warnType == "Lifetime"
                && warnCatId == this->new_category && this->new_category != "0"){

            double exceed = total_in_life_cat - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) +
                            ") you set for the category - " + comGen->convertIdName("category.txt", warnCatId);
        }

        if(total_in_year >= warnAmt && warnType == "Year" && warnCatId == "0"){
            double exceed = total_in_year - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) + ") you set for a year";
        }

        if(total_in_year_cat >= warnAmt && warnType == "Year"
                && warnCatId == this->new_category && this->new_category != "0"){

            double exceed = total_in_year_cat - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) +
                            ") you set for a year for the category - " + comGen->convertIdName("category.txt", warnCatId);
        }

        if(total_in_month >= warnAmt && warnType == "Month" && warnCatId == "0"){
            double exceed = total_in_month - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) + ") you set for a month";
        }

        if(total_in_month_cat >= warnAmt && warnType == "Month"
                && warnCatId == this->new_category && this->new_category != "0"){

            double exceed = total_in_month_cat - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) +
                            ") you set for a month for the category - " + comGen->convertIdName("category.txt", warnCatId);
        }

        if(total_in_day >= warnAmt && warnType == "Day" && warnCatId == "0"){
            double exceed = total_in_day - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) + ") you set for a day";
        }

        if(total_in_day_cat >= warnAmt && warnType == "Day"
                && warnCatId == this->new_category && this->new_category != "0"){

            double exceed = total_in_day_cat - warnAmt;
            returnString << "Warning! " + warnName +
                            "\nYou have spended RM" + QString::number(exceed, 'f', 2) +
                            " more than the limit (RM" + QString::number(warnAmt, 'f', 2) +
                            ") you set for a day for the category - " + comGen->convertIdName("category.txt", warnCatId);
        }

    }

    return returnString;
}

spending_limit::spending_limit(QString catId, QString date)
{
    comGen = new common_general();
    this->new_category = catId;
    this->new_date = date;
}

spending_limit::~spending_limit()
{
    delete comGen;
}
