#ifndef EDIT_WARN_H
#define EDIT_WARN_H

#include <QDialog>
#include "common_general.h"
#include "common_warn.h"

namespace Ui {
class edit_warn;
}

class edit_warn : public QDialog
{
    Q_OBJECT

public:
    explicit edit_warn(QWidget *parent = 0);
    ~edit_warn();

    void retrieveRecord(QString); //get the data into the textbox

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::edit_warn *ui;
    common_general *comGen;
    common_warn *comWarn;

    QString selectedId; //current selected record

    bool preCheckUserInput(QString, QString, QString); //check user input
    void retrieveCategory(); //retrieve category from text file
};

#endif // EDIT_WARN_H
