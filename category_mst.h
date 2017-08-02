#ifndef CATEGORY_MST_H
#define CATEGORY_MST_H

#include <QDialog>
#include "add_cat.h"

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

private:
    Ui::Category_mst *ui;
    add_cat *addCat;
};

#endif // CATEGORY_MST_H
