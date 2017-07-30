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
    void on_pushButton_clicked();

private:
    Ui::Category_mst *ui;
    add_cat *addcat;
};

#endif // CATEGORY_MST_H
