#ifndef XAXIASETTING_H
#define XAXIASETTING_H

#include <QDialog>

namespace Ui {
class XAxiaSetting;
}

class XAxiaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit XAxiaSetting(QWidget *parent = 0);
    ~XAxiaSetting();

private:
    Ui::XAxiaSetting *ui;
};

#endif // XAXIASETTING_H
