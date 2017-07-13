#ifndef DISPATCH_H
#define DISPATCH_H

#include <vector>
#include <QString>
#include <QDate>
#include <functional>
#include <QtSql/QSqlDatabase>
#include <QAxObject>
#include <QSqlQuery>

using std::vector;
using std::function;

class Dispatcher
{
public:

    Dispatcher();

    static bool ConnectDatabase(QString, QString);

    // tbCell导入
    bool ImportCell(QString tableName, QString filePath);

    // 导出给定表
    bool ExportTable(QString, QString);

    bool ExportLast(QString);

    double GetLastTime() {
        return _time;
    }

    // 小区配置信息查询
    vector<vector<QString>> SectorIdQuery(QString);
    vector<vector<QString>> SectorNameQuery(QString);

    vector<vector<QString>> SectorId();

    // 基站eNodeB信息查询
    vector<vector<QString>> NodeIdQuery(QString);
    vector<vector<QString>> NodeNameQuery(QString);

    // KPI指标信息查询
    vector<vector<QString>> KPIQuery(QString, QString, QDate, QDate);

    // PRB信息统计与查询
    vector<vector<QString>> PRBQuery(QString, QDateTime, QDateTime);

    vector<vector<QString>> C2IAnalyse();

    vector<vector<QString>> TripleAnalyse(double rate);
private:
    vector<vector<QString>> _ReadData(QString);

    bool _ReadDatabase(QString, QString, QString);

    bool _ReadExcel(QString, QString);

    bool __ImportDatabase(QAxObject *, int, int, int, QString);

    bool __CheckData(QString, QVariantList, int);

    bool _prepareforPRB();

    bool _prepareforC2I();

    bool _prepareforTriple(double rate);

    QSqlQuery query;

    double _time;

    //连接数据库
    static QSqlDatabase db;

    QString _lastquery;
};

#endif // DISPATCH_H
