#ifndef COMMON_GENERAL_H
#define COMMON_GENERAL_H

#include <QString>
#include <QStringList>

class common_general
{
public:
    common_general();

    void setFileName(QString);
    QString getFileName();
    void setTmpFileName(QString);
    QString getTmpFileName();

    bool writeNewRecord(QStringList, QString); //write new record into file
    bool checkNameExist(QString, QString); //check if name already exists
    void writeIntoFile(QStringList userInput); //write user input data into file
    int generateNewId(); //generate new id to add into file
    QStringList retrieveRecords(QString, QString); //retrieve records from textfile
    QString listBoxIsEmpty(QString); //return "" if listbox is empty
    bool DayTypeString(QString); //return false if type selection is not day/daily
    QString convertIdName(QString, QString); //convert id to name
    QString retrieveEditRecord(QString, QString); //retrieve recods for edit windows
    void editIntoFile(QStringList, QString); //write user input data into file
    void deleteRecord(QString); //delete the user chosen record

private:
    QString fileName;
    QString fileNameTmp;

};

#endif // COMMON_GENERAL_H
