#include "dispatch.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <ctime>

using namespace ORMLite;
using std::cout;


Dispatcher::~Dispatcher() {

}


std::string Dispatcher::Dispatch(json requestInfo)
{
    json responseInfo;
    // 根据请求信息内容，转到相应的处理逻辑
    switch (requestInfo["op"].get<int>()) {
    default:
        std::cout << "[ERROR] Bad request" << std::endl;
        break;
    }

    return std::move(responseInfo.dump());
}




void Dispatcher::_Logout()
{
}
