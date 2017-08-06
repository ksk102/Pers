#ifndef EDIT_WARN_H
#define EDIT_WARN_H

#include <QDialog>
#include "general.h"
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

    void on_list_warnType_currentIndexChanged(const QString &arg1);

private:
    Ui::edit_warn *ui;
    general *gen;
    common_warn *comWarn;

    QString selectedId; //current selected record

    void showHideDayType(QString); //to show or hide day type listbox
    bool retrieveCategory(); //retrieve category from text file
    void showListing(QString, QString); //to show category in the listbox
    bool preCheckUserInput(QString, QString, QString); //check user input
    void editWarnLine(QString, QString); //edit the record in textfile
};

#endif // EDIT_WARN_H
