#ifndef GENERAL_H
#define GENERAL_H

#include<QString>
#include<QStringList>

class general{
public:
    //methods
    bool checkInvalidString(QString); //check if the userinput contain "|||"
    int checkUserInput(QString, QString, QString); //check the user input before add it into system
    int checkDataExist(QString, QString, QString); //check if user input already exists
    bool deleteRecord(QString, QString); //delete the user chosen record
    int generateNewId(QString); //generate new id for records
    int writeNewRecord(QString, QString); //write new record into textfile
    QStringList retrieveRecords(QString, QString); //retrieve records from textfile
    QString retrieveEditRecord(QString, QString); //retrieve recods for edit windows
    bool editRecord(QString, QString, QString); //save the editted record into system
    QString convertIdName(QString, QString);
};

#endif // GENERAL_H
