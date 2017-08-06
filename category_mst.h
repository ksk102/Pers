#ifndef CATEGORY_MST_H
#define CATEGORY_MST_H

#include <QDialog>
#include "add_cat.h"
#include "edit_cat.h"
#include "general.h"

namespace Ui {
class Category_mst;
}

class Category_mst : public QDialog
{
    Q_OBJECT

public:
    explicit Category_mst(QWidget *parent = 0);
    ~Category_mst();

private slots:

    void on_btn_addNew_clicked();

    void on_btn_edit_clicked();

    void on_btn_delete_clicked();

    void on_btn_search_clicked();

private:
    QString searchString;

    Ui::Category_mst *ui;
    add_cat *addCat;
    edit_cat *editCat;
    general *gen;

    void showListing(QString, QString); //show listing on the treewidget
    void retrieveCategory(QString); //retrieve records from file
};

#endif // CATEGORY_MST_H
