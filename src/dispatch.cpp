#include "dispatch.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <ctime>

Dispatcher::Dispatcher()
{

}

Dispatcher::~Dispatcher()
{

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
