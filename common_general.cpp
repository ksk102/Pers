#include "common_general.h"
#include <QFile>
#include <QTextStream>

common_general::common_general()
{

}

void common_general::setFileName(QString filename){
    this->fileName = filename;
}

QString common_general::getFileName(){
    return this->fileName;
}

void common_general::setTmpFileName(QString filename)
{
    this->fileNameTmp = filename;
}

QString common_general::getTmpFileName()
{
    return this->fileNameTmp;
}

bool common_general::writeNewRecord(QStringList userInput, QString curId)
{
    QString recordName = userInput[0];

    //check if the name already exists, doesn't exists if return true
    if(checkNameExist(recordName,curId)){

        if(curId == "-1"){
            writeIntoFile(userInput);
        }
        else{
            editIntoFile(userInput, curId);
        }

        return true;
    }
    else{
        return false;
    }
}

bool common_general::checkNameExist(QString userinput, QString curId)
{

    //open file
    QFile file(this->getFileName());

    //check if file exists
    if(!file.exists()){
        return true;
    }

    //open file for read only
    file.open(QFile::ReadOnly | QFile::Text);


    QTextStream in(&file);
    QString line;
    QStringList currentRecord;
    QString recordId;
    QString recordName;

    userinput = userinput.toUpper();

    //read the file line by line
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
           return false; //name already exists
       }
    }
    file.close();
    return true; //name doesn't exists
}

void common_general::writeIntoFile(QStringList userInput)
{
    //open file
    QFile file(this->getFileName());
    file.open(QFile::Append | QFile::Text);

    int new_id = generateNewId();

    QTextStream out(&file);

    out << new_id;
    foreach(QString data, userInput){
        out << "|||||" << data;
    }
    out << endl;

    file.flush();
    file.close();
}

int common_general::generateNewId()
{
    QFile file(this->getFileName());

    if(file.exists()){

        file.open(QFile::ReadOnly | QFile::Text);

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
    else{
        return 1;
    }
}

QStringList common_general::retrieveRecords(QString filename, QString searchString)
{
    if(filename == ""){
        filename = this->getFileName();
    }

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
    file.open(QFile::ReadOnly | QFile::Text);

    //read the file line by line
    QTextStream in(&file);

    while (!in.atEnd()) {
       line = in.readLine();

       lineData = line.split("|||||");
       recordName = lineData[1];

       if(searchString == "" || (recordName.contains(searchString, Qt::CaseInsensitive))){
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

QString common_general::listBoxIsEmpty(QString lstboxText)
{
    if(lstboxText == "<Select one>"){
        return "";
    }
    else{
        return lstboxText;
    }
}

bool common_general::DayTypeString(QString lstboxType)
{
    if(lstboxType == "Daily" || lstboxType == "Day"){
        return true;
    }
    else{
        return false;
    }
}

QString common_general::convertIdName(QString filename, QString Id)
{
    QString theRecordLine = retrieveEditRecord(filename, Id);

    QStringList theRecordLineList = theRecordLine.split("|||||");
    QString name = theRecordLineList[1];

    return name;
}

QString common_general::retrieveEditRecord(QString filename, QString curId)
{
    if(filename == ""){
        filename = this->getFileName();
    }

    //open file
    QFile file(filename);

    //open file for write only
    file.open(QFile::ReadOnly | QFile::Text);

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

void common_general::editIntoFile(QStringList userInput, QString curId)
{
    QString ori_filename = this->getFileName();
    QString tmp_filename = this->getTmpFileName();

    //open file
    QFile file(ori_filename);
    QFile file_tmp(tmp_filename);

    //append new entry into file
    file.open(QFile::ReadOnly | QFile::Text);
    file_tmp.open(QFile::WriteOnly | QFile::Text);

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
       else{

           out << recordId;
           foreach(QString data, userInput){
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
}

void common_general::deleteRecord(QString curId)
{
    QString ori_filename = this->getFileName();
    QString tmp_filename = this->getTmpFileName();

    //open file
    QFile file(ori_filename);
    QFile file_tmp(tmp_filename);

    //open file for write only
    file.open(QFile::ReadOnly | QFile::Text);
    file_tmp.open(QFile::WriteOnly | QFile::Text);

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
}
