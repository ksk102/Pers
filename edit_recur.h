#ifndef EDIT_RECUR_H
#define EDIT_RECUR_H

#include <QDialog>
#include "common_general.h"
#include "common_recur.h"

namespace Ui {
class edit_recur;
}

class edit_recur : public QDialog
{
    Q_OBJECT

public:
    explicit edit_recur(QWidget *parent = 0);
    ~edit_recur();

    void retrieveRecord(QString); //get the data into the textbox

private slots:
    void on_btn_cancel_clicked();

    void on_btn_confirm_clicked();

    void on_lst_recurType_currentIndexChanged(const QString &arg1);

private:
    Ui::edit_recur *ui;
    common_general *comGen;
    common_recur *commonRecur;

    QString selectedId; //current selected record

    bool preCheckUserInput(QString, QString, QString); //check user input
    void retrieveCategory(); //retrieve category from text file
    void showHideDayType(QString); //to show or hide day type listbox
};

#endif // EDIT_RECUR_H
