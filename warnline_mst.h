#ifndef WARNLINE_MST_H
#define WARNLINE_MST_H

#include <QDialog>
#include <QStringList>
#include <QTreeWidget>
#include "common_general.h"
#include "add_warn.h"
#include "edit_warn.h"
#include "common_warn.h"

namespace Ui {
class warnLine_mst;
}

class warnLine_mst : public QDialog
{
    Q_OBJECT

public:
    explicit warnLine_mst(QWidget *parent = 0);
    ~warnLine_mst();

private slots:

    void on_btn_add_clicked();

    void on_btn_edit_clicked();

    void on_btn_delete_clicked();

    void on_btn_search_clicked();

    void on_tw_warnList_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    QStringList searchString;

    Ui::warnLine_mst *ui;
    common_general *comGen;
    common_warn *comWarn;
    add_warn *addWarn;
    edit_warn *editWarn;

    void retrieveCategory(); //retrieve category from text file
    void setSearchString(); //get searchString from userinputs
    void setColumnInfo();
    void retrieveWarningList(); //retrieve records from file
    QStringList retrieveRecords(); //retrieve listing from file
    void showListing(QString, QString, QString, QString, QString, QString); //show listing on the treewidget



};

#endif // WARNLINE_MST_H
