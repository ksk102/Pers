#include "warnline_mst.h"
#include "ui_warnline_mst.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

warnLine_mst::warnLine_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warnLine_mst)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("warnline.txt");
    comGen->setTmpFileName("warnline_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    /*get searchString from userinputs*/
    setSearchString();

    setColumnInfo();

    //show the listing
    retrieveWarningList();
}

warnLine_mst::~warnLine_mst()
{
    delete ui;
    delete comGen;
}

void warnLine_mst::on_btn_add_clicked()
{
    addWarn = new add_warn(this);
    addWarn->setModal(true);
    addWarn->exec();

    retrieveWarningList(); //show the listing
}

void warnLine_mst::showListing(QString warnId, QString warnName, QString warnPoint, QString warnType, QString warnDayType, QString warnCat)
{
    QString warnCatName = "";

    if(warnCat != "0"){
        warnCatName = comGen->convertIdName("category.txt", warnCat);
    }


    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_warnList);

    itm->setText(0, warnId); //column, data
    itm->setText(1, warnName); //column, data
    itm->setText(2, warnPoint); //column, data
    itm->setText(3, warnType); //column, data
    itm->setText(4, warnDayType); //column, data
    itm->setText(5, warnCatName); //column, data
}

void warnLine_mst::retrieveWarningList()
{
    ui->tw_warnList->clear(); //clear the listing

    QStringList recordsList = retrieveRecords(); //call the method to get the records from text file

    QStringList warnRecord;
    QString warnId;
    QString warnName;
    QString warnPoint;
    QString warnType;
    QString warnDayType;
    QString warnCat;

    for(int i=0; i<recordsList.size(); i++){
        warnRecord = recordsList[i].split("|||||");
        warnId = warnRecord[0];
        warnName = warnRecord[1];
        warnPoint = warnRecord[2];
        warnType = warnRecord[3];
        warnDayType = warnRecord[4];
        warnCat = warnRecord[5];

        showListing(warnId, warnName, warnPoint, warnType, warnDayType, warnCat);
    }
}

void warnLine_mst::on_btn_edit_clicked()
{
    int currentRow = ui->tw_warnList->currentIndex().row(); //user selected row

    //if user click edit button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to edit");
        return;
    }

    QString currentId = ui->tw_warnList->currentItem()->text(0);

    editWarn = new edit_warn(this);
    editWarn->retrieveRecord(currentId);
    editWarn->setModal(true);
    editWarn->exec();

    retrieveWarningList(); //show the listing
}

void warnLine_mst::on_btn_delete_clicked()
{
    int currentRow = ui->tw_warnList->currentIndex().row(); //user selected row

    //if user click delete button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to delete");
        return;
    }
    QString currentId = ui->tw_warnList->currentItem()->text(0);

    //selected row's name
    QString currentText = ui->tw_warnList->currentItem()->text(1);
    int reply = QMessageBox::question(this,"Delete Record","Are you sure want to delete " +currentText+"?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){
        comGen->deleteRecord(currentId);
        retrieveWarningList();
    }
}

void warnLine_mst::retrieveCategory()
{
    //call the method to get the records from text file
    QStringList recordsList = comGen->retrieveRecords("category.txt", "");

    QStringList catRecord;
    QString catId;
    QString catName;

    for(int i=0; i<recordsList.size(); i++){
        catRecord = recordsList[i].split("|||||");
        catId = catRecord[0];
        catName = catRecord[1];

        ui->lst_cat->addItem(catName, catId);
    }
}

void warnLine_mst::setSearchString()
{
     QString warnType = ui->lst_warnType->currentText();

    warnType = comGen->listBoxIsEmpty(warnType);

    searchString = QStringList();

    searchString << ui->txt_name->text() <<ui->txt_pointFrom->text() << ui->txt_pointTo->text() << warnType << QString::number(ui->lst_cat->currentData().toInt());
}

void warnLine_mst::setColumnInfo()
{
    ui->tw_warnList->setColumnCount(6); //set the listing column number
    ui->tw_warnList->setHeaderLabels(QStringList() << "Warning ID" << "Warning Name" << "Warning Point (RM)" << "Warning Type" << "Warning Day Type" << "Category"); //set the columns header name

    ui->tw_warnList->setColumnWidth(0, 70);
    ui->tw_warnList->setColumnWidth(1, 270);
    ui->tw_warnList->setColumnWidth(2, 120);
    ui->tw_warnList->setColumnWidth(3, 90);
    ui->tw_warnList->setColumnWidth(4, 120);
    ui->tw_warnList->setColumnWidth(5, 100);
}

void warnLine_mst::on_btn_search_clicked()
{
    setSearchString();
    retrieveWarningList();
}

QStringList warnLine_mst::retrieveRecords()
{
    QString filename = comGen->getFileName();

    QString warnName = searchString[0];
    double warnPointFrom = searchString[1].toDouble();
    double warnPointTo = searchString[2].toDouble();
    QString warnType = searchString[3];
    QString warnCat = searchString[4];

    //open file
    QFile file(filename);

    QString line;
    QString recordName;
    double recordPoint;
    QString recordType;
    QString recordDayType;
    QString recordCat;
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
       recordPoint = lineData[2].toDouble();
       recordType = lineData[3];
       recordDayType = lineData[4];
       recordCat = lineData[5];

       if((warnName == "" || recordName.contains(warnName, Qt::CaseInsensitive))
           && (warnPointFrom == 0 || warnPointFrom <= recordPoint)
           && (warnPointTo == 0 || warnPointTo >= recordPoint)
           && (warnType == "" || warnType == recordType)
           && (warnCat == "0" || warnCat == recordCat)){
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

void warnLine_mst::on_tw_warnList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_btn_edit_clicked();
}
