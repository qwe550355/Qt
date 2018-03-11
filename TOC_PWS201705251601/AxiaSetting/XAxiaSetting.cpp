#include "XAxiaSetting.h"
#include "ui_XAxiaSetting.h"

XAxiaSetting::XAxiaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XAxiaSetting)
{
    ui->setupUi(this);
}

XAxiaSetting::~XAxiaSetting()
{
    delete ui;
}
