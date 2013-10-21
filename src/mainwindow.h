#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserial/qextserialport.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_m_send_btn_clicked();
    void readCom();

    void on_m_open_btn_clicked();

    void on_m_autosend_btn_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *readTimer;
    QTimer *sendTimer;
public:
    void setSerialPort();
    QString getHexStr(QByteArray temp);
};
extern QextSerialPort *myCom;
#endif // MAINWINDOW_H
