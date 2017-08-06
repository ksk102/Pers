#include "general.h"
#include <QFile>
#include <QTextStream>

bool general::checkInvalidString(QString userInput){

    QStringList userInputs = userInput.split("|||||");

    foreach(QString data, userInputs){
        if(data.contains("|||", Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

int general::checkUserInput(QString filename, QString userInput, QString curId)
{
    QStringList userInputs = userInput.split("|||||");
    QString recordName = userInputs[0];

    int data_exists = checkDataExist(recordName,filename,curId);

    if(recordName == ""){
        return 3;
    }
    else if(checkInvalidString(userInput)){
        return 4;
    }
    else if(data_exists != 0){
        return data_exists; // 1 for data already exists, 2 for coundn't open the file
    }
    else{
        return 0; //true
    }
}

int general::checkDataExist(QString userinput, QString filename, QString curId){

    //open file
    QFile file(filename);

    //check if file exists
    if(!file.exists()){
        return 0;
    }

    //open file for read only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return 2; //couldn't open the file
    }

    //read the file line by line
    QTextStream in(&file);
    QString line;
    QStringList currentRecord;
    QString recordId;
    QString recordName;

    userinput = userinput.toUpper();

    while (!in.atEnd()) {
       line = in.readLine();

       currentRecord = line.split("|||||"); //split the data
       recordId = currentRecord[0];
       recordName = currentRecord[1];

       recordName = recordName.toUpper();

       //if the userinput is same as one of the record in the file
       //AND that record is not the field user currently editing
       if(recordName == userinput && curId != recordId){
           file.close();
           return 1; //data already exists
       }
    }
    file.close();
    return 0; //data doesn't exists
}

bool general::deleteRecord(QString filename, QString curId)
{
    QString ori_filename = filename + ".txt";
    QString tmp_filename = filename +"_tmp.txt";

    //open file
    QFile file(ori_filename);
    QFile file_tmp(tmp_filename);

    //open file for write only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }
    if(!file_tmp.open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }

    QTextStream in(&file);
    QTextStream out(&file_tmp);
    QString line;
    QStringList currentRecord;
    QString recordId;

    while (!in.atEnd()) {

       line = in.readLine();

        currentRecord = line.split("|||||"); //split the data
        recordId = currentRecord[0];

        if(recordId != curId){
            out << line << endl;
        }

    }

    file.close();
    file_tmp.flush();
    file_tmp.close();

    QFile::remove(ori_filename);
    QFile::rename(tmp_filename,ori_filename);

    return true;
}

int general::generateNewId(QString filename)
{
    QFile file(filename);

    if(file.exists()){
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            return -1;
        }

        QTextStream in(&file);
        QString line;

        while(!in.atEnd()){
            line = in.readLine(); //read until last record
        }
        QStringList lastRecord = line.split("|||||"); //split the data of last record

        int newId = lastRecord[0].toInt() + 1; //id of last record plus 1
        file.close();

        return newId;
    }

    return -2;
}

int general::writeNewRecord(QString filename, QString userInput)
{
    //open file
    QFile file(filename);

    int new_id = generateNewId(filename);

    if(new_id <= 0){
        switch(new_id){
            case -1: //couldn't open the file
                return -1;
            break;
            case -2: //file doesn't exists
                new_id = 1;
            break;
        }
    }

    //append new entry into file
    if(!file.open(QFile::Append | QFile::Text))
    {
        return -1;
    }

    QTextStream out(&file);
    QStringList userInputs = userInput.split("|||||");

    out << new_id;
    foreach(QString data, userInputs){
        out << "|||||" << data;
    }
    out << endl;

    file.flush();
    file.close();

    return 1;
}

QStringList general::retrieveRecords(QString filename, QString searchString)
{
    //open file
    QFile file(filename);

    QString line;
    QString recordName;
    QStringList lineData;
    QStringList linesList;

    //check if file exists
    if(!file.exists()){
        return linesList;
    }

    //open file for read only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return linesList;
    }

    //read the file line by line
    QTextStream in(&file);

    while (!in.atEnd()) {
       line = in.readLine();

       lineData = line.split("|||||");
       recordName = lineData[1];

       if(searchString != ""){
           if(recordName.contains(searchString, Qt::CaseInsensitive)){
               linesList << line;
           }
       }
       else{
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

QString general::retrieveEditRecord(QString filename, QString curId)
{
    //open file
    QFile file(filename);

    //open file for write only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return "|||||-1";
    }

    QTextStream in(&file);
    QString line;
    QStringList currentRecord;
    QString recordId;

    while (!in.atEnd()) {

       line = in.readLine();

       currentRecord = line.split("|||||"); //split the data
       recordId = currentRecord[0];

       if(curId == recordId){
           file.close();
           return line;
           break;
       }

    }

    file.close();
    return "|||||-2";
}

bool general::editRecord(QString filename, QString userInputData, QString curId)
{
    QString ori_filename = filename + ".txt";
    QString tmp_filename = filename +"_tmp.txt";

    //open file
    QFile file(ori_filename);
    QFile file_tmp(tmp_filename);

    //append new entry into file
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }
    if(!file_tmp.open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }

    QTextStream in(&file);
    QTextStream out(&file_tmp);
    QStringList userInputs = userInputData.split("|||||");

    QString line;
    QStringList currentRecord;
    QString recordId;

    while (!in.atEnd()) {

       line = in.readLine();

       currentRecord = line.split("|||||"); //split the data
       recordId = currentRecord[0];

       if(recordId != curId){
           out << line << endl;
       }
       else{

           out << recordId;
           foreach(QString data, userInputs){
               out << "|||||" << data;
           }
           out << endl;

       }

    }

    file.close();
    file_tmp.flush();
    file_tmp.close();

    QFile::remove(ori_filename);
    QFile::rename(tmp_filename,ori_filename);

    return true;
}

QString general::convertIdName(QString filename, QString Id)
{
    QString theRecordLine = retrieveEditRecord(filename, Id);

    QStringList theRecordLineList = theRecordLine.split("|||||");
    QString name = theRecordLineList[1];

    return name;
}
