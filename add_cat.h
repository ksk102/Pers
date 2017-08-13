#ifndef ADD_CAT_H
#define ADD_CAT_H

#include <QDialog>
#include <QString>
#include "common_general.h"

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
    common_general *comGen;

    bool preCheckUserInput(QString); //check user input
};

#endif // ADD_CAT_H
