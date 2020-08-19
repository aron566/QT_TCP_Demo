#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>

/**
 * @brief clientWidget::clientWidget
 * @param parent
 */
clientWidget::clientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientWidget)
{
    ui->setupUi(this);

    //设置客户端标题
    setWindowTitle("TCP客户端");

    TcpSocket = new QTcpSocket(this);/**< 通讯套接字*/

    //连接成功信号处理
    connect(TcpSocket ,&QTcpSocket::connected ,
            [=]()
            {
                ui->read_textEdit->setText("成功与服务器建立连接!");
            });

    //读取数据信号处理
    connect(TcpSocket ,&QTcpSocket::readyRead ,
            [=]()
            {
                ui->read_textEdit->append(TcpSocket->readAll());
            });
}

/**
 * @brief clientWidget::~clientWidget
 */
clientWidget::~clientWidget()
{
    delete ui;
}

/**
 * @brief clientWidget::on_send_pushButton_clicked
 */
void clientWidget::on_send_pushButton_clicked()
{
    if(TcpSocket == nullptr)
    {
        return;
    }

    //获取需发送的内容
    QString str = ui->write_textEdit->toPlainText();

    //发送
    TcpSocket->write(str.toUtf8().data());
}

/**
 * @brief clientWidget::on_close_pushButton_clicked
 */
void clientWidget::on_close_pushButton_clicked()
{
    if(TcpSocket == nullptr)
    {
        return;
    }

    //主动断开连接
    TcpSocket->disconnectFromHost();
    TcpSocket->close();

    //清空显示区域
    ui->read_textEdit->setText("");
}

/**
 * @brief clientWidget::on_connect_pushButton_clicked
 */
void clientWidget::on_connect_pushButton_clicked()
{
    if(TcpSocket == nullptr)
    {
        return;
    }

    //获取输入的IP
    QString ip = ui->ip_lineEdit->text();

    //获取输入的端口号
    quint16 port = ui->port_lineEdit->text().toUShort();

    //主动和服务器建立连接
    TcpSocket->connectToHost(QHostAddress(ip) ,port);
}
