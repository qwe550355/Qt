#include "TOC.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TOC w;
    w.setFixedSize(1064,678);
    w.show();

    return a.exec();
}
