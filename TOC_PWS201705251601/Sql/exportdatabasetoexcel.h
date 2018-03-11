#ifndef EXPORTDATABASETOEXCEL_H
#define EXPORTDATABASETOEXCEL_H

#include <QDialog>
#include <QSqlTableModel>
#include <QString>
#include <QSqlError>
#include <QList>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>
#include <QAxBase>
#include <QAxObject>
#include <QDateTime>
#include <QModelIndex>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlRecord>
#include <QTableView>
#include <QDialog>
#include <QPushButton>
#include <QVector>

#include "iterator"
#include "list"
#include "algorithm"
#include "iostream"
#include <QFile>
#include <QFileDialog>
#include <QAbstractItemView>
#include <QSqlQuery>
class ExportDataBaseToExcel : public QDialog
{
    Q_OBJECT

public:
    ExportDataBaseToExcel(QWidget *parent = 0);
    ~ExportDataBaseToExcel();
    bool exportData(QSqlDatabase &db , const QString &Table , const QString &filename , bool AutoSave);
private:
    void convertToColName(int data,QString &res);
    QString to26AlphabetString(int data);

    QSqlTableModel *exportDataModel;
    QSqlError *error;
};

#endif // EXPORTDATABASETOEXCEL_H
