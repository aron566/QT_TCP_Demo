#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>/**< 通讯套接字*/
#include <QHostAddress>

namespace Ui {
class clientWidget;
}

class clientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientWidget(QWidget *parent = nullptr);
    ~clientWidget();

private slots:
    void on_send_pushButton_clicked();

    void on_close_pushButton_clicked();

    void on_connect_pushButton_clicked();

private:
    Ui::clientWidget *ui;

    QTcpSocket *TcpSocket = nullptr;/**< 通讯套接字*/
};

#endif // CLIENTWIDGET_H
