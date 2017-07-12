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

    // 选择文件路径
    connect(ui->chooseFilePath_Cell, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择导入文件"), ".", "Excel Files(*.xlsx *.xls *.csv)");
        ui->importCellEdit->setText(path);
    });
    connect(ui->chooseFilePath_KPI, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择导入文件"), ".", tr("Excel Files(*.xlsx *.xls *.csv)"));
        ui->importKPIEdit->setText(path);
    });
    connect(ui->chooseFilePath_PRB, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择导入文件"), ".", tr("Excel Files(*.xlsx *.xls *.csv)"));
        ui->importPRBEdit->setText(path);
    });
    connect(ui->chooseFilePath_MRO, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择导入文件"), ".", tr("Excel Files(*.xlsx *.xls *.csv)"));
        ui->importMROEdit->setText(path);
    });
    connect(ui->chooseExportPath, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("选择导出文件"), ".", tr("Excel Files(*.xlsx *.xls *.csv)"));
        ui->exportFilePathEdit->setText(path);
    });    
    connect(ui->exportResult, &QPushButton::clicked, [this](){
        QString path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("选择导出文件"), ".", tr("Excel Files(*.xlsx *.xls)"));
        if (path.length() == 0)
            return;

        if (!_dispatcher.ExportLast(path))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导出失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    /////////////////////////////////////////////

    // 查询按钮
    connect(ui->sectorQuery, &QPushButton::clicked, [this](){
        if (ui->sectorIdEdit->text() != "") {
            _ShowTable([this](){
                return _dispatcher.SectorIdQuery(ui->sectorIdEdit->text());
            });
        } else if (ui->sectorNameEdit->text() != "") {
            _ShowTable([this](){
                return _dispatcher.SectorNameQuery(ui->sectorNameEdit->text());
            });
        } else {
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("输入为空"));
        }
    });
    connect(ui->sectorNameChoose, &QComboBox::currentTextChanged, [this](){
        ui->sectorNameEdit->setText(ui->sectorNameChoose->currentText());
    });
    connect(ui->eNodeQuery, &QPushButton::clicked, [this](){
        if (ui->nodeIdEdit->text() != "") {
            _ShowTable([this](){
                return _dispatcher.NodeIdQuery(ui->nodeIdEdit->text());
            });
        } else if (ui->nodeNameEdit->text() != "") {
            _ShowTable([this](){
                return _dispatcher.NodeNameQuery(ui->nodeNameEdit->text());
            });
        } else {
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("输入为空"));
        }
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
    ///////////////////////////////////////////

    // 导入按钮
    connect(ui->importCell, &QPushButton::clicked, [this]() {
        if (ui->importCellEdit->text() == ""){
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("文件路径为空"));
            return;
        }
        if (!_dispatcher.ImportCell(ui->importCellEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    connect(ui->importKPI, &QPushButton::clicked, [this](){
        if (ui->importKPIEdit->text() == ""){
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("文件路径为空"));
            return;
        }

        if (!_dispatcher.ImportKPI(ui->importKPIEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    connect(ui->importPRB, &QPushButton::clicked, [this](){
        if (ui->importPRBEdit->text() == ""){
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("文件路径为空"));
            return;
        }

        if (!_dispatcher.ImportPRB(ui->importPRBEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    connect(ui->importMRO, &QPushButton::clicked, [this](){
        if (ui->importMROEdit->text() == ""){
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("文件路径为空"));
            return;
        }

        if (!_dispatcher.ImportMRO(ui->importMROEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导入失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    ///////////////////////////////////////////

    // 导出按钮
    connect(ui->exportTable, &QPushButton::clicked, [this](){
        if (ui->importPRBEdit->text() == ""){
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("文件路径为空"));
            return;
        }

        if (!_dispatcher.ExportTable(ui->tableName->currentText(), ui->importPRBEdit->text()))
            QMessageBox::warning(this, "error", QString::fromLocal8Bit("导出失败"));
        else
            QMessageBox::information(this, "info", QString::fromLocal8Bit("导入成功"));
    });
    ////////////////////////////////////////////

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

    auto barSeries = new QBarSeries();

    auto axisX = new QBarCategoryAxis();

    bool mark = true;
    //构建 series，作为图表的数据源
    for (int i = 1; i < result.size(); ) {
        auto barSet = new QBarSet(result[i][0]);
        QString pre = result[i][0];
        while (i < result.size() && pre == result[i][0] ) {
            if (mark)
                axisX->append(result[i][1].left(10));
            barSet->append(result[i][2].toFloat());
            qDebug() << result[i][0] << result[i][1] << result[i][2];
            i++;
        }
        mark = false;
        barSeries->append(barSet)    ;  // 将 series 添加至图表中
    }

    auto barChart = new QChart();
    barChart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setGeometry(0, 0, 600, 600);
    barChart->addSeries(barSeries);

    barChart->createDefaultAxes();
    barChart->setAxisX(axisX, barSeries);

    barChart->legend()->setAlignment(Qt::AlignBottom);
    barScene->addItem(barChart);

    ui->scrollArea->setWidget(barView);
}




