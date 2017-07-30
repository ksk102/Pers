#ifndef ADD_CAT_H
#define ADD_CAT_H

#include <QDialog>

namespace Ui {
class add_cat;
}

class add_cat : public QDialog
{
    Q_OBJECT

public:
    explicit add_cat(QWidget *parent = 0);
    ~add_cat();

private:
    Ui::add_cat *ui;
};

#endif // ADD_CAT_H
