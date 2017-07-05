#include "management.h"
#include "ui_management.h"


#include "src/include/json.hpp"
#include "src/protocol.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QStandardItem>
#include <QTableView>

using Connor_Socket::Client;
using json = nlohmann::json;


Management::Management(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Management)
{
    ui->setupUi(this); 
    InitWidget();
    InitConnect();

}

Management::~Management()
{
    delete ui;

}

void Management::InitWidget() {
    this->setWindowTitle(QString::fromWCharArray(L"查询界面"));


}

void Management::InitConnect() {
    connect(ui->sectorQuery, &QPushButton::clicked, [this](){
        _ShowTable([](){
            return std::vector<std::vector<std::string>>{{"S", "d"}, {"张博康", "sdaasd"}};
        });
    });

    connect(ui->eNodeQuery, &QPushButton::clicked, [this](){
        _ShowTable([](){
            return std::vector<std::vector<std::string>>{};
        });
    });

    connect(ui->KPIQuery, &QPushButton::clicked, [this](){

    });

    connect(ui->PRBQuery, &QPushButton::clicked, [this](){

    });
}

void Management::_ShowTable(std::function<std::vector<std::vector<std::string>>(void)> fn) {

    const auto& result = fn();
    if (result.size() == 0)
        return;

    // 判断是否已经有widget，有则删除
    if(ui->scrollArea->widget() != 0)
        delete ui->scrollArea->widget();

    QTableView *tableView = new QTableView();
    tableView->setFrameStyle(QFrame::NoFrame);
    //设置选中时为整行选中
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    int cnt = result[0].size();

    QStandardItemModel *model = new QStandardItemModel();
    for (int i = 0; i < cnt; i++) {
        model->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(result[0][i])));
    }

    tableView->setModel(model);
    //利用setModel()方法将数据模型与QTableView绑定
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < cnt; j++) {
            model->setItem(i, j, new QStandardItem(QString::fromStdString(result[i][j])));
        }
    }

    ui->scrollArea->setWidget(tableView);
}


void Management::_ShowGraph(std::function<std::vector<std::vector<std::string>>(void)> fn) {

}






