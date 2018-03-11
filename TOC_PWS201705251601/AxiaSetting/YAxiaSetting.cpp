#include "YAxiaSetting.h"
#include "ui_YAxiaSetting.h"

YAxiaSetting::YAxiaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YAxiaSetting)
{
    ui->setupUi(this);
}

YAxiaSetting::~YAxiaSetting()
{
    delete ui;
}
