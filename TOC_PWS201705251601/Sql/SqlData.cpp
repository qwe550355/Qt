#include "SqlData.h"
#include "SqlData.h"
#include"QDebug"

SqlData::SqlData()
{
    S7 = new TypeTransform();
}
//_________________________________________________________________________________
SqlData::~SqlData()
{

}
//_________________________________________________________________________________
void SqlData::insert_database(QSqlDatabase& database,QString name)//changeable
{
    QSqlQuery query(database);
    if(!query.exec("insert into data(name) values(password('"+name+"') )"))
        std::cout<<query.lastError().text().toStdString()<<std::endl;
    else
        database.commit();
}
//_________________________________________________________________________________
bool  SqlData::connect_db(const QString dbName,QSqlDatabase& db)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qDebug() << "Database Error!";
            return false;
    }
    return true;
}
//_________________________________________________________________________________
void SqlData::create_table()
{
    QSqlQuery query;
    bool ok1 = query.exec("CREATE TABLE IF NOT EXISTS  User (Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "Name VARCHAR(20) NOT NULL,"
                                       "Password INTEGER NULL)");
    bool ok2 = query.exec("CREATE TABLE IF NOT EXISTS  MetersParaM (Date varchar(40) PRIMARY KEY ,"
                                                        "TOC1 REAL,"
                                                        "TOC2 REAL,"
                                                        "FC1Pressure REAL,"
                                                        "FC1Flow REAL,"
                                                        "FC2Pressure REAL,"
                                                        "FC2Flow REAL,"
                                                        "FC3Pressure REAL,"
                                                        "FC3Flow REAL,"
                                                        "rPump REAL)");
    if (ok1&&ok2)
    {
        qDebug()<<"Ceate table User and MetersParaM partition success or it has been created before"<<endl;
    }
    else
    {
        qDebug()<<"Ceate table partition failed, something trouble happens"<<endl;
    }
}
//_________________________________________________________________________________
void SqlData::drop_table()
{
  //null
}
//_________________________________________________________________________________
void SqlData::insert_data(QSqlDatabase& db,byte* ReadBuffer)
{
//    QSqlQuery query(db);
//    query.prepare("INSERT INTO MetersParaM (Date, TOC1, TOC2, FC1Pressure, "
//                    "FC1Flow, FC2Pressure, FC2Flow, FC3Pressure, FC3Flow, rPump) VALUES (:Date, :TOC1, :TOC2, :FC1Pressure, "
//                    ":FC1Flow, :FC2Pressure, :FC2Flow, :FC3Pressure, :FC3Flow, :rPump)");
//    query.bindValue(":Date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd"));
//    query.bindValue(":TOC1", S7->ByteToNum(ReadBuffer,2,4));
//    query.bindValue(":TOC2",S7->ByteToNum(ReadBuffer,6,4));
//    query.bindValue(":FC1Pressure", S7->ByteToNum(ReadBuffer,10,4));
//    query.bindValue(":FC1Flow", S7->ByteToNum(ReadBuffer,14,4));
//    query.bindValue(":FC2Pressure",S7->ByteToNum(ReadBuffer,18,4));
//    query.bindValue(":FC2Flow", S7->ByteToNum(ReadBuffer,22,4));
//    query.bindValue(":FC3Pressure",S7->ByteToNum(ReadBuffer,18,4));
//    query.bindValue(":FC3Flow", S7->ByteToNum(ReadBuffer,22,4));
//    query.bindValue(":rPump", S7->ByteToNum(ReadBuffer,26,4));
//    query.exec();
    index ++;
    QSqlQuery query(db);
    query.prepare("INSERT INTO MetersParaM (Date, TOC1, TOC2, FC1Pressure, "
                    "FC1Flow, FC2Pressure, FC2Flow, FC3Pressure, FC3Flow, rPump) VALUES (:Date, :TOC1, :TOC2, :FC1Pressure, "
                    ":FC1Flow, :FC2Pressure, :FC2Flow, :FC3Pressure, :FC3Flow, :rPump)");
    query.bindValue(":Date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd"));
    query.bindValue(":TOC1",10 + index);
    query.bindValue(":TOC2",7 - index);
    query.bindValue(":FC1Pressure", 7 + index);
    query.bindValue(":FC1Flow", 8 + index);
    query.bindValue(":FC2Pressure", 9 + index);
    query.bindValue(":FC2Flow", 10 + index);
    query.bindValue(":FC3Pressure", 11 + index);
    query.bindValue(":FC3Flow", 12 + index);
    query.bindValue(":rPump", 13 + index);
    query.exec();
    if(index == 5)
        index = 0;
}
//_________________________________________________________________________________
double* SqlData::search_data(int id)
{
    return 0;
}
//_________________________________________________________________________________
double* SqlData::search_data(char *name)
{
    return 0;
}
//_________________________________________________________________________________
void SqlData::delete_data(int age)
{

}
//_________________________________________________________________________________


