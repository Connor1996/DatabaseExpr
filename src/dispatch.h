﻿#ifndef DISPATCH_H
#define DISPATCH_H

#include <vector>
#include <QString>
#include <functional>
#include <QtSql/QSqlDatabase>

using std::vector;
using std::function;

class Dispatcher
{
public:

    Dispatcher();

    ~Dispatcher();

    QSqlDatabase db;

    void connect_database();
    //连接数据库
    vector<vector<QString>> KPIHandler();
    // tbCell导入
    bool ImportCell(QString filePath);

    // tbKPI导入
    bool ImportKPI(QString filePath);

    // tbPRB导入
    bool ImportPRB(QString filePath);

    // tbMROData导入
    bool ImportMRO(QString filePath);

    // 导出给定表
    bool ExportTable(QString tableName, QString filePath);

    // 小区配置信息查询
    vector<vector<QString>> SectorInfoQuery(QString);

    // 基站eNodeB信息查询
    vector<vector<QString>> NodeInfoQuery(QString);

    // KPI指标信息查询
    vector<vector<QString>> KPIQuery();

    // PRB信息统计与查询
    vector<vector<QString>> PRBQuery();

};

#endif // DISPATCH_H
