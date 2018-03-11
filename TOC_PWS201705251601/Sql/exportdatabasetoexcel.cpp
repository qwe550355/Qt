#include "exportdatabasetoexcel.h"

ExportDataBaseToExcel::ExportDataBaseToExcel(QWidget *parent)
    : QDialog(parent)
{

}

ExportDataBaseToExcel::~ExportDataBaseToExcel()
{

}
bool ExportDataBaseToExcel::exportData(QSqlDatabase &db , const QString &Table , const QString &filename, bool AutoSave)
{
    exportDataModel = new QSqlTableModel(this,db);
    QStringList Tables = db.tables(QSql::AllTables);
    qDebug()<<"Tables:"<<Tables;
    int TablesSize;
    TablesSize = Tables.size();
    qDebug()<<"TablesSize:"<<TablesSize;
    int i;
    for(i = 0 ; i < TablesSize ; ++i)
    {
        if(Tables.at(i) == Table)
            break;
        else
            continue;
    }
    qDebug()<<"find table at:"<<i;
    if(i == TablesSize)
    {
        qDebug()<<tr("Such %1 has no match %2").arg(db.databaseName()).arg(Table);
        return false;
    }
    exportDataModel->setTable(Tables.at(i));
    if(exportDataModel->lastError().isValid())
    {
        qDebug()<<exportDataModel->lastError();
        return false;
    }
    exportDataModel->select();

    QVariant Variant;
    QList<QVariant> rowVariant;
    QList<QList<QVariant>> myExportData;

    int exportDataModelRowSize = exportDataModel->rowCount();
    int exportDataModelColSize = exportDataModel->columnCount();

    qDebug()<<tr("%1:").arg(db.databaseName())<<exportDataModelRowSize<<"\n";
    qDebug()<<tr("%1:").arg(db.databaseName())<<exportDataModelColSize<<"\n";

    for(int i = 0 ; i < exportDataModelRowSize ; ++i)
    {
        for(int j = 0 ; j < exportDataModelColSize ; ++j)
        {
            QModelIndex idIndex = exportDataModel->index(i,j);
            Variant = idIndex.data();
            rowVariant.append(Variant);
        }
        myExportData.append(rowVariant);
        rowVariant.clear();
    }
    //建立Excel对象

    QAxObject *excel = new QAxObject("Excel.Application");

    excel->dynamicCall("SetVisible(bool)", false);

    excel->setProperty("Visible", false);

    QAxObject *workbooks = excel->querySubObject("WorkBooks");

    excel->setProperty("Caption","TemperatureData");
    workbooks->dynamicCall("Add");

    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheets = workbook->querySubObject("Worksheets");
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);
    if(worksheet == NULL)
    {
        qDebug()<<"worksheet false";
        return false;
    }
    if(myExportData.size() <= 0)
    {
        qDebug()<<"DataBase.size() <= 0";
        return false;
    }
    int row = myExportData.size();
    int col = myExportData.at(0).size();
    qDebug()<<"row:"<<row;
    qDebug()<<"col:"<<col;
    QString rangStr;
    convertToColName(col,rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    qDebug()<<rangStr;
    QAxObject *range = worksheet->querySubObject("Range(const QString &)",QString(rangStr));
    if(range == NULL)
    {
        qDebug()<<"range false";
        return false;
    }
    bool succ = false;
    QVariantList vars;
    QVariant res;
    const int rows = myExportData.size();
    for(int i = 0 ; i < rows ; ++i)
    {
        vars.append(QVariant(myExportData[i]));
    }
    res = QVariant(vars);
    succ = range->setProperty("Value",res);
    qDebug()<<"Write Success?:"<<succ;

    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyyMMddhhmmss");
    qDebug()<<"Current Time:"<<timeStr;
    QString path;
    path = QDir::currentPath();
    path = path.replace("/","\\");
    QString saveAsPara = path +"\\" + filename + timeStr + ".xlsx";

    workbook->dynamicCall("SaveAs(const QString&)",saveAsPara);
    if(!AutoSave)
        QMessageBox::information(this,tr("OK"),tr("Save Successfully"));
    workbook->dynamicCall("Close(Boolean)",false);
    worksheet->clear();//释放所有工作表
    excel->dynamicCall("Quit(void)");
    delete excel;//释放excel
    return true;
}
void ExportDataBaseToExcel::convertToColName(int data, QString &res)
{
    int tempData = data/26;
    if(tempData > 0)
    {
        int mode = data % 26;
        convertToColName(mode,res);
        convertToColName(tempData,res);
    }
    else
    {
        res = (to26AlphabetString(data) + res);
    }
}
QString ExportDataBaseToExcel::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;
    return QString(ch);
}
