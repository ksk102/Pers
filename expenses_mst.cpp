#include "expenses_mst.h"
#include "ui_expenses_mst.h"
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QMessageBox>

expenses_mst::expenses_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::expenses_mst)
{
    ui->setupUi(this);

    comGen = new common_general();
    comExp = new common_expenses();
    comGen->setFileName("expenses.txt");
    comGen->setTmpFileName("expenses_tmp.txt");

    retrieveCategory(); //retrieve category from text file

    /*get searchString from userinputs*/
    setSearchString();

    setColumnInfo();

    initialDateSelection();

    //show the listing
    retrieveExpensesList();
}

expenses_mst::~expenses_mst()
{
    delete ui;
    delete comGen;
    delete comExp;
}

void expenses_mst::on_btn_add_clicked()
{
    addExp = new add_expense(this);
    addExp->setModal(true);
    addExp->exec();

    retrieveExpensesList();
}

void expenses_mst::retrieveCategory()
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

void expenses_mst::setSearchString()
{
    searchString = QStringList();

    QString date_from = ui->lst_year_from->currentText() + ui->lst_month_from->currentText() + ui->lst_day_from->currentText();
    QString date_to = ui->lst_year_to->currentText() + ui->lst_month_to->currentText() + ui->lst_day_to->currentText();

    searchString << ui->txt_name->text() << ui->txt_amout_from->text() << ui->txt_amout_to->text() <<  QString::number(ui->lst_cat->currentData().toInt()) << date_from << date_to;
}

void expenses_mst::setColumnInfo()
{
    ui->tw_expList->setColumnCount(6); //set the listing column number
    ui->tw_expList->setHeaderLabels(QStringList() << "Expense ID" << "Expense Date" << "Expense Time" << "Expense Name" << "Expense Amount (RM)" << "Category"); //set the columns header name

    ui->tw_expList->setColumnWidth(0, 70);
    ui->tw_expList->setColumnWidth(1, 100);
    ui->tw_expList->setColumnWidth(2, 100);
    ui->tw_expList->setColumnWidth(3, 270);
    ui->tw_expList->setColumnWidth(4, 140);
    ui->tw_expList->setColumnWidth(5, 110);
}

void expenses_mst::retrieveExpensesList()
{
    ui->tw_expList->clear(); //clear the listing

    QStringList recordsList = retrieveRecords(); //call the method to get the records from text file

    QStringList expRecord;
    QString expId;
    QString expName;
    QString expAmt;
    QString expCat;
    QString expDate;
    QString expTime;

    double total = 0;

    for(int i=0; i<recordsList.size(); i++){
        expRecord = recordsList[i].split("|||||");
        expId = expRecord[0];
        expName = expRecord[1];
        expAmt = expRecord[2];
        expCat = expRecord[3];
        expDate = expRecord[5];
        expTime = expRecord[6];

        total += expAmt.toDouble();

        showListing(expId, expName, expAmt, expCat, expDate, expTime);
    }

    ui->txt_total->setText(QString::number(total,'f', 2));
}

QStringList expenses_mst::retrieveRecords()
{
    QString filename = comGen->getFileName();

    QString name = searchString[0];
    double amtFrom = searchString[1].toDouble();
    double amtTo = searchString[2].toDouble();
    QString category = searchString[3];
    double date_from = searchString[4].toDouble();
    double date_to = searchString[5].toDouble();


    //open file
    QFile file(filename);

    QString line;
    QString recordName;
    double recordAmt;
    QString recordCat;
    QString recordDesc;
    int recordDate;
    QString recordTime;

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
       recordDesc = lineData[4];
       recordDate = lineData[5].toDouble();
       recordTime = lineData[6];

       if((name == "" || recordName.contains(name, Qt::CaseInsensitive))
           && (amtFrom == 0 || amtFrom <= recordAmt)
           && (amtTo == 0 || amtTo >= recordAmt)
           && (category == "0" || category == recordCat)
           && (date_from == 0 || date_from <= recordDate)
           && (date_to == 0 || date_to >= recordDate)){
           linesList << line;
       }
    }
    file.close();

    return linesList;
}

void expenses_mst::showListing(QString id, QString name, QString amt, QString cat, QString date, QString time)
{
    QString catName = "";

    if(cat != "0"){
        catName = comGen->convertIdName("category.txt", cat);
    }

    QString dateS = comExp->convertDate(date);
    QString timeS = comExp->convertTime(time);

    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_expList);

    itm->setText(0, id); //column, data
    itm->setText(1, dateS); //column, data
    itm->setText(2, timeS); //column, data
    itm->setText(3, name); //column, data
    itm->setText(4, amt); //column, data
    itm->setText(5, catName); //column, data
}

