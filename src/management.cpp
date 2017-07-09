#include "management.h"
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
            return _dispatcher.SectorInfoQuery(ui->sectorIdEdit->text());
        });
    });

    connect(ui->eNodeQuery, &QPushButton::clicked, [this](){
        _ShowTable([this](){
            return _dispatcher.NodeInfoQuery(ui->nodeIdEdit->text());
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

    connect(ui->importCell, &QPushButton::clicked, [this]() {
        if (!_dispatcher.ImportCell(ui->importCellEdit->text()))
            QMessageBox::warning(0, "error", QString::fromLocal8Bit("导入失败"));
    });

    connect(ui->importKPI, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportKPI(ui->importKPIEdit->text()))
            QMessageBox::warning(0, "error", QString::fromLocal8Bit("导入失败"));
    });

    connect(ui->importPRB, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportPRB(ui->importPRBEdit->text()))
            QMessageBox::warning(0, "error", QString::fromLocal8Bit("导入失败"));
    });

    connect(ui->importMRO, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportMRO(ui->importMROEdit->text()))
            QMessageBox::warning(0, "error", QString::fromLocal8Bit("导入失败"));
    });

    connect(ui->exportTable, &QPushButton::clicked, [this](){
        if (!_dispatcher.ExportLast())
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导出失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });

}

void Management::_ShowTable(function<vector<vector<QString>>(void)> fn)
{
    const auto& result = fn();
    ui->count->setText(QString::number(result.size() - 1));
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
        model->setHorizontalHeaderItem(i, new QStandardItem(result[0][i]));
    }

    tableView->setModel(model);
    //利用setModel()方法将数据模型与QTableView绑定
    for (int i = 1; i < result.size(); i++) {
        for (int j = 0; j < cnt; j++) {
            model->setItem(i - 1, j, new QStandardItem(result[i][j]));
        }
    }

    ui->scrollArea->setWidget(tableView);
}


void Management::_ShowGraph(function<vector<vector<QString>>(void)> fn)
{
    const auto& result = fn();

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
    lineview->setFrameStyle(QFrame::NoFrame);

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
    //lineChart->setTitle("Week Line Chart");
    lineChart->setGeometry(10, 10, 560, 300);
    linescene->addItem(lineChart);

    ui->scrollArea->setWidget(lineview);
}




