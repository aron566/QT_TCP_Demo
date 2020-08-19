#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class fileclient;
}

class fileclient : public QWidget
{
    Q_OBJECT

public:
    explicit fileclient(QWidget *parent = nullptr);
    ~fileclient();

private slots:
    void on_connect_pushButton_clicked();

private:
    Ui::fileclient *ui;
    QTcpSocket *TcpSocket;
    QFile *file;        /**< 文件对象*/
    QString filename;   /**< 打开的文件名称*/
    qint64 filesize;    /**< 文件大小信息*/
    qint64 recfilesize; /**< 已接收文件大小*/
    bool isStart = true;
};

#endif // FILECLIENT_H
