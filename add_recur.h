#ifndef ADD_RECUR_H
#define ADD_RECUR_H

#include <QDialog>
#include "common_general.h"
#include "common_recur.h"

namespace Ui {
class add_recur;
}

class add_recur : public QDialog
{
    Q_OBJECT

public:
    explicit add_recur(QWidget *parent = 0);
    ~add_recur();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

    void on_lst_recurType_currentIndexChanged(const QString &arg1);

private:
    Ui::add_recur *ui;
    common_general *comGen;
    common_recur *commonRecur;

    bool preCheckUserInput(QString, QString, QString); //check user input
    void retrieveCategory(); //retrieve category from text file
    void showHideDayType(QString); //to show or hide day type listbox
};

#endif // ADD_RECUR_H
