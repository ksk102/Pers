#ifndef ADD_WARN_H
#define ADD_WARN_H

#include <QDialog>
#include "general.h"

namespace Ui {
class add_warn;
}

class add_warn : public QDialog
{
    Q_OBJECT

public:
    explicit add_warn(QWidget *parent = 0);
    ~add_warn();

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

    void on_list_warnType_currentIndexChanged(const QString &arg1);

private:
    Ui::add_warn *ui;
    general *gen;

    void showHideDayType(QString); //to show or hide day type listbox
    bool retrieveCategory(); //retrieve category from text file
    void showListing(QString, QString); //to show category in the listbox
    QString listBoxIsEmpty(QString); //return empty,if user didn't select any from listbox
    bool preCheckUserInput(QString, QString, QString); //check user input
};

#endif // ADD_WARN_H
