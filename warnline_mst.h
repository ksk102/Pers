#ifndef WARNLINE_MST_H
#define WARNLINE_MST_H

#include <QDialog>
#include "add_warn.h"
#include "edit_warn.h"
#include "common_warn.h"
#include <QStringList>

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

private:
    QString searchString;

    Ui::warnLine_mst *ui;
    add_warn *addWarn;
    edit_warn *editWarn;
    general *gen;
    common_warn *comWarn;

    QStringList retrieveRecords(QString, QString); //retrieve listing from file
    void showListing(QString, QString, QString, QString, QString, QString); //show listing on the treewidget
    void retrieveWarningList(QString); //retrieve records from file
    bool retrieveCategory(); //get category list
    void showCatListing(QString, QString); //show category list

};

#endif // WARNLINE_MST_H
