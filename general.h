#ifndef GENERAL_H
#define GENERAL_H

#include<QString>

class general{
public:
    //methods
    bool checkInvalidString(QString); //check if the userinput contain "|||"
    int checkUserInput(QString, QString, int); //check the user input before add it into system
    int checkDataExist(QString, QString, int); //check if user input already exists
    bool deleteRecord(QString, int); //delete the user chosen record
};

#endif // GENERAL_H
