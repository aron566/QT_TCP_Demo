#include "fileclient.h"
#include "ui_fileclient.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QDebug>
/**
 * @brief fileclient::fileclient
 * @param parent
 */
fileclient::fileclient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileclient)
{
    ui->setupUi(this);
    //设置客户端标题
    setWindowTitle("文件客户端");

    TcpSocket = new QTcpSocket(this);/**< 通讯套接字*/

    /*建立文件对象*/
    file = new QFile;

    /*重置进度条*/
    ui->progressBar->setValue(0);

    //连接成功信号处理
    connect(TcpSocket ,&QTcpSocket::connected ,
            [=]()
            {
                ui->textEdit->setText("成功与服务器建立连接!");
            });
    //读取数据信号处理
    connect(TcpSocket ,&QTcpSocket::readyRead ,
            [=]()
            {
                /*取出接收的内容*/
                QByteArray buf = TcpSocket->readAll();
                if(isStart == true)
                {
                    qDebug() << "接收到文件头部信息";
                    isStart = false;/**< 接收文件头信息*/
                    //分段               0   0  1  1  2    2
                    //解析头部信息 buf = "hello##1024##333333"
                    //QString str = "hello##1024";
                    //str.section("##",0,0);//这个拆出来hello，其中##是分段标识符，格式为：段1+标识符+段2+标识符+段3+标识符+……
                    filename = QString(buf).section("##" ,0 ,0);
                    filesize = QString(buf).section("##" ,1 ,1).toLongLong();
                    recfilesize = 0;

                    /*打开文件*/
                    file->setFileName(filename);

                    bool isok = file->open(QIODevice::WriteOnly);
                    if(isok == false)
                    {
                        qDebug() << "WriteOnly error 40!";
                    }
                    ui->textEdit->append(QString("本次接收文件：%1").arg(filename));
                    ui->textEdit->append(QString("本次接收文件大小：%1KB").arg(filesize/1024));
                    //设置进度条
                    ui->progressBar->setMinimum(0);//最小值
                    ui->progressBar->setMaximum(filesize/1024);//最大值
                    ui->progressBar->setValue(0);//当前值
                    ui->textEdit->moveCursor(QTextCursor::End);
                    ui->textEdit->append("Rec:");
                }
                else
                {
                    /*接收文件处理*/
                    qint64 len = file->write(buf);
                    if(len > 0)
                    {
                        ui->textEdit->moveCursor(QTextCursor::End);
                        ui->textEdit->insertPlainText(QString("+"));
                        recfilesize += len;//累积接收大小
                    }

                    //更新进度条
                    ui->progressBar->setValue(recfilesize/1024);

                    /*判断是否接收完成*/
                    if(recfilesize == filesize)
                    {
                        /*先给服务发送（接收文件完成的消息）*/
                        len = TcpSocket->write("file done");
                        if(len > 0)
                        {
                            ui->textEdit->moveCursor(QTextCursor::End);
                            ui->textEdit->append("文件已接收完毕!");
                        }
                        /*关闭tcp连接和文件*/
                        file->close();
                        QMessageBox::information(this ,"完成" , "文件接收完成！");

                        /*目前屏蔽可多次选择发送*/
//                        TcpSocket->disconnectFromHost();
//                        TcpSocket->close();
                        isStart = true;
                    }
                }
            });
}

/**
 * @brief fileclient::~fileclient
 */
fileclient::~fileclient()
{
    TcpSocket->disconnectFromHost();
    TcpSocket->close();
    delete ui;
}

/**
 * @brief fileclient::on_connect_pushButton_clicked
 */
void fileclient::on_connect_pushButton_clicked()
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
