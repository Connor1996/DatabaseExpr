#include "management.h"
#include "ui_management.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QStandardItem>
#include <QTableView>
#include <QtCharts/QChartView>
#include <QtCharts/QbarSeries>
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
        if (ui->sectorIdEdit->text() == "") {
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("输入为空"));
            return;
        }

        _ShowTable([this](){
            return _dispatcher.SectorInfoQuery(ui->sectorIdEdit->text());
        });
    });

    connect(ui->eNodeQuery, &QPushButton::clicked, [this](){
        if (ui->nodeIdEdit->text() == "") {
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("输入为空"));
            return;
        }

        _ShowTable([this](){
            return _dispatcher.NodeInfoQuery(ui->nodeIdEdit->text());
        });
    });

    connect(ui->KPIQuery, &QPushButton::clicked, [this](){
        _ShowGraph([this](){
            return _dispatcher.KPIQuery(ui->NetName_KPI->currentText(),
                                        ui->startDate_KPI->date(), ui->endDate_KPI->date());
        });
    });

    connect(ui->PRBQuery, &QPushButton::clicked, [this](){
        _ShowGraph([this](){
            return _dispatcher.PRBQuery(ui->NetName_PRB->currentText(),
                                        ui->startDate_PRB->date(), ui->endDate_PRB->date());
        });
    });

    connect(ui->importCell, &QPushButton::clicked, [this]() {
        if (!_dispatcher.ImportCell(ui->importCellEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });

    connect(ui->importKPI, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportKPI(ui->importKPIEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });

    connect(ui->importPRB, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportPRB(ui->importPRBEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });

    connect(ui->importMRO, &QPushButton::clicked, [this](){
        if (!_dispatcher.ImportMRO(ui->importMROEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });


    connect(ui->exportTable, &QPushButton::clicked, [this](){
        if (!_dispatcher.ExportLast())
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导出失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });

    connect(ui->sectorQuery_9, &QPushButton::clicked, [this](){
        if (!_dispatcher.ExportTable(ui->tableName->currentText(), ui->importPRBEdit->text()))
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

    ui->count->setText(QString::number(result.size() - 1));
    // 判断是否已经有widget，有则删除
    if(ui->scrollArea->widget() != 0)
        delete ui->scrollArea->widget();

    // 柱状图
    auto barScene = new QGraphicsScene();
    auto barView = new QGraphicsView();
    barView->setScene(barScene);

    barView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    barView->setRenderHint(QPainter::Antialiasing);
    barScene->setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    barView->setSceneRect(10, 10,600, 600);
    barView->setSizeIncrement(600,600);
    //barView->resizeAnchor();
    barView->resize(600,600);
    barView->setFrameStyle(QFrame::NoFrame);



    auto barChart = new QChart();
    barChart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setGeometry(10, 10, 500, 500);
    auto axisX = new QBarCategoryAxis();
    auto axisY = new QValueAxis();

    axisY->setRange(0.0, 0.05);

    auto barSeries = new QBarSeries();
    barChart->addSeries(barSeries);
    barChart->setAxisY(axisY, barSeries);
    barChart->setAxisX(axisX, barSeries);
    bool mark = true;
    //构建 series，作为图表的数据源
    for (int i = 1; i < result.size(); ) {
        auto barSet = new QBarSet(result[i][0]);
        QString pre = result[i][0];
        while (i < result.size() && pre == result[i][0] ) {
            if (mark)
                axisX->append(result[i][1]);
            barSet->append(result[i][2].toFloat());
            qDebug() << result[i][2] << result[i][2].toFloat();
            i++;
        }
        mark = false;
        barSeries->append(barSet)    ;  // 将 series 添加至图表中
    }


    barChart->legend()->setAlignment(Qt::AlignBottom);
    barChart->createDefaultAxes();
    barScene->addItem(barChart);

    ui->scrollArea->setWidget(barView);
}




