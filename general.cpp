#include "general.h"
#include <QFile>
#include <QDebug>

bool general::checkInvalidString(QString userinput){

    if(userinput.contains("|||", Qt::CaseInsensitive)){
        return true;
    }
    else{
        return false;
    }

}

int general::checkDataExist(QString userinput, QString filename){

    //open file
    QFile file(filename);

    //check if file exists
    if(!file.exists()){
        return 0;
    }

    //open file for read only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        //show unsucessful alert box
        return 2; //couldn't open the file
    }

    //read the file line by line
    QTextStream in(&file);
    QString line;

    userinput = userinput.toUpper();

    while (!in.atEnd()) {
       line = in.readLine();
       line = line.toUpper();

       if(line == userinput){
           file.close();
           return 1; //data already exists
       }
    }
    file.close();
    return 0; //no error
}
