#ifndef DISPATCH_H
#define DISPATCH_H

#include <vector>
#include <string>

using std::string;
using std::vector;


class Dispatcher
{
public:

    Dispatcher();

    ~Dispatcher();

    vector<vector<string>> KPIHandler();
    // tbCell导入
    bool ImportCell(string filePath);

    // tbKPI导入
    bool ImportKPI(string filePath);

    // tbPRB导入
    bool ImportPRB(string filePath);

    // tbMROData导入
    bool ImportMRO(string filePath);

    // 导出给定表
    bool ExportTable(string tableName, string filePath);

    // 小区配置信息查询
    vector<vector<string>> SectorInfoQuery();

    // 基站eNodeB信息查询
    vector<vector<string>> NodeInfoQuery();

    // KPI指标信息查询
    vector<vector<string>> KPIQuery();

    // PRB信息统计与查询
    vector<vector<string>> PRBQuery();

};

#endif // DISPATCH_H
