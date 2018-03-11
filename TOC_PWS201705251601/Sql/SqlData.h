#ifndef SQLDATA_H
#define SQLDATA_H

#include <QObject>
#include <iostream>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QVariant>
#include <string.h>
#include"S7/snap7.h"
#include"S7/TypeTransform.h"
#include"QDateTime"

class SqlData
{
public:
    SqlData();
    ~SqlData();
    void insert_database(QSqlDatabase& database,QString name);
    bool connect_db(const QString,QSqlDatabase&);
    void create_table();
    void drop_table();
    void insert_data(QSqlDatabase& db, byte* ReadBuffer);
    double* search_data(int id);
    double* search_data(char *name);
    void delete_data(int age);
    TypeTransform *S7;
    int index = 0;
};

#endif // SQLDATA_H
