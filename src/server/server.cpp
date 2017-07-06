#include "server.h"

#include "../protocol.h"
#include "../include/json.hpp"

using namespace Connor_Socket;
using json = nlohmann::json;


Server::Server()
    : _count(0)
{
    _listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_listeningSocket == -1)
        throw std::runtime_error("Cannot create listening socket");

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(SERVER_PORT);
    _serverAddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定监听端口
    if (bind(_listeningSocket, reinterpret_cast<SOCKADDR*>(&_serverAddr),
             sizeof(_serverAddr)) == -1)
    {
        closesocket(_listeningSocket);
        throw std::runtime_error("Failed at bind");
    }

    // 开始监听，指定监听队列的大小
    if (listen(_listeningSocket, 5))
    {
        closesocket(_listeningSocket);
        throw std::runtime_error("Failed at listen");
    }
    std::cout << "Wait for connection..." << std::endl;

}

Server::~Server()
{
    shutdown(_listeningSocket, SD_BOTH);
    closesocket(_listeningSocket);
    std::cout << "Server is closed" << std::endl;
}

void Server::Start()
{

    //各个连接客户端线程输出信息锁
    std::mutex mtx;
    // 不断等待客户端请求的到来
    while (true)
    {
        SOCKADDR_IN clientAddr;
        int addrlen = sizeof(clientAddr);
        // 监听到客户端的请求，新建立一个socket保持通信
        SOCKET newConnection = accept(_listeningSocket, (SOCKADDR *)&clientAddr, &addrlen);
        // 将新建的socket放入线程单独运行
        _socketThreads.emplace_back(std::thread([this, &mtx] (SOCKET &&connection)
        {
            char recvBuf[DEFAULT_BUFLEN];
            int recvBufLen = DEFAULT_BUFLEN;
            Dispatcher dispatcher(connection, this);

            if (connection < 0)
            {
                cout << "[ERROR] Failed at accept" << endl;
            }
            else
            {
                cout << "[INFO] New come, now " << ++_count << " connections in total" << endl;

                while(true)
                {
                    if (recv(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        // 保证cout完整执行而不被其他线程打断
                        mtx.lock();
                        // 下线处理由dispatcher的析构函数进行
                        cout << "[INFO] Someone offline, now " << --_count << " connections in total" << endl;
                        mtx.unlock();

                        // 断开连接
                        shutdown(connection, SD_BOTH);
                        closesocket(connection);
                        break;
                    }

                    string receiveInfo(recvBuf);
                    receiveInfo = receiveInfo.substr(0, receiveInfo.find("}") + 1);
                    cout << "[INFO] receive message: " << receiveInfo << endl;

                    std::string responseStr;
                    try
                    {
                        // 调用Dispatcher分发到相应处理逻辑
                        responseStr = dispatcher.Dispatch(std::move(json::parse(receiveInfo)));
                    }
                    catch (std::exception e)
                    {
                        //shutdown(connection, SD_BOTH);
                        //closesocket(connection);
                        cout << "[ERROR] wrong field for json" << endl;
                    }

                    char recvBuf[DEFAULT_BUFLEN];
                    int recvBufLen = DEFAULT_BUFLEN;
                    strcpy(recvBuf, responseStr.c_str());
                    if (send(connection, recvBuf, recvBufLen, 0) <= 0)
                    {
                        cout << "[ERROR] failed at send messager, code: " << WSAGetLastError() << endl;
                    }
                    else
                        cout << "[INFO] send message: " << responseStr << endl;
                }
            }
        }, newConnection));

        _socketThreads.back().detach(); //使子线程独立运行
    }
}

