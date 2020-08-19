#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QDebug>
#include <QMessageBox>

/**
 * @brief ServerWidget::ServerWidget
 * @param parent
 */
ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    //设置窗口标题
    setWindowTitle("服务器端口：8888");

    //监听套接字，指定父对象自动回收空间
    TcpServer = new QTcpServer(this);

    //指定监听端口
    TcpServer->listen(QHostAddress::Any ,8888);

    connect(TcpServer ,&QTcpServer::newConnection ,
            [=]()
            {
                //新的连接产生，获取Tcp连接套接字
                TcpSocket = TcpServer->nextPendingConnection();

                //获取对方的IP地址
                QString IP = TcpSocket->peerAddress().toString();

                //获取对方端口
                quint16 Port = TcpSocket->peerPort();

                QString temp = QString("[%1-%2]:成功连接").arg(IP).arg(Port);

                //显示连接信息
                ui->readtextEdit->setText(temp);

                //服务器端连接数据接收信号
                connect(TcpSocket ,&QTcpSocket::readyRead ,
                        [=]()
                        {
                            //从通讯套接字中，取出数据
                            QByteArray array = TcpSocket->readAll();
                            //追加显示
                            ui->readtextEdit->append(array);

                        });

//                //断开连接信号处理
//                connect(TcpSocket ,&QTcpSocket::disconnect ,this ,&ServerWidget::);
            });
}

/**
 * @brief ServerWidget::~ServerWidget
 */
ServerWidget::~ServerWidget()
{
    delete ui;
}

/**
 * @brief ServerWidget::on_sendButton_clicked
 */
void ServerWidget::on_sendButton_clicked()
{
    //防止段错误
    if(nullptr == TcpSocket)
    {
        return;
    }
    //获取输入的内容
    QString str = ui->writetextEdit->toPlainText();
    //发送出去
    TcpSocket->write(str.toUtf8().data());
}

/**
 * @brief ServerWidget::on_closeButton_clicked
 */
void ServerWidget::on_closeButton_clicked()
{
    //防止段错误
    if(nullptr == TcpSocket)
    {
        return;
    }
    //主动和客户端断开连接
    TcpSocket->disconnectFromHost();
    TcpSocket->close();
    TcpSocket = nullptr;
}
