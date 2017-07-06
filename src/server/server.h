#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <set>

#include "dispatch.h"
#include "src/socket.h"
#include "request.h"

// 前置声明解决相互包含
class Dispatcher;


namespace Connor_Socket {

class Server : Socket
{
public:
    // 构造函数，打开监听接口等待请求
    Server();

    ~Server();

    void Start();


protected:
    // 监听客户端访问的socket
    SOCKET _listeningSocket;

    // 地址信息
    SOCKADDR_IN _serverAddr;

    // 持有与各个客户端保持通信的线程
    std::vector<std::thread> _socketThreads;

    // 持有用户名相对应的dispathcer
    std::unordered_map<int, Dispatcher*> _dispatchers;

    // 所连接的socket数目
    int _count;

};

}
#endif // SERVER_H
