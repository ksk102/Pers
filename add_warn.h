#ifndef ADD_WARN_H
#define ADD_WARN_H

#include <QDialog>

namespace Ui {
class add_warn;
}

class add_warn : public QDialog
{
    Q_OBJECT

public:
    explicit add_warn(QWidget *parent = 0);
    ~add_warn();

private:
    Ui::add_warn *ui;
};

#endif // ADD_WARN_H
