#include "common_warn.h"

int common_warn::preCheckUserInput(QString name, QString amount, QString type)
{
    bool validate;
    double dou_amount = amount.toDouble(&validate);

    if(name == ""){
        return -1; //name is empty
    }
    else if(name.contains("|||", Qt::CaseInsensitive)){
        return -2; //name contain Invalid symbol
    }
    else if(amount.contains("|||", Qt::CaseInsensitive)){
        return -3; //amount contain Invalid symbol
    }
    else if(amount == ""){
        return -4; //amount is empty
    }
    else if(!validate){
        return -5; //amount is not number
    }
    else if(type == ""){
        return -6; //type is empty
    }

    return 1;
}
