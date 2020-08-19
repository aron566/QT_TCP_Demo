#ifndef FILESERVER_H
#define FILESERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
namespace Ui {
class fileserver;
}

class fileserver : public QWidget
{
    Q_OBJECT

public:
    explicit fileserver(QWidget *parent = nullptr);
    ~fileserver();

private slots:
    void on_filepushButton_clicked();

    void on_sendpushButton_clicked();
private:
    void SendData();
private:
    Ui::fileserver *ui;
    QTimer myTimer;
    QTcpServer *TcpServer;  /**< 监听套接字*/
    QTcpSocket *TcpSocket;
    QFile file;             /**< 文件对象*/
    QString filename;       /**< 打开的文件名称*/
    qint64 filesize;        /**< 文件大小信息*/
    qint64 sendfilesize;    /**< 已发送文件大小*/
};

#endif // FILESERVER_H
