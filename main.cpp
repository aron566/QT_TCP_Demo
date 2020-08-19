#include "serverwidget.h"
#include "clientwidget.h"
#include "udpclienta.h"
#include "udpclientb.h"
#include "fileclient.h"
#include "fileserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    /*TCP服务端显示*/
//    ServerWidget w;
//    w.show();

//    /*TCP客户端显示*/
//    clientWidget clientwin;
//    clientwin.show();

    /*udp窗口A显示*/
//    udpClientA udpwina;
//    udpwina.show();

//    /*udp窗口B显示*/
//    udpClientB udpwinb;
//    udpwinb.show();

    /*文件客户端*/
    fileclient filecliwin;
    filecliwin.show();

    /*文件服务器*/
    fileserver fileserwin;
    fileserwin.show();

    return a.exec();
}
