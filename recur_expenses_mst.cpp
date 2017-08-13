#include "recur_expenses_mst.h"
#include "ui_recur_expenses_mst.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

recur_expenses_mst::recur_expenses_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recur_expenses_mst)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("recurring.txt");
    comGen->setTmpFileName("recurring_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    /*get searchString from userinputs*/
    setSearchString();

    setColumnInfo();

    //show the listing
    retrieveRecurList();
}

recur_expenses_mst::~recur_expenses_mst()
{
    delete ui;
    delete comGen;
}

void recur_expenses_mst::on_btn_add_clicked()
{
    addRecur = new add_recur(this);
    addRecur->setModal(true);
    addRecur->exec();

    retrieveRecurList();
}

void recur_expenses_mst::retrieveCategory()
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

void recur_expenses_mst::setColumnInfo()
{
    ui->tw_recurList->setColumnCount(6); //set the listing column number
    ui->tw_recurList->setHeaderLabels(QStringList() << "Recurring ID" << "Recurring Name" << "Recurring Amount (RM)" << "Category" << "Recurring Type" << "Recurring Day Type"); //set the columns header name

    ui->tw_recurList->setColumnWidth(0, 75);
    ui->tw_recurList->setColumnWidth(1, 250);
    ui->tw_recurList->setColumnWidth(2, 135);
    ui->tw_recurList->setColumnWidth(3, 100);
    ui->tw_recurList->setColumnWidth(4, 90);
    ui->tw_recurList->setColumnWidth(5, 120);

}

void recur_expenses_mst::retrieveRecurList()
{
    ui->tw_recurList->clear(); //clear the listing

    QStringList recordsList = retrieveRecords(); //call the method to get the records from text file

    QStringList recurRecord;
    QString recurId;
    QString recurName;
    QString recurAmt;
    QString recurType;
    QString recurDayType;
    QString recurCat;

    for(int i=0; i<recordsList.size(); i++){
        recurRecord = recordsList[i].split("|||||");
        recurId = recurRecord[0];
        recurName = recurRecord[1];
        recurAmt = recurRecord[2];
        recurCat = recurRecord[3];
        recurType = recurRecord[4];
        recurDayType = recurRecord[5];

        showListing(recurId, recurName, recurAmt, recurCat, recurType, recurDayType);
    }
}

QStringList recur_expenses_mst::retrieveRecords()
{
    QString filename = comGen->getFileName();

    QString recurName = searchString[0];
    double recurAmtFrom = searchString[1].toDouble();
    double recurAmtTo = searchString[2].toDouble();
    QString recurCat = searchString[3];
    QString recurType = searchString[4];

    //open file
    QFile file(filename);

    QString line;
    QString recordName;
    double recordAmt;
    QString recordCat;
    QString recordType;
    QString recordDayType;

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
       recordAmt = lineData[2].toDouble();
       recordCat = lineData[3];
       recordType = lineData[4];
       recordDayType = lineData[5];

       if((recurName == "" || recordName.contains(recurName, Qt::CaseInsensitive))
           && (recurAmtFrom == 0 || recurAmtFrom <= recordAmt)
           && (recurAmtTo == 0 || recurAmtTo >= recordAmt)
           && (recurCat == "0" || recurCat == recordCat)
           && (recurType == "" || recurType == recordType)){
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

void recur_expenses_mst::showListing(QString id, QString name, QString amt, QString cat, QString type, QString daytype)
{
    QString catName = "";

    if(cat != "0"){
        catName = comGen->convertIdName("category.txt", cat);
    }

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_recurList);

    itm->setText(0, id); //column, data
    itm->setText(1, name); //column, data
    itm->setText(2, amt); //column, data
    itm->setText(3, catName); //column, data
    itm->setText(4, type); //column, data
    itm->setText(5, daytype); //column, data
}

void recur_expenses_mst::setSearchString()
{
    QString recurType = ui->lst_recurType->currentText();

    recurType = comGen->listBoxIsEmpty(recurType);

    searchString = QStringList();

    searchString << ui->txt_name->text() << ui->txt_amout_from->text() << ui->txt_amout_to->text() <<  QString::number(ui->lst_cat->currentData().toInt()) << recurType;
}

void recur_expenses_mst::on_btn_edit_clicked()
{
    int currentRow = ui->tw_recurList->currentIndex().row(); //user selected row

    //if user click edit button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to edit");
        return;
    }

    QString currentId = ui->tw_recurList->currentItem()->text(0);

    editRecur = new edit_recur(this);
    editRecur->retrieveRecord(currentId);
    editRecur->setModal(true);
    editRecur->exec();

    retrieveRecurList(); //show the listing
}

void recur_expenses_mst::on_tw_recurList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_btn_edit_clicked();
}

void recur_expenses_mst::on_btn_delete_clicked()
{
    int currentRow = ui->tw_recurList->currentIndex().row(); //user selected row

    //if user click delete button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to delete");
        return;
    }
    QString currentId = ui->tw_recurList->currentItem()->text(0);

    //selected row's name
    QString currentText = ui->tw_recurList->currentItem()->text(1);
    int reply = QMessageBox::question(this,"Delete Record","Are you sure want to delete " +currentText+"?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){

        comGen->deleteRecord(currentId);
        retrieveRecurList();

    }
}

void recur_expenses_mst::on_btn_search_clicked()
{
    setSearchString();
    retrieveRecurList();
}
