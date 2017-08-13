#ifndef SPENDING_LIMIT_H
#define SPENDING_LIMIT_H

#include <QString>
#include <QStringList>
#include "common_general.h"

class spending_limit
{

private:
    common_general *comGen;

    QString new_category;
    QString new_date;

    QStringList retrieveWarnExp(QString); //retrieve warnline/expenses from file

public:
    spending_limit(QString, QString);
    ~spending_limit();
    QStringList checkWarnLine(); //check if exceed the spending limit
};

#endif // SPENDING_LIMIT_H
