#include "general.h"
#include <QFile>
#include <QTextStream>

bool general::checkInvalidString(QString userinput){

    if(userinput.contains("|||", Qt::CaseInsensitive)){
        return true;
    }
    else{
        return false;
    }

}

int general::checkUserInput(QString filename, QString userinput)
{
    int data_exists = checkDataExist(userinput,filename);

    if(userinput == ""){
        return 3;
    }
    else if(checkInvalidString(userinput)){
        return 4;
    }
    else if(data_exists != 0){
        return data_exists; // 1 for data already exists, 2 for coundn't open the file
    }
    else{
        return 0; //true
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

bool general::deleteRecord(QString filename, int recordno)
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

    QTextStream out(&file);
    QTextStream in(&file_tmp);
    QString line;

    int counter = 0;
    while (!out.atEnd()) {

       line = out.readLine();

       if(counter != recordno){
           in << line << endl;
       }
       counter++;

    }
    file.flush();
    file.close();
    file_tmp.close();

    QFile::remove(ori_filename);
    QFile::rename(tmp_filename,ori_filename);

    return true;
}
