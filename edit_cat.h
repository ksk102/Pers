#ifndef EDIT_CAT_H
#define EDIT_CAT_H

#include <QDialog>
#include <QString>
#include "common_general.h"

namespace Ui {
class edit_cat;
}

class edit_cat : public QDialog
{
    Q_OBJECT

public:
    explicit edit_cat(QWidget *parent = 0);
    ~edit_cat();

    void retrieveRecord(QString); //get the data into the textbox

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

private:

    QString selectedId; //current selected record

    Ui::edit_cat *ui;
    common_general *comGen;

    bool preCheckUserInput(QString); //check user input
};

#endif // EDIT_CAT_H
