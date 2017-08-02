#ifndef ADD_CAT_H
#define ADD_CAT_H

#include <QDialog>
#include <QString>
#include "general.h"

namespace Ui {
class add_cat;
}

class add_cat : public QDialog
{
    Q_OBJECT

public:
    explicit add_cat(QWidget *parent = 0);
    ~add_cat();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::add_cat *ui;
    general *gen;

    void checkUserInput(QString); //check the user input before add it into system
    void writeNewCategory(QString); //to write new category into category.txt
    int checkDataExist(QString); //check if user input already exists
};

#endif // ADD_CAT_H
