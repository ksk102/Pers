#ifndef RECURRING_EXPENSES_H
#define RECURRING_EXPENSES_H

#include <QStringList>
#include "common_general.h"
#include "common_expenses.h"

class recurring_expenses
{
public:
    recurring_expenses();
    ~recurring_expenses();

    common_general *comGen;
    common_expenses *comExp;

private:
    void recordLoginDate();
    void writeLoginDate(QString date); //write login date into file
    void addRecurringExpenses(); // check and add recurring expenses into system
    bool isFirstTimeToday(); //compare today's date with last login date
    QString getlastDate(); //get last login date from file
    QStringList lastLoginDate(); //get last login date
    QStringList todayDate(); //get today's date
    QStringList retrieveRecurExpenses(QString); //get recurring expenses from file
    void addDailyRecurExpenses(int, int, int, int, int, int); //add daily recurring expenses
    void addDailyRecurExpensesSubMonth(QString, QString, QString, int, int, int, int, int, int, int, int, QString); //sub function for addDailyRecurExpenses
    void addDailyRecurExpensesSubMonthSubDay(QString, QString, QString, QString); //sub function for addDailyRecurExpensesSubMonth
    void identifyDayinMonth(int, QString); //identify the day in a month
};

#endif // RECURRING_EXPENSES_H
