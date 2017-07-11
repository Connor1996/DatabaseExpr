#include "dispatch.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <ctime>

#include <QMessageBox>
#include <QApplication>
#include <QtSql\QSqlError>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlRecord>

Dispatcher::Dispatcher()
    : _lastSql("")
{
    connect_database();
}

Dispatcher::~Dispatcher()
{

}

void Dispatcher::connect_database()
{
    db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("DRIVER={SQL SERVER};"
                          "SERVER=%1;"
                          "DATABASE=%2;"
                          "UID=%3;"
                          "PWD=%4;")
            .arg("XB-20170316TUZZ")
            .arg("TD-LTE")
            .arg("sa")
            .arg("1212");

    db.setDatabaseName(dsn);
    if(!db.open()){
        QMessageBox::warning(0, qApp->tr("Cannot open database"),
                             db.lastError().databaseText(), QMessageBox::Cancel);
    }
    else
        qDebug() << "OK!";
}

bool Dispatcher::ExportLast()
{
    if (_lastSql == "")
        return false;
    // do last sql
    // and export

    return true;
}

bool Dispatcher::_import(QString sql)
{
    QSqlQuery query(db);
    bool result = query.exec(sql);
    qDebug() << query.lastError().databaseText();
    return result;
}

// tbCell导入
bool Dispatcher::ImportCell(QString filePath)
{
    // todo
    QString sql;

    sql = QString("select * into tbKPI from OPENDATASOURCE('Microsoft.Ace.OLEDB.12.0',"

                  "'Data Source=%1;"
                  "Extended Properties=Excel 8.0')...[sheet1$]")
                   .arg(filePath);

    qDebug() << sql;
    return _import(sql);
}


// tbKPI导入
bool Dispatcher::ImportKPI(QString filePath)
{
    // todo
    return true;
}


// tbPRB导入
bool Dispatcher::ImportPRB(QString filePath)
{
    // todo
    return true;
}


// tbMROData导入
bool Dispatcher::ImportMRO(QString filePath)
{
    // todo
    return true;
}


// 导出给定表
bool Dispatcher::ExportTable(QString tableName, QString filePath)
{
    // todo
    QString sql;

       sql = QString("EXEC master..xp_cmdshell"
                     "'bcp TD-LTE.dbo.%1 out %2 -c -q"
                     " -S %3"
                     " -U %4"
                     " -P %5'")
               .arg(tableName)
               .arg(filePath)
               .arg("XB-20170316TUZZ")
               .arg("sa")
               .arg("1212");
    qDebug() << sql;
    return _import(sql);
    //return true;
}

vector<vector<QString>> Dispatcher::_ReadData(QString sql)
{
    _lastSql = sql;

    vector<vector<QString>> result;

    QSqlQuery query(db);
    if (!query.exec(sql))
        qDebug() << "[ERROR]" << query.lastError().databaseText();
    int size = query.record().count();
    vector<QString> columnName;
    for (int i = 0; i < size; i++){
        columnName.push_back(query.record().fieldName(i));
    }
    result.push_back(columnName);
    while(query.next()){
        vector<QString> item;

        for (int i = 0; i < size; i++){
            item.push_back(query.value(i).toString());
        }
        result.push_back(item);
    }

    return result;
}

// 小区配置信息查询
vector<vector<QString>> Dispatcher::SectorInfoQuery(QString sectorId)
{

    return _ReadData(QString("select * from tbCell where SECTOR_ID = '%1'")
                     .arg(sectorId));
}

// 基站eNodeB信息查询
vector<vector<QString>> Dispatcher::NodeInfoQuery(QString nodeId)
{
    return _ReadData(QString("select * from tbCell where ENODEBID = %1")
                     .arg(nodeId));
}
// KPI指标信息查询

vector<vector<QString>> Dispatcher::KPIQuery(QString netName, QDate startDate, QDate endDate)
{
    return _ReadData(QString::fromLocal8Bit("select 小区1, 起始时间, [RRC连接重建比率 (%)] from tbKPI where 网元名称 = '%1' and 起始时间>='%2 00:00:00' and 起始时间<='%3 00:00:00' order by 起始时间 asc, 小区1 asc")
                    .arg(netName)
                    .arg(startDate.toString("MM/dd/yyyy"))
                    .arg(endDate.toString("MM/dd/yyyy")));
}

// PRB信息统计与查询
vector<vector<QString>> Dispatcher::PRBQuery(QString netName, QDate startDate, QDate endDate)
{
    vector<vector<QString>> result;

    return result;
}

void Dispatcher::_Import_database(QAxObject *worksheet, int start, int end, int rows, QString table)
{
    int label = 'A';
    QString startlabel,endlabel="";
    startlabel = QString((char)label);
    //endlabel = QString((char)(label+rows-1))
    if(rows>26)
        endlabel = QString((char)(label+rows/26-1))+QString((char)(label+rows%26-1));
    else
        endlabel = QString((char)(label+rows-1));
    QString Range = startlabel+ QString::number(start) +":"+ endlabel + QString::number(end);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    QSqlQuery query(db);
    db.transaction();
    QString re = "";
    for(int i=0; i<rows-1; i++)
        re += ", ?";
    QString sql = QString("insert into %1 values(?%2)")
                .arg(table)
                .arg(re);
    //qDebug() << sql;
    query.prepare(sql);
    QVariantList DataList;

    for(int j=0; j<rows; j++)
    {
        QVariantList item;
        for(int i=0; i<=end-start; i++)
        {
            QVariantList allEnvDataList_i =  allEnvDataList[i].toList();
            if(!allEnvDataList_i[j].isNull())
                item.push_back(allEnvDataList_i[j]);
            else
                item.push_back(NULL);
        }
        DataList.push_back(item);
    }
    qDebug() << DataList.count();
    for(int j=0; j<rows; j++)
        query.addBindValue(DataList[j].toList());
    query.execBatch();
    db.commit();
}

void Dispatcher::Read_Excel(QString FileName, QString table)
{
    QAxObject *excel = NULL;
    QAxObject *workbooks = NULL;
    QAxObject *workbook = NULL;
    excel = new QAxObject("Excel.Application");
    if (!excel)
    {
        qDebug() << "EXCEL对象丢失!";
    }
    excel->dynamicCall("SetVisible(bool)", false);
    workbooks = excel->querySubObject("WorkBooks");
    workbook = workbooks->querySubObject("Open(QString, QVariant)", FileName);
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//打开第一个sheet
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    qDebug() << intRows;
    qDebug() << intCols;
    int count=1;
    for(int i=0; i<intRows/50; i++)
    {
        if(count==1)
            _Import_database(worksheet,count+1,count+49,intCols,table);
        else
            _Import_database(worksheet,count,count+49,intCols,table);
        count+=50;
        qDebug() << count;
    }
    _Import_database(worksheet, count, intRows, intCols,table);
    workbooks->dynamicCall("Close()");
}