void expenses_mst::initialDateSelection()
{
    //current date
    QDate date = QDate::currentDate();
    int year = date.toString("yyyy").toInt();
    QString month = date.toString("MM");
    QString dayS = date.toString("dd");

    //starting year in the selection
    int startYear = 2000;

    //identify the day in a month
    identifyDayinMonthFrom(2000, "1");
    identifyDayinMonthTo(year, month);

    //set the date selection
    for(int i=startYear;i<=year;i++){
        QString iYear = QString::number(i);
        if(i<10){
            iYear = "0" + iYear;
        }
        ui->lst_year_from->addItem(iYear, iYear);
        ui->lst_year_to->addItem(iYear, iYear);
    }

    for(int i=1;i<=12;i++){
        QString iMonth = QString::number(i);
        if(i<10){
            iMonth = "0" + iMonth;
        }
        ui->lst_month_from->addItem(iMonth,iMonth);
        ui->lst_month_to->addItem(iMonth,iMonth);
    }

    //set date_from
    ui->lst_year_from->setCurrentText("2000");
    ui->lst_year_from->setCurrentText("1");
    ui->lst_year_from->setCurrentText("1");

    //set current date to date_to
    QString yearS = QString::number(year);

    ui->lst_year_to->setCurrentText(yearS);
    ui->lst_month_to->setCurrentText(month);
    ui->lst_day_to->setCurrentText(dayS);
}

void expenses_mst::identifyDayinMonthFrom(int year, QString month)
{
    ui->lst_day_from->clear();
    int day_in_month_from = comExp->identifyDayinMonth(year, month);
    for(int i=1;i<=day_in_month_from;i++){
        QString iDay = QString::number(i);
        if(i<10){
            iDay = "0" + iDay;
        }
        ui->lst_day_from->addItem(iDay,iDay);
    }
}

void expenses_mst::identifyDayinMonthTo(int year, QString month)
{
    ui->lst_day_to->clear();
    int day_in_month_from = comExp->identifyDayinMonth(year, month);
    for(int i=1;i<=day_in_month_from;i++){
        QString iDay = QString::number(i);
        if(i<10){
            iDay = "0" + iDay;
        }
        ui->lst_day_to->addItem(iDay,iDay);
    }
}

void expenses_mst::on_btn_edit_clicked()
{
    int currentRow = ui->tw_expList->currentIndex().row(); //user selected row

    //if user click edit button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to edit");
        return;
    }

    QString currentId = ui->tw_expList->currentItem()->text(0);

    editExp = new edit_expense(this);
    editExp->retrieveRecord(currentId);
    editExp->setModal(true);
    editExp->exec();

    retrieveExpensesList(); //show the listing
}

void expenses_mst::on_tw_expList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_btn_edit_clicked();
}

void expenses_mst::on_btn_delete_clicked()
{
    int currentRow = ui->tw_expList->currentIndex().row(); //user selected row

    //if user click delete button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to delete");
        return;
    }
    QString currentId = ui->tw_expList->currentItem()->text(0);

    //selected row's name
    QString currentText = ui->tw_expList->currentItem()->text(3);
    int reply = QMessageBox::question(this,"Delete Record","Are you sure want to delete " +currentText+"?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){

        comGen->deleteRecord(currentId);
        retrieveExpensesList();

    }
}

void expenses_mst::on_btn_search_clicked()
{
    setSearchString();
    retrieveExpensesList();
}

void expenses_mst::on_lst_month_from_currentIndexChanged(const QString &arg1)
{
    int year = ui->lst_year_from->currentText().toInt();
    identifyDayinMonthFrom(year, arg1);
}

void expenses_mst::on_lst_year_from_currentIndexChanged(const QString &arg1)
{
    int year = arg1.toInt();
    identifyDayinMonthFrom(year, ui->lst_month_from->currentText());
}

void expenses_mst::on_lst_month_to_currentIndexChanged(const QString &arg1)
{
    int year = ui->lst_year_to->currentText().toInt();
    identifyDayinMonthTo(year, arg1);
}

void expenses_mst::on_lst_year_to_currentIndexChanged(const QString &arg1)
{
    int year = arg1.toInt();
    identifyDayinMonthTo(year, ui->lst_month_to->currentText());
}
