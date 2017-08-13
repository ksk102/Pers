#ifndef COMMON_EXPENSES_H
#define COMMON_EXPENSES_H

#include <QString>

class common_expenses
{
public:
    common_expenses();
    int preCheckUserInput(QString, QString, QString);
    int identifyDayinMonth(int, QString);
    QString convertDate(QString); //convert from yyyyMMdd to dd/MM/yyyy
    QString convertTime(QString); //convert from hhmmAP to hh:mm AP
};

#endif // COMMON_EXPENSES_H
