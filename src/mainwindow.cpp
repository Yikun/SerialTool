#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readTimer = new QTimer(this);
    connect(readTimer,SIGNAL(timeout()),this,SLOT(readCom()));
    //readTimer->start(10);
    sendTimer = new QTimer(this);
    connect(sendTimer,SIGNAL(timeout()),this,SLOT(on_m_send_btn_clicked()));
    myCom=NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSerialPort()
{


        QString portName ="/dev/"+ui->m_port_cb->currentText();   //获取串口名
        myCom = new QextSerialPort(portName, QextSerialPort::Polling);

        myCom->setBaudRate((BaudRateType)ui->m_baudrate_cb->currentText().toInt());

        //设置数据位
        myCom->setDataBits((DataBitsType)ui->m_data_cb->currentText().toInt());

        //设置校验
        switch(ui->m_jiou_cb->currentIndex()){
        case 0:
             myCom->setParity(PAR_NONE);
             break;
        case 1:
            myCom->setParity(PAR_ODD);
            break;
        case 2:
            myCom->setParity(PAR_EVEN);
            break;
        default:
            myCom->setParity(PAR_NONE);
            qDebug("set to default : PAR_NONE");
            break;
        }

        //设置停止位
            switch(ui->m_stop_cb->currentIndex()){
            case 0:
                myCom->setStopBits(STOP_1);
                break;
            case 1:
                break;
            case 2:
                myCom->setStopBits(STOP_2);
                break;
            default:
                myCom->setStopBits(STOP_1);
                qDebug("set to default : STOP_1");
                break;
            }

        myCom->setFlowControl(FLOW_OFF);

        myCom->setTimeout(10);
}

void MainWindow::on_m_send_btn_clicked()
{
    QString str=ui->m_send_cb->currentText();
    QByteArray outData=str.toAscii();
    QDateTime m_time = QDateTime::currentDateTime();
    int size=outData.size();

    //如果串口没有打开
    if (!myCom->isOpen())
    {
        return;
    }
    //如果发送数据为空
    if (outData.isEmpty())
    {
        ui->m_send_cb->setFocus();
        return;
    }

    //linux下的回车符为一个字符\r
    if (outData[size-1]!='\r')
    {
        outData.resize(size+1);
        outData[size]='\r';
    }

    //默认16进制发送不加入回车符
    if (ui->m_hexsend_check->isChecked())//转化为16进制发送
    {
        outData=myCom->GetHexValue(str);
        size=outData.size();
        myCom->write(outData);
        QByteArray temp=outData;
        QString strHex;//16进制数据
        QDataStream out(&temp,QIODevice::ReadWrite);
        while (!out.atEnd())
        {
            qint8 outChar=0;
            out>>outChar;
            QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

            if (str.length()>1)
            {
                strHex+=str+" ";
            }
            else
            {
                strHex+="0"+str+" ";
            }
        }
        if(!ui->m_stopshowsend_check->isChecked())
            ui->m_recv_text->append(m_time.toString(tr("[yyyy-MM-dd hh:mm:ss][发送]"))+"[HEX]"+strHex.toUpper());
    }
    else
    {
        size=outData.size();
        myCom->write(outData);
        if(!ui->m_stopshowsend_check->isChecked())
            ui->m_recv_text->append(m_time.toString(tr("[yyyy-MM-dd hh:mm:ss][发送]"))+"[ASCII]"+outData);
    }
}

void MainWindow::readCom()
{
    QDateTime m_time = QDateTime::currentDateTime();
    QString strHex;//16进制数据
    QString strNomal;//正常数据
    if(ui->m_stopshow_check->isChecked())
    {

    }
    else
    {
    if(myCom)
    {
        if( myCom->bytesAvailable() >= 18 )
        {
            QByteArray temp=myCom->readAll();
            if (!temp.isEmpty() /*&& IsShow*/)
            {

                strHex=getHexStr(temp);
                strNomal=temp;

                    if (ui->m_hexshow_check->isChecked())
                    {
                        ui->m_recv_text->append(m_time.toString(tr("[yyyy-MM-dd hh:mm:ss][接收]"))+"[Hex]"+strHex.toUpper());
                    }
                    else
                    {
                        ui->m_recv_text->append(m_time.toString(tr("[yyyy-MM-dd hh:mm:ss][接收]"))+"[ASCII]"+strNomal);
                    }

                qDebug()<<"Check Data Dialog:"<<m_time.toString("[yyyy-MM-dd hh:mm:ss][接收]")<<strHex;
            }
        }
    }
    else
    {
        qDebug()<<"Com is null.";
    }
    }
}


void MainWindow::on_m_open_btn_clicked()
{
    setSerialPort();
    if(ui->m_open_btn->text()=="Open")
    {
        if(myCom->open(QIODevice::ReadWrite)){

            ui->m_open_btn->setText("Close");
            ui->m_autosend_btn->setDisabled(false);
            ui->m_send_btn->setDisabled(false);

            ui->m_baudrate_cb->setDisabled(true);
            ui->m_data_cb->setDisabled(true);
            ui->m_jiou_cb->setDisabled(true);
            ui->m_port_cb->setDisabled(true);
            ui->m_stop_cb->setDisabled(true);
            readTimer->start(10);

            // 检测一下参数
            qDebug()<<"Port Name:" + myCom->portName();

            qDebug()<<"Baud Rate:" + QString::number(myCom->baudRate(), 10);

            qDebug()<<"Data Bits:" + QString::number(myCom->dataBits(), 10);

            qDebug()<<"Parity :" + QString::number(myCom->parity(), 10);

            qDebug()<<"Stop Bit:" + QString::number(myCom->stopBits(),10);
        }
        else{
            QMessageBox::critical(this, tr("Open Failed"), tr("Can't open ") + myCom->portName() + tr("\nThis port is busy or not exist!"), QMessageBox::Ok);
            return;
        }

    }else
    {
        readTimer->stop();
        myCom->close();
        ui->m_open_btn->setText("Open");
        ui->m_autosend_btn->setDisabled(true);
        ui->m_send_btn->setDisabled(true);
        ui->m_baudrate_cb->setDisabled(false);
        ui->m_data_cb->setDisabled(false);
        ui->m_jiou_cb->setDisabled(false);
        ui->m_port_cb->setDisabled(false);
        ui->m_stop_cb->setDisabled(false);
    }
}

void MainWindow::on_m_autosend_btn_clicked()
{
    if(ui->m_autosend_btn->text()==tr("自动发送"))
    {
        ui->m_autosend_btn->setText(tr("停止"));
        ui->m_time_tex->setDisabled(true);
        sendTimer->start(ui->m_time_tex->text().toInt());
    }else
    {
        ui->m_autosend_btn->setText(tr("自动发送"));
        ui->m_time_tex->setDisabled(false);
        sendTimer->stop();
    }

}

QString MainWindow::getHexStr(QByteArray temp)
{
    QString strHex;
    QDataStream out(&temp,QIODevice::ReadWrite);
    while (!out.atEnd())
    {
        qint8 outChar=0;
        out>>outChar;
        QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

        if (str.length()>1)
        {
            strHex+=str+" ";
        }
        else
        {
            strHex+="0"+str+" ";
        }
    }
    return strHex;
}
