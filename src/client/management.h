#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "client.h"


#include <QWidget>
#include <QLabel>

#include <unordered_map>


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


    Ui::Management *ui;

    // server socket对象
    Connor_Socket::Client *_client;

};

#endif // MANAGEMENT_H
