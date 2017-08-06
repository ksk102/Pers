#ifndef WARNLINE_MST_H
#define WARNLINE_MST_H

#include <QDialog>
#include "add_warn.h"

namespace Ui {
class warnLine_mst;
}

class warnLine_mst : public QDialog
{
    Q_OBJECT

public:
    explicit warnLine_mst(QWidget *parent = 0);
    ~warnLine_mst();

private slots:

    void on_btn_add_clicked();

private:
    Ui::warnLine_mst *ui;
    add_warn *addWarn;
};

#endif // WARNLINE_MST_H
