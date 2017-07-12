﻿#include "dispatch.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <ctime>

#include <QtNetwork\QHostInfo>
#include <QMessageBox>
#include <QApplication>
#include <QtSql\QSqlError>
#include <QDebug>
#include <QDir>

#include <QSqlQuery>
#include <QSqlRecord>

QSqlDatabase Dispatcher::db = QSqlDatabase::addDatabase("QODBC");

Dispatcher::Dispatcher()
    : query(db)
{

}


bool Dispatcher::ConnectDatabase(QString name, QString password)
{
    QString dsn = QString("DRIVER={SQL SERVER};"
        "SERVER=%1;"
        "DATABASE=%2;"
        "UID=%3;"
        "PWD=%4;")
        .arg(QSysInfo::machineHostName()) //XB-20170316TUZZ
        .arg("TD-LTE")
        .arg(name)
        .arg(password);
    db.setDatabaseName(dsn);
    return db.open();
}

bool Dispatcher::ExportLast(QString filePath)
{

    if (query.lastQuery() == "")
        return false;
    // do last sql
    // and export

    return _ReadDatabase(query.lastQuery(),query.lastQuery(), filePath);
}


// tbCell导入
bool Dispatcher::ImportCell(QString filePath)
{
    return _ReadExcel("tbCell", filePath);
}


// tbKPI导入
bool Dispatcher::ImportKPI(QString filePath)
{
    return _ReadExcel("tbKPI", filePath);
}


// tbPRB导入
bool Dispatcher::ImportPRB(QString filePath)
{
    return _ReadExcel("tbPRB", filePath);
}


// tbMROData导入
bool Dispatcher::ImportMRO(QString filePath)
{
    return _ReadExcel("tbMROData", filePath);
}


// 导出给定表
bool Dispatcher::ExportTable(QString tableName, QString filePath)
{
    QString sql = QString("select * from %1").arg(tableName);
    QString ssql = QString("select TOP 1 * from %1").arg(tableName);
    return _ReadDatabase(sql, ssql, filePath);
}

vector<vector<QString>> Dispatcher::_ReadData(QString sql)
{
    vector<vector<QString>> result;

    if (!query.exec(sql))
        qDebug() << "[ERROR]" << query.lastError().databaseText();
    int size = query.record().count();
    vector<QString> columnName;
    for (int i = 0; i < size; i++) {
        columnName.push_back(query.record().fieldName(i));
    }
    result.push_back(columnName);
    while (query.next()) {
        vector<QString> item;

        for (int i = 0; i < size; i++) {
            item.push_back(query.value(i).toString());
        }
        result.push_back(item);
    }

    return result;
}

// 小区配置信息查询
vector<vector<QString>> Dispatcher::SectorIdQuery(QString sectorId)
{

    return _ReadData(QString("select * from tbCell where SECTOR_ID = '%1'")
        .arg(sectorId));
}

vector<vector<QString>> Dispatcher::SectorNameQuery(QString sectorName)
{
    return _ReadData(QString("select * from tbCell where SECTOR_NAME = '%1'")
        .arg(sectorName));
}

vector<vector<QString>> Dispatcher::SectorId()
{
    return _ReadData(QString("select SECTOR_NAME from tbCell"));
}


// 基站eNodeB信息查询
vector<vector<QString>> Dispatcher::NodeIdQuery(QString nodeId)
{
    return _ReadData(QString("select * from tbCell where ENODEBID = %1")
        .arg(nodeId));
}

vector<vector<QString>> Dispatcher::NodeNameQuery(QString nodeName)
{
    return _ReadData(QString("select * from tbCell where ENODEB_NAME = %1")
        .arg(nodeName));
}

// KPI指标信息查询
vector<vector<QString>> Dispatcher::KPIQuery(QString netName, QDate startDate, QDate endDate)
{
    return _ReadData(QString::fromLocal8Bit("select 小区1, 起始时间, [RRC连接重建比率 (%)] from tbKPI where 网元名称 = '%1' and 起始时间>='%2 00:00:00' and 起始时间<='%3 00:00:00' order by 小区1 asc, 起始时间 asc")
        .arg(netName)
        .arg(startDate.toString("MM/dd/yyyy"))
        .arg(endDate.toString("MM/dd/yyyy")));
}

// PRB信息统计与查询
vector<vector<QString>> Dispatcher::PRBQuery(QString netName, QDate startDate, QDate endDate)
{
    _prepareforPRB();
    return _ReadData(QString::fromLocal8Bit("select 网元名称, 起始时间, [第60个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] from tbPRBnew where 网元名称 = '%1' and 起始时间>='%2 00:00:00' and 起始时间<='%3 00:00:00' order by 网元名称 asc, 起始时间 asc")
        .arg(netName)
        .arg(startDate.toString("MM/dd/yyyy"))
        .arg(endDate.toString("MM/dd/yyyy")));
}

