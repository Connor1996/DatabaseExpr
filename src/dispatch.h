#ifndef DISPATCH_H
#define DISPATCH_H

using std::string;
using std::vector;


class Dispatcher
{
public:

    Dispatcher()
        : _connection(connection)
    { }

    ~Dispatcher();

    vector<vector<string>> KPIHandler();

private:
    // 与dispatcher绑定的socket连接
    SOCKET _connection;

};

#endif // DISPATCH_H
