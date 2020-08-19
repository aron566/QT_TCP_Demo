#include "udpclienta.h"
#include "ui_udpclienta.h"

udpClientA::udpClientA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::udpClientA)
{
    ui->setupUi(this);

    //建立套接字
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(8888);

    //新数据接收信号
    connect(udpSocket ,&QUdpSocket::readyRead ,this ,&udpClientA::dealMsg);

    //设置窗口标题
    setWindowTitle("REC port:8888");
}

udpClientA::~udpClientA()
{
    delete ui;
}

/**
 * @brief udpClientA::on_sendpushButton_clicked
 */
void udpClientA::on_sendpushButton_clicked()
{
    if(udpSocket == nullptr)
    {
        return;
    }
    //获取设置的对端ip地址
    QString ip = ui->IPlineEdit->text();
    //获取设置的对端port
    quint16 port = ui->PORTlineEdit->text().toUShort();
    //获取需发送的内容
    QString str = ui->writetextEdit->toPlainText();

    //发送数据
    udpSocket->writeDatagram(str.toUtf8() ,QHostAddress(ip) ,port);

}

/**
 * @brief udpClientA::on_closepushButton_clicked
 */
void udpClientA::on_closepushButton_clicked()
{

}

/**
 * @brief udpClientA::dealMsg slot
 */
void udpClientA::dealMsg()
{
    if(udpSocket == nullptr)
    {
        return;
    }
    char data[1024];
    QHostAddress cliIpAddr;
    quint16 port;
    qint64 len = udpSocket->readDatagram(data ,sizeof(data) ,&cliIpAddr ,&port);
    if(len > 0)
    {
        //格式化显示数据
        QString str = QString("[%1-%2] %3").arg(cliIpAddr.toString()).arg(port).arg(data);
        ui->readtextEdit->append(str);
    }
}

/**
 * @brief udpClientA::on_joinpushButton_clicked
 */
void udpClientA::on_joinpushButton_clicked()
{
    udpSocket->close();

    /*绑定组播网路D类地址且为ipv4，设置端口为8888*/
    udpSocket->bind(QHostAddress::AnyIPv4 ,8888 ,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    udpSocket->joinMulticastGroup(QHostAddress("224.0.0.1"));

    //设置窗口标题
    setWindowTitle("myport:8888 ip:224.0.0.1");
}

/**
 * @brief udpClientA::on_leavepushButton_clicked
 */
void udpClientA::on_leavepushButton_clicked()
{
    udpSocket->leaveMulticastGroup(QHostAddress("224.0.0.1"));
}
