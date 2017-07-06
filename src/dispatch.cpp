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
bool Dispatcher::ImportCell(string filePath)
{
    // todo
    return true;
}


// tbKPI导入
bool Dispatcher::ImportKPI(string filePath)
{
    // todo
    return true;
}


// tbPRB导入
bool Dispatcher::ImportPRB(string filePath)
{
    // todo
    return true;
}


// tbMROData导入
bool Dispatcher::ImportMRO(string filePath)
{
    // todo
    return true;
}


// 导出给定表
bool Dispatcher::ExportTable(string tableName, string filePath)
{
    // todo
    return true;
}

// 小区配置信息查询
vector<vector<string>> Dispatcher::SectorInfoQuery()
{
    vector<vector<string>> result;
    QSqlQuery query(db);
    QString sector_id="11317-128";
    QString sql = QString("select * from tbCell where SECTOR_ID = '%1'")
            .arg(sector_id);
    query.exec(sql);

    int size = query.record().count();
    if(query.first()){
        vector<string> item;
        for (int i = 0; i < size; i++){
            //item.push_back(query.value(i).toString());
            //qDebug() << query.value(i).toString();
        }
        result.push_back(item);
    }
    return result;
}

// 基站eNodeB信息查询
vector<vector<string>> Dispatcher::NodeInfoQuery()
{
    vector<vector<string>> result;

    return result;
}

// KPI指标信息查询
vector<vector<string>> Dispatcher::KPIQuery()
{
    vector<vector<string>> result;

    // ODBC
    // ...

    // while(query.next()) {
    // read item
    // json itemInfo = {...}
    // responseInfo.push_back(itemInfo.dump());

    return result;
}

// PRB信息统计与查询
vector<vector<string>> Dispatcher::PRBQuery()
{
    vector<vector<string>> result;

    return result;
}
