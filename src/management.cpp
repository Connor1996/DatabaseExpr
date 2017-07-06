﻿#include "management.h"
#include "ui_management.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QStandardItem>
#include <QTableView>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>


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
        _ShowTable([this](){
            return _dispatcher.SectorInfoQuery();
        });
    });

    connect(ui->eNodeQuery, &QPushButton::clicked, [this](){
        _ShowTable([this](){
            return _dispatcher.NodeInfoQuery();
        });
    });

    connect(ui->KPIQuery, &QPushButton::clicked, [this](){
        _ShowGraph([this](){
            return _dispatcher.KPIQuery();
        });
    });

    connect(ui->PRBQuery, &QPushButton::clicked, [this](){
        _ShowGraph([this](){
            return _dispatcher.PRBQuery();
        });
    });
}

void Management::_ShowTable(std::function<std::vector<std::vector<std::string>>(void)> fn)
{
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


void Management::_ShowGraph(std::function<std::vector<std::vector<std::string>>(void)> fn)
{
    const auto& result = fn();
    if (result.size() == 0)
        return;

    // 判断是否已经有widget，有则删除
    if(ui->scrollArea->widget() != 0)
        delete ui->scrollArea->widget();

    //page2 折线图
    auto linescene = new QGraphicsScene();
    auto lineview = new QGraphicsView();
    lineview->setScene(linescene);
    lineview->setRenderHint(QPainter::Antialiasing);
    linescene->setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    lineview->setSceneRect(10, 10,560, 300);
    lineview->setSizeIncrement(560,300);
    lineview->resizeAnchor();

    QCategoryAxis *axisX = new QCategoryAxis();
    QCategoryAxis *axisY = new QCategoryAxis();
    //axisY->setRange(0,6);
    axisX->setRange(0,7);


    // 构建 series，作为图表的数据源
    auto lineseries = new QLineSeries();
    lineseries->append(0, 0);
    lineseries->append(1, 2);
    auto lineChart = new QChart();
    lineChart->addSeries(lineseries);  // 将 series 添加至图表中
    lineChart->setAxisX(axisX, lineseries);
    lineChart->setAxisY(axisY, lineseries);

    lineChart->legend()->hide();
    lineChart->createDefaultAxes();
    lineChart->setTitle("Week Line Chart");
    lineChart->setGeometry(10, 10, 560, 300);
    linescene->addItem(lineChart);

    ui->scrollArea->setWidget(lineview);
}



