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

    searchString = ui->txt_name->text();

    ui->tw_catList->setColumnCount(2); //set the listing column number
    ui->tw_catList->setHeaderLabels(QStringList() << "Category ID" << "Category Name"); //set the columns header name
    retrieveCategory(searchString); //show the listing
}

Category_mst::~Category_mst()
{
    delete ui;
}

void Category_mst::on_btn_addNew_clicked()
{
    addCat = new add_cat(this);
    addCat->setModal(true);
    addCat->exec();

    retrieveCategory(searchString); //show the listing
}

void Category_mst::showListing(QString catId, QString catName)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_catList);
    itm->setText(0, catId); //column, data
    itm->setText(1, catName); //column, data
}

void Category_mst::retrieveCategory(QString searchString)
{
    ui->tw_catList->clear(); //clear the listing

    gen = new general();
    QStringList recordsList = gen->retrieveRecords("category.txt", searchString); //call the method to get the records from text file

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

    retrieveCategory(searchString); //show the listing
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

        gen = new general();
        if(!gen->deleteRecord("category",currentId)){
            QMessageBox::critical(this,"Unsucessful","Error occur while deleting the data");
        }

        retrieveCategory(searchString);

    }
}

void Category_mst::on_btn_search_clicked()
{
    searchString = ui->txt_name->text();
    retrieveCategory(searchString);
}
