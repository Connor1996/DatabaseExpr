#ifndef DISPATCH_H
#define DISPATCH_H

#include "server.h"
#include "src/socket.h"
#include "src/include/json.hpp"

using json = nlohmann::json;

// 前置声明解决相互包含
namespace Connor_Socket {
    class Server;
}


class Dispatcher
{
public:

    // 传入SOCKET和Server的构造函数
    // @param:
    //      connection  与相应客户端建立的socket连接
    //      parent      server对象指针
    Dispatcher(SOCKET &connection, Connor_Socket::Server *server)
        : _connection(connection)
    { }

    ~Dispatcher();

    // 根据请求信息，分发到相应的函数处理请求
    // @param:
    //      requestInfo json序列化后请求信息
    // @return:
    //      json序列化后的返回信息
    std::string Dispatch(json requestInfo);


    json KPIHandler(json requestInfo);
private:
    // 与dispatcher绑定的socket连接
    SOCKET _connection;

};

#endif // DISPATCH_H
