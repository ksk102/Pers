#ifndef RECUR_EXPENSES_MST_H
#define RECUR_EXPENSES_MST_H

#include <QDialog>
#include <QTreeWidget>
#include <QStringList>
#include "common_general.h"
#include "add_recur.h"
#include "edit_recur.h"

namespace Ui {
class recur_expenses_mst;
}

class recur_expenses_mst : public QDialog
{
    Q_OBJECT

public:
    explicit recur_expenses_mst(QWidget *parent = 0);
    ~recur_expenses_mst();

private slots:
    void on_btn_add_clicked();

    void on_btn_edit_clicked();

    void on_tw_recurList_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_btn_delete_clicked();

    void on_btn_search_clicked();

private:
    QStringList searchString;

    Ui::recur_expenses_mst *ui;
    common_general *comGen;
    add_recur *addRecur;
    edit_recur *editRecur;

    void retrieveCategory(); //retrieve category from text file
    void setColumnInfo();
    void retrieveRecurList(); //retrieve records from file
    QStringList retrieveRecords(); //retrieve listing from file
    void showListing(QString, QString, QString, QString, QString, QString); //show listing on the treewidget
    void setSearchString(); //get searchString from userinputs
};

#endif // RECUR_EXPENSES_MST_H
