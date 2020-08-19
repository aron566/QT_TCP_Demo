#include "fileserver.h"
#include "ui_fileserver.h"
#include <QFileDialog>/**< 打开文件对话框*/
#include <QFileInfo>/**< 获取文件信息*/
#include <QDebug>
/**
 * @brief fileserver::fileserver
 * @param parent
 */
fileserver::fileserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileserver)
{
    ui->setupUi(this);

    /*初始按钮不可用状态*/
    ui->filepushButton->setEnabled(false);
    ui->sendpushButton->setEnabled(false);

    /*监听套接字*/
    TcpServer = new QTcpServer(this);

    /*监听*/
    TcpServer->listen(QHostAddress::Any ,8888);

    //设置窗口标题
    setWindowTitle("文件服务器端口：8888");

    /*如果成功连接触发信号 newconnection*/
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
                ui->textEdit->setText(temp);

                //设置按钮可用
                ui->filepushButton->setEnabled(true);
                ui->sendpushButton->setEnabled(true);

                //服务器端连接数据接收信号
                connect(TcpSocket ,&QTcpSocket::readyRead ,
                        [=]()
                        {
                            //从通讯套接字中，取出数据
                            QByteArray array = TcpSocket->readAll();
                            //追加显示
                            ui->textEdit->append("Rec:"+array+".");

                            if(QString(array) == "file done")
                            {
                                //文件接收完毕
                                ui->textEdit->append("对方文件接收完毕.");
                                file.close();
                                //断开客户端端口,目前屏蔽可多次选择发送
//                                TcpSocket->disconnectFromHost();
//                                TcpSocket->close();
                            }
                        });

            });

    connect(&myTimer ,&QTimer::timeout ,
            [=]()
    {
        /*先停止定时器*/
        myTimer.stop();

        /*发送文件*/
        SendData();
    });
}

/**
 * @brief fileserver::~fileserver
 */
fileserver::~fileserver()
{
    TcpSocket->disconnectFromHost();
    TcpSocket->close();
    delete ui;
}

/**
 * @brief fileserver::SendData
 */
void fileserver::SendData()
{
    qint64 len = 0;
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->append("Send:");
    do
    {
        /*每次发送文件的大小*/
        char buf[4*1024] = {0};
        len = 0;

        len = file.read(buf ,sizeof(buf));
        if(len > 0)
        {
            ui->textEdit->moveCursor(QTextCursor::End);
            ui->textEdit->insertPlainText(QString("+"));
        }

        /*发送数据，读多少发多少*/
        len = TcpSocket->write(buf ,len);

        /*发送数据累积*/
        sendfilesize += len;

    }while(len > 0);

    /*判断是否发送完毕*/
    if(sendfilesize == filesize)
    {
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->append("文件已发送完毕!");
        file.close();

        /*断开客户端*/
//        TcpSocket->disconnectFromHost();
//        TcpSocket->close();
        ui->filepushButton->setEnabled(true);
    }
}


/**
 * @brief fileserver::on_filepushButton_clicked
 */
void fileserver::on_filepushButton_clicked()
{
    /*选择文件*/
    QString filepath = QFileDialog::getOpenFileName(this ,"open" ,"../");
    if(filepath.isEmpty() == false)
    {
        filename.clear();
        filesize = 0;
        /*获取文件信息*/
        QFileInfo info(filepath);
        filename = info.fileName();
        filesize = info.size();
        qDebug() << "path:" << filepath << "filename" << filename << "size" << filesize;
        /*重置已发送大小为0*/
        sendfilesize = 0;

        /*只读方式打开文件*/
        file.setFileName(filepath);
        bool isok = file.open(QIODevice::ReadOnly);

        if(isok == false)
        {
            qDebug() << "只读打开文件失败！78";
            return;
        }

        /*提示打开文件的路径*/
        ui->textEdit->append("待发送文件："+filepath);

        /*设置打开文件按钮不可用状态*/
        ui->filepushButton->setEnabled(false);
        ui->sendpushButton->setEnabled(true);
    }
    else
    {
        qDebug() << "打开文件错误！62";
    }
}

/**
 * @brief fileserver::on_sendpushButton_clicked
 */
void fileserver::on_sendpushButton_clicked()
{
    /*发送文件*/
    /*1、先发送文件头部信息*/
    QString head = QString("%1##%2").arg(filename).arg(filesize);
    //发送
    qint64 len = TcpSocket->write(head.toUtf8());
    if(len > 0)
    {
        qDebug() << "头部发送成功，wait ...";
        /*启动定时器稍加延时20ms，防止粘包*/
        myTimer.start(20);
    }
    else
    {
        qDebug() << "文件发送失败！";
        file.close();
        /*发送失败重置选择按钮，禁止发送按钮*/
        ui->filepushButton->setEnabled(true);
        ui->sendpushButton->setEnabled(false);
    }

}
