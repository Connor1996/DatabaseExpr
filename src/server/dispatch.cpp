#include "dispatch.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <ctime>

using std::cout;
#include "src/protocol.h"

Dispatcher::~Dispatcher() {

}


std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;
    // 根据请求信息内容，转到相应的处理逻辑
    switch (requestInfo["op"].get<int>()) {
    case KPI:
        responseInfo = KPIHandler(requestInfo);
        break;
    default:
        std::cout << "[ERROR] Bad request" << std::endl;
        break;
    }

    return std::move(responseInfo.dump());
}

json Dispatcher::KPIHandler(json requestInfo)
{
    json responseInfo;

    // ODBC
    // ...

    // while(query.next()) {
    // read item
    // json itemInfo = {...}
    // responseInfo.push_back(itemInfo.dump());

    return responseInfo;
}


