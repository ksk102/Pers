#include "common_warn.h"

QString common_warn::listBoxIsEmpty(QString lstboxText)
{
    if(lstboxText == "<Select one>"){
        return "";
    }
    else{
        return lstboxText;
    }
}

int common_warn::preCheckUserInput(QString warnName, QString warnPoint, QString warnType)
{
    bool validate;
    double dou_warnPoint = warnPoint.toDouble(&validate);

    if(warnName.contains("|||", Qt::CaseInsensitive)){
        return -1; //Name contain Invalid symbol
    }
    else if(warnPoint.contains("|||", Qt::CaseInsensitive)){
        return -2; //Point contain Invalid symbol
    }
    else if(warnPoint == ""){
        return -3; //Point is empty
    }
    else if(!validate){
        return -4; //Point is not number
    }
    else if(warnType == ""){
        return -5; //Type is empty
    }

    return 1;
}
