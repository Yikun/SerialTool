#include <QtGui/QApplication>
#include "mainwindow.h"
#include "qextserial/qextserialport.h"
#include <QTextCodec>
QextSerialPort *myCom;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    return a.exec();
}
