#include "management.h"
#include "ui_management.h"


#include "src/include/json.hpp"
#include "src/protocol.h"

#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsView>

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

}