bool Dispatcher::__ImportDatabase(QAxObject *worksheet, int start, int end, int rows, QString table)
{
    int label = 'A';
    QString startlabel, endlabel = "";
    startlabel = QString((char)label);

    if (rows > 26)
        endlabel = QString((char)(label + rows / 26 - 1)) + QString((char)(label + rows % 26 - 1));
    else
        endlabel = QString((char)(label + rows - 1));
    QString Range = startlabel + QString::number(start) + ":" + endlabel + QString::number(end);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range);
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();

    db.transaction();
    QString re = "";
    for (int i = 0; i< rows - 1; i++)
        re += ", ?";
    QString sql = QString("insert into %1 values(?%2)")
        .arg(table)
        .arg(re);
    //qDebug() << sql;
    query.prepare(sql);
    QVariantList DataList;

    for (int j = 0; j<rows; j++)
    {
        QVariantList item;
        for (int i = 0; i <= end - start; i++)
        {
            QVariantList allEnvDataList_i = allEnvDataList[i].toList();
            if (!allEnvDataList_i[j].isNull())
                item.push_back(allEnvDataList_i[j]);
            else
                item.push_back(QVariant(QVariant::String));
        }
        DataList.push_back(item);
    }
    qDebug() << DataList.count();
    for (int j = 0; j < rows; j++)
        query.addBindValue(DataList[j].toList());
    query.execBatch();
    db.commit();

    return true;
}

bool Dispatcher::_ReadExcel(QString tableName, QString fileName)
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
    workbook = workbooks->querySubObject("Open(QString, QVariant)", fileName);
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//打开第一个sheet
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    qDebug() << intRows;
    qDebug() << intCols;
    int count = 1;
    for (int i = 0; i < intRows / 50; i++)
    {
        if (count == 1)
            __ImportDatabase(worksheet, count + 1, count + 49, intCols, tableName);
        else
            __ImportDatabase(worksheet, count, count + 49, intCols, tableName);
        count += 50;
        qDebug() << count;
    }
    bool result = __ImportDatabase(worksheet, count, intRows, intCols, tableName);
    workbooks->dynamicCall("Close()");
    return result;
}

bool Dispatcher::_ReadDatabase(QString SQL, QString ssql, QString pathName)
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
    workbooks->dynamicCall("Add");
    workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject * worksheets = workbook->querySubObject("Sheets");
    QAxObject * worksheet = workbook->querySubObject("worksheets(int)", 1);
    //worksheets->querySubObject("Add(QVariant)",worksheet->asVariant());
    //QString ssql = QString("select TOP 1 * from %1").arg(tableName);
    query.exec(ssql);
    QAxObject *cellX;
    int label = 'A';
    for (int i = 0; i < query.record().count(); i++) {
        QString X = QString((char)(label + i)) + QString::number(1);//设置要操作的单元格
        cellX = worksheet->querySubObject("Range(QVariant, QVariant)", X);//获取单元格
        cellX->dynamicCall("SetValue(const QVariant&)", QVariant(query.record().fieldName(i)));//设置单元格的值
    }

    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(pathName));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel

    QString sql = QString("insert into OPENROWSET('Microsoft.Jet.OLEDB.4.0'"
        ",'Excel 5.0;HDR=YES;DATABASE=%1',sheet1$)"
        "%2")
        .arg(pathName)
        .arg(SQL);

    qDebug() << sql;
    bool result = query.exec(sql);
    delete excel;

    return result;
}

bool Dispatcher::_prepareforPRB()
{
    QString sql = QString::fromLocal8Bit("create table tbPRBnew ("
                          " 网元名称 nvarchar(50),"
                          " 起始时间 nvarchar(50),");
    for(int i=0; i<99; i++)
    {
        sql += QString::fromLocal8Bit(" [第%1个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,").arg(QString::number(i,10));
    }
    sql += QString::fromLocal8Bit(" [第99个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float)");
    qDebug() << query.exec(sql);
    sql.clear();
    sql = QString::fromLocal8Bit("create procedure PRB_Hour_Pro"
                  " as"
                  " begin"
                  " insert into tbPRBnew select 网元名称, 起始时间");
    for(int i=0; i<100; i++)
    {
        sql += QString::fromLocal8Bit(", avg([第%1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第%1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]").arg(QString::number(i,10));
    }
    sql += QString::fromLocal8Bit(" from tbPRB group by 网元名称, 起始时间 order by 网元名称 asc, 起始时间 asc end");
    qDebug() << query.exec(sql);
    sql.clear();
    sql = "exec dbo.PRB_Hour_pro";
    return query.exec(sql);

}

vector<vector<QString>> Dispatcher::C2IAnalyse()
{
    _prepareforC2I();
    return _ReadData("exec dbo.RSRP_Prb_pro");
}

