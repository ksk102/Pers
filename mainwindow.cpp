#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recurring = new recurring_expenses();
    showTodayExpenses(); //show your expenses summary in the mainwindow

    //warnline function
    showExceedWarning();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete recurring;
    delete spendLimit;
}

void MainWindow::on_btn_addCat_clicked()
{
    categoryMst = new Category_mst(this);
    //categoryMst->show();
    categoryMst->setModal(true);
    categoryMst->exec();
}

void MainWindow::on_btn_addWarn_clicked()
{
    warnLineMst = new warnLine_mst(this);
    //warnLineMst->show();
    warnLineMst->setModal(true);
    warnLineMst->exec();
}

void MainWindow::on_btn_addRecur_clicked()
{
    recurMst = new recur_expenses_mst(this);
    recurMst->setModal(true);
    recurMst->exec();
    showTodayExpenses();
    showExceedWarning();
}

void MainWindow::on_btn_addExp_clicked()
{
    addExp = new add_expense(this);
    addExp->setModal(true);
    addExp->exec();
    showTodayExpenses();
    showExceedWarning();
}

void MainWindow::on_btn_viewExp_clicked()
{
    expMst = new expenses_mst(this);
    expMst->setModal(true);
    expMst->exec();
    showTodayExpenses();
    showExceedWarning();
}

QString MainWindow::getTodayDay()
{
    QDate today = QDate::currentDate();
    QString day = today.toString("dd");

    return day;
}

QString MainWindow::getTodayMonth()
{
    QDate today = QDate::currentDate();
    QString month = today.toString("MM");

    return month;
}

QString MainWindow::getTodayYear()
{
    QDate today = QDate::currentDate();
    QString year = today.toString("yyyy");

    return year;
}

void MainWindow::showTodayExpenses()
{
    ui->lw_todayExp->clear();
    ui->lw_monthExp->clear();
    ui->lw_yearExp->clear();
    ui->lw_exp->clear();

    //get categories to show in mainwindows
    QStringList catList = comGen->retrieveRecords("category.txt", "");
    QStringList catRecord;
    QStringList catId;

    for(int i=0; i<catList.size(); i++){
        catRecord = catList[i].split("|||||");
        catId << catRecord[0];
    }


    //get every expenses
    QStringList expensesList = comGen->retrieveRecords("expenses.txt", "");

    QStringList expRecord;
    double expAmt;
    QString expCat;
    QString expDate;
    double todaytotal = 0;
    double monthtotal = 0;
    double yeartotal = 0;
    double lifetotal = 0;
    double todaycattotal[catId.size()] = {0};
    double monthcattotal[catId.size()] = {0};
    double yearcattotal[catId.size()] = {0};
    double lifecattotal[catId.size()] = {0};

    //format the date
    QString todayYear = getTodayYear();
    QString todayMonth = todayYear + getTodayMonth();
    QString todayDate = todayMonth + getTodayDay();

    for(int i=0; i<expensesList.size(); i++){
        expRecord = expensesList[i].split("|||||");
        expAmt = expRecord[2].toDouble();
        expCat = expRecord[3];
        expDate = expRecord[5];

        //calculate total spending today
        if(todayDate == expDate){
            todaytotal += expAmt;
            ui->txt_todayExp->setText(QString::number(todaytotal, 'f', 2));
        }

        //calculate total spending in current month
        QDate expDateD = QDate::fromString(expDate, "yyyyMMdd");
        QString expMonth = expDateD.toString("yyyyMM");
        if(todayMonth == expMonth){
            monthtotal += expAmt;
            ui->txt_monthExp->setText(QString::number(monthtotal, 'f', 2));
        }

        //calculate total spending in current year
        QString expYear = expDateD.toString("yyyy");
        if(todayYear == expYear){
            yeartotal += expAmt;
            ui->txt_yearExp->setText(QString::number(yeartotal, 'f', 2));
        }

        //calculate total spending in lifetime
        lifetotal += expAmt;
        ui->txt_exp->setText(QString::number(lifetotal, 'f', 2));

        //calculate total groupby category
        int j = 0;
        foreach(QString cat, catId){
            //day
            if(expCat == cat && (todayDate == expDate)){
                todaycattotal[j] += expAmt;
            }

            //month
            if(expCat == cat && (todayMonth == expMonth)){
                monthcattotal[j] += expAmt;
            }

            //year
            if(expCat == cat && (todayYear == expYear)){
                yearcattotal[j] += expAmt;
            }

            //lifetime
            if(expCat == cat){
                lifecattotal[j] += expAmt;
            }

            j++;
        }
    }

    //show the spending according to category and day/month/year/lifetime
    int j=0;
    QString theString;
    foreach(QString cat, catId){
        theString = comGen->convertIdName("category.txt", cat) + " - RM"
                            + QString::number(todaycattotal[j], 'f', 2);
        ui->lw_todayExp->addItem(theString);

        theString = comGen->convertIdName("category.txt", cat) + " - RM"
                            + QString::number(monthcattotal[j], 'f', 2);
        ui->lw_monthExp->addItem(theString);

        theString = comGen->convertIdName("category.txt", cat) + " - RM"
                            + QString::number(yearcattotal[j], 'f', 2);
        ui->lw_yearExp->addItem(theString);

        theString = comGen->convertIdName("category.txt", cat) + " - RM"
                            + QString::number(lifecattotal[j], 'f', 2);
        ui->lw_exp->addItem(theString);

        j++;
    }
}

void MainWindow::showExceedWarning()
{
    ui->lw_warnLine->clear();

    //warnline function
    //format the date
    QString todayYear = getTodayYear();
    QString todayMonth = todayYear + getTodayMonth();
    QString todayDate = todayMonth + getTodayDay();

    QStringList catList = comGen->retrieveRecords("category.txt", "");
    QStringList catRecord;
    QString catId;

    for(int i=0; i<catList.size(); i++){
        catRecord = catList[i].split("|||||");
        catId = catRecord[0];

        //check if exceed the spending limit
        spendLimit = new spending_limit(catId, todayDate);
        QStringList overspendList = spendLimit->checkWarnLine();

        foreach(QString data, overspendList){
            //if the exact same string already inside, don't add second one
            if((ui->lw_warnLine->findItems(data,Qt::MatchExactly)).isEmpty()){
                ui->lw_warnLine->addItem(data);
            }
        }
    }
}
