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
    query.exec(sql);

    int size = query.record().count();

    vector<QString> columnName;
    for (int i = 0; i < size; i++){
        columnName.push_back(query.record().fieldName(i));
    }
    result.push_back(columnName);
    if(query.next()){
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
    return _ReadData(QString("select 起始时间 , 小区1 , [RRC连接重建比率 (%)] from tbKPI where 网元名称 = %1 and 起始时间>%2 and 起始时间<%3 order by 起始时间 asc, 小区1 asc")
                    .arg(netName)
                    .arg(startDate.toString("dd/MM/yyyy"))
                    .arg(endDate.toString("dd/MM/yyyy")));
    //return true;
}

// PRB信息统计与查询
vector<vector<QString>> Dispatcher::PRBQuery(QString netName, QDate startDate, QDate endDate)
{
    vector<vector<QString>> result;

    return result;
}
