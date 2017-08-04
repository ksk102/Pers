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

    ui->tw_catList->setColumnCount(1); //set the listing column number
    ui->tw_catList->setHeaderLabel("Category Name"); //set the column header name
    retrieveCategory(); //show the listing
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

    ui->tw_catList->clear(); //clear the listing
    retrieveCategory(); //show the listing
}

void Category_mst::showListing(QString item)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->tw_catList);
    itm->setText(0, item); //column, data
}

void Category_mst::retrieveCategory()
{
    //open file
    QFile file("category.txt");

    //check if file exists
    if(!file.exists()){
        return;
    }

    //open file for read only
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return; //couldn't open the file
    }

    //read the file line by line
    QTextStream in(&file);
    QString line;

    while (!in.atEnd()) {
       line = in.readLine();
       showListing(line);
    }
    file.close();
}

void Category_mst::on_btn_edit_clicked()
{

}

void Category_mst::on_btn_delete_clicked()
{
    int currentRow = ui->tw_catList->currentIndex().row(); //user selected row

    //if user click delete button, but didnt select any row
    if(currentRow == -1){
        QMessageBox::information(this,"Error","Please select the record that you wish to delete");
        return;
    }

    //selected row's name
    QString currentText = ui->tw_catList->currentItem()->text(0);
    int reply = QMessageBox::question(this,"Delete Record","Are you sure want to delete " +currentText+"?", QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){

        gen = new general();
        if(!gen->deleteRecord("category",currentRow)){
            QMessageBox::critical(this,"Unsucessful","Error occur while deleting the data");
        }
        ui->tw_catList->clear();
        retrieveCategory();

    }
}
