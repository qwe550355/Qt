#ifndef YAXIASETTING_H
#define YAXIASETTING_H

#include <QDialog>

namespace Ui {
class YAxiaSetting;
}

class YAxiaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit YAxiaSetting(QWidget *parent = 0);
    ~YAxiaSetting();

private:
    Ui::YAxiaSetting *ui;
};

#endif // YAXIASETTING_H
