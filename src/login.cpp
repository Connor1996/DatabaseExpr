#include "login.h"
#include "ui_login.h"

#include <QMessageBox>
#include <QPainter>
#include <QBrush>

#include "management.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent), window(nullptr),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitConnect();
    ui->loginButton->setShortcut(Qt::Key_Return);//对应键盘上面大的回车键
    setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮

    //setFixedSize(960, 640);
    // 设置背景
    //setAutoFillBackground(true);
}

void Widget::InitConnect()
{

    // 触发登陆
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(Login()));
    connect(ui->cancelButtion, &QPushButton::clicked, [this](){ this->close(); });
}

Widget::~Widget()
{
    delete ui;
    delete window;
}


void Widget::Login()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    if (username == "") {
        QMessageBox::information(this, "info", "username not filled");
    } else if (password == "") {
        QMessageBox::information(this, "info", "password not filled");
    } else if (Dispatcher::ConnectDatabase(username, password)){
        this->close();
        window = new Management();
        window->show();
    } else {
        QMessageBox::information(this, "info", "wrong password");
        this->ui->passwordEdit->clear();
    }
}
