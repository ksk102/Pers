#include "category_mst.h"
#include "ui_category_mst.h"
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Category_mst::Category_mst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Category_mst)
{
    ui->setupUi(this);

    comGen = new common_general();
    comGen->setFileName("category.txt");
    comGen->setTmpFileName("category_tmp.txt");

    /*get searchString from userinputs*/
    searchString = ui->txt_name->text();

    setColumnInfo();

    //show the listing
    retrieveCategory();
}

Category_mst::~Category_mst()
{
    delete ui;
    delete comGen;
}

void Category_mst::on_btn_addNew_clicked()
{
    addCat = new add_cat(this);
    addCat->setModal(true);
    addCat->exec();

    retrieveCategory(); //show the listing
}

void Category_mst::showListing(QString catId, QString catName)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_catList);
    itm->setText(0, catId); //column, data
    itm->setText(1, catName); //column, data
}

void Category_mst::retrieveCategory()
{
    ui->tw_catList->clear(); //clear the listing

    QStringList recordsList = comGen->retrieveRecords("", searchString); //call the method to get the records from text file

    QStringList catRecord;
    QString catId;
    QString catName;

    for(int i=0; i<recordsList.size(); i++){
        catRecord = recordsList[i].split("|||||");
        catId = catRecord[0];
        catName = catRecord[1];

        showListing(catId, catName);
    }
}

void Category_mst::setColumnInfo()
{
    ui->tw_catList->setColumnCount(2); //set the listing column number
    ui->tw_catList->setHeaderLabels(QStringList() << "Category ID" << "Category Name"); //set the columns header name
}

void Category_mst::on_btn_edit_clicked()
{
    int currentRow = ui->tw_catList->currentIndex().row(); //user selected row

    //if user click edit button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to edit");
        return;
    }

    QString currentId = ui->tw_catList->currentItem()->text(0);

    editCat = new edit_cat(this);
    editCat->retrieveRecord(currentId);
    editCat->setModal(true);
    editCat->exec();

    retrieveCategory(); //show the listing
}

void Category_mst::on_btn_delete_clicked()
{
    int currentRow = ui->tw_catList->currentIndex().row(); //user selected row

    //if user click delete button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to delete");
        return;
    }
    QString currentId = ui->tw_catList->currentItem()->text(0);

    //selected row's name
    QString currentText = ui->tw_catList->currentItem()->text(1);
    int reply = QMessageBox::question(this,"Delete Record","Are you sure want to delete " +currentText+"?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){
        comGen->deleteRecord(currentId);
        retrieveCategory();
    }
}

void Category_mst::on_btn_search_clicked()
{
    searchString = ui->txt_name->text();
    retrieveCategory();
}

void Category_mst::on_tw_catList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_btn_edit_clicked();
}
