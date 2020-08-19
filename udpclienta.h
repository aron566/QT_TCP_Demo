#ifndef UDPCLIENTA_H
#define UDPCLIENTA_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>

namespace Ui {
class udpClientA;
}

class udpClientA : public QWidget
{
    Q_OBJECT

public:
    explicit udpClientA(QWidget *parent = nullptr);
    ~udpClientA();

private slots:
    void on_sendpushButton_clicked();

    void on_closepushButton_clicked();

//    void on_connectpushButton_clicked();/**< udp无需连接*/

    void dealMsg();/**< 处理接收到的消息*/

    void on_joinpushButton_clicked();

    void on_leavepushButton_clicked();

private:
    Ui::udpClientA *ui;

    QUdpSocket *udpSocket = nullptr;
};

#endif // UDPCLIENTA_H
