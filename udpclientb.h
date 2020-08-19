#ifndef UDPCLIENTB_H
#define UDPCLIENTB_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>

namespace Ui {
class udpClientB;
}

class udpClientB : public QWidget
{
    Q_OBJECT

public:
    explicit udpClientB(QWidget *parent = nullptr);
    ~udpClientB();

private slots:
    void on_sendpushButton_clicked();

    void on_closepushButton_clicked();

//    void on_connectpushButton_clicked();/**< udp无需连接*/

    void dealMsg();/**< 处理接收到的消息*/

    void on_joinpushButton_clicked();

    void on_leavepushButton_clicked();

private:
    Ui::udpClientB *ui;

    QUdpSocket *udpSocket = nullptr;
};

#endif // UDPCLIENTB_H
