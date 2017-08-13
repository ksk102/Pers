#ifndef ADD_WARN_H
#define ADD_WARN_H

#include <QDialog>
#include "common_general.h"
#include "common_warn.h"

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

private:
    Ui::add_warn *ui;
    common_general *comGen;
    common_warn *comWarn;

    void retrieveCategory(); //retrieve category from text file
    bool preCheckUserInput(QString, QString, QString); //check user input
};

#endif // ADD_WARN_H
