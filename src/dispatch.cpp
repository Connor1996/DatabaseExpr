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

// tbCell导入
bool Dispatcher::ImportCell(QString filePath)
{
    // todo
    return true;
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
    return true;
}

vector<vector<QString>> Dispatcher::_ReadData(QString sql)
{
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
    vector<vector<QString>> result;

    return result;
}

// KPI指标信息查询
vector<vector<QString>> Dispatcher::KPIQuery()
{
    vector<vector<QString>> result;

    // ODBC
    // ...

    // while(query.next()) {
    // read item
    // json itemInfo = {...}
    // responseInfo.push_back(itemInfo.dump());

    return result;
}

// PRB信息统计与查询
vector<vector<QString>> Dispatcher::PRBQuery()
{
    vector<vector<QString>> result;

    return result;
}
