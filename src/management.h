#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QWidget>
#include <QLabel>

#include <unordered_map>
#include <functional>

#include "dispatch.h"

namespace Ui {
class Management;
}

class Management : public QWidget
{
    Q_OBJECT

public:
    explicit Management(QWidget *parent = 0);

    ~Management();

private:
    void InitWidget();
    void InitConnect();

    void _ShowTable(function<vector<vector<QString>>(void)>);
    void _ShowBarGraph(function<vector<vector<QString>>(void)>);
    void _ShowLineGraph(function<vector<vector<QString>>(void)>);
    Ui::Management *ui;

    Dispatcher _dispatcher;

};

#endif // MANAGEMENT_H
