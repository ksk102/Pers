#ifndef CATEGORY_MST_H
#define CATEGORY_MST_H

#include <QDialog>
#include "add_cat.h"
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

private:
    Ui::Category_mst *ui;
    add_cat *addCat;
    general *gen;

    void showListing(QString);
    void retrieveCategory();
};

#endif // CATEGORY_MST_H