bool Dispatcher::_prepareforC2I()
{
    QString sql = QString("create table tbC2INew ("
                          " ServingSector nvarchar(50),"
                          " InterferingSector nvarchar(50),"
                          " mean float,"
                          " std float,"
                          " Prb9 float,"
                          " Prb6 float"
                          ")");
    query.exec(sql);
    sql.clear();
    sql = QString("create procedure Calc_mean_std_pro"
                  " as"
                  "  begin"
                  "   select ServingSector, InterferingSector, avg(LteScRSRP - LteNcRSRP) as mean, stdev(LteScRSRP - LteNcRSRP) as std into tbMROMean_Std from tbMROData group by ServingSector, InterferingSector"
                  "  end");
    qDebug() << query.exec(sql);
    sql.clear();
    sql = QString("exec dbo.Calc_mean_std_pro");
    qDebug() << query.exec(sql);
    sql = QString("create procedure RSRP_Prb_pro"
                  " as"
                      " begin"
                          " truncate table tbC2INew"
                          " insert into tbC2INew select ServingSector, InterferingSector, mean, std, 0, 0 from tbMROMean_Std"
                          " update tbC2INew set Prb9 = ("
                                  " case"
                                  " when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std < 5 and (9 - tbC2INew.mean)/tbC2INew.std >= 0 then (select prb from tbNormStatistics where tbNormStatistics.id = round((9 - tbC2INew.mean)/tbC2INew.std, 2, 0) * 100)"
                                  " when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std < 0 then 1 - (select prb from tbNormStatistics where tbNormStatistics.id = abs(round((9 - tbC2INew.mean)/tbC2INew.std, 2, 0)) * 100)"
                                  " when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std >= 5 then 1 "
                                  " end"
                              " ), Prb6 = ("
                                  " case"
                                  " when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std < 5 and (6 - tbC2INew.mean)/tbC2INew.std >= 0 then (select prb from tbNormStatistics where tbNormStatistics.id = round((6 - tbC2INew.mean)/tbC2INew.std, 2, 0) * 100)"
                                  " when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std < 0 then 1 - (select prb from tbNormStatistics where tbNormStatistics.id = abs(round((6 - tbC2INew.mean)/tbC2INew.std, 2, 0)) * 100)"
                                  " when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std >= 5 then 1 "
                                  " end"
                              " )"
                          " select * from tbC2INew where Prb9 is not null and Prb6 is not null"
                      " end");
    return query.exec(sql);
}

vector<vector<QString>> Dispatcher::TripleAnalyse()
{
    qDebug()<<_prepareforTriple();
    return _ReadData("select * from tbC2I3");
}

bool Dispatcher::_prepareforTriple()
{
    QString sql = QString("create table tbC2I3 ("
                          " a varchar(50),"
                          " b varchar(50),"
                          " c varchar (50),"
                          " PRIMARY KEY (a,b,c)"
                      ")");
    qDebug()<<query.exec(sql);
    sql.clear();
    sql = QString("create function Prb6GE70(@x varchar(50), @y varchar(50))"
                  " returns float"
                  " as"
                      " begin"
                      " declare @rslt float"
                      " set @rslt = 0"
                      " select @rslt = Prb6 from tbC2INew where ServingSector = @x and InterferingSector = @y and Prb6 >= 0.7"
                      " return @rslt"
                      " end");
    qDebug()<<query.exec(sql);
    sql.clear();
    sql = QString("with sa (ServingSector, InterferingSector) as ("
                  " select ServingSector, InterferingSector from tbC2INew"
                  " ),"
               " sb (ServingSector, InterferingSector) as ("
                  " select ServingSector, InterferingSector from tbC2INew"
                  " ),"
               " sc (ServingSector, InterferingSector) as ("
                  " select ServingSector, InterferingSector from tbC2INew"
                  " )"
              " insert into tbC2I3 select sa.ServingSector as a, sb.ServingSector as b, sc.ServingSector as c from  sa, sb, sc where sa.ServingSector != sb.ServingSector and sb.ServingSector!=sc.ServingSector and sc.ServingSector != sa.ServingSector"
                  " and ((select dbo.Prb6GE70(sa.ServingSector, sb.ServingSector)) != 0 or (select dbo.Prb6GE70(sb.ServingSector, sa.ServingSector)) != 0 )"
                  " and ((select dbo.Prb6GE70(sa.ServingSector, sc.ServingSector)) != 0 or (select dbo.Prb6GE70(sc.ServingSector, sa.ServingSector)) != 0 )"
                  " and ((select dbo.Prb6GE70(sc.ServingSector, sb.ServingSector)) != 0 or (select dbo.Prb6GE70(sb.ServingSector, sc.ServingSector)) != 0 )");
    return query.exec(sql);
}
