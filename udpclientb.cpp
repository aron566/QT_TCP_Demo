#include "udpclientb.h"
#include "ui_udpclientb.h"

udpClientB::udpClientB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::udpClientB)
{
    ui->setupUi(this);

    //建立套接字
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(9999);

    //新数据接收信号
    connect(udpSocket ,&QUdpSocket::readyRead ,this ,&udpClientB::dealMsg);

    //设置窗口标题
    setWindowTitle("REC port:9999");
}

udpClientB::~udpClientB()
{
    delete ui;
}

/**
 * @brief udpClientB::on_sendpushButton_clicked
 */
void udpClientB::on_sendpushButton_clicked()
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
 * @brief udpClientB::on_closepushButton_clicked
 */
void udpClientB::on_closepushButton_clicked()
{

}

/**
  * @brief udpClientB::dealMsg slot
  */
void udpClientB::dealMsg()
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
 * @brief udpClientB::on_joinpushButton_clicked
 */
void udpClientB::on_joinpushButton_clicked()
{
    udpSocket->close();

    /*绑定组播网路D类地址且为ipv4，设置端口为9999*/
    udpSocket->bind(QHostAddress::AnyIPv4 ,9999 ,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    udpSocket->joinMulticastGroup(QHostAddress("224.0.0.2"));

    //设置窗口标题
    setWindowTitle("myport:9999 ip:224.0.0.2");
}

/**
 * @brief udpClientB::on_leavepushButton_clicked
 */
void udpClientB::on_leavepushButton_clicked()
{
    udpSocket->leaveMulticastGroup(QHostAddress("224.0.0.2"));
}
