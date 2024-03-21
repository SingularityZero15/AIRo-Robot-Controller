#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SerialPortInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PortBox_clicked()
{
    RefreshSerialPort(0);
}

void MainWindow::SerialPortInit()
{
    serial = new QSerialPort;                       //申请内存,并设置父对象

    // 获取计算机中有效的端口号，然后将端口号的名称给端口选择控件
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serial->setPort(info);                      // 在对象中设置串口
        if(serial->open(QIODevice::ReadWrite))      // 以读写方式打开串口
        {
            ui->PortBox->addItem(info.portName());  // 添加计算机中的端口
            serial->close();                        // 关闭
        } else
        {
            qDebug() << "串口打开失败，请重试";
        }
    }

    // 参数配置
    // 波特率
    ui->baudRateBox->addItem("115200");

    // // 校验，校验默认选择无
    // ui->ParityBox->addItem("无");
    // serial->setParity(QSerialPort::NoParity);

    // // 数据位，数据位默认选择8位
    // ui->BitBox->addItem("8");
    // serial->setDataBits(QSerialPort::Data8);

    // // 停止位，停止位默认选择1位
    // ui->StopBox->addItem("1");
    // serial->setStopBits(QSerialPort::OneStop);

    // // 控制流，默认选择无
    // ui->ControlBox->addItem("无");
    // serial->setFlowControl(QSerialPort::NoFlowControl);

    // 刷新串口
    RefreshSerialPort(0);

    // // 信号
    connect(serial,&QSerialPort::readyRead,this,&MainWindow::DataReceived);      // 接收数据
    // connect(ui->SendWordOrder,&QPushButton::clicked,this,&MainWindow::DataSend); // 发送数据
    // connect(ui->SendButton,&QPushButton::clicked,this,&MainWindow::DataSend);    // 发送数据
    // connect(ui->SendEditBtn1,&QPushButton::clicked,this,&MainWindow::DataSend);  // 发送数据
    // connect(ui->SendEditBtn2,&QPushButton::clicked,this,&MainWindow::DataSend);  // 发送数据
    // connect(ui->SendEditBtn3,&QPushButton::clicked,this,&MainWindow::DataSend);  // 发送数据
}

// 刷新串口
void MainWindow::RefreshSerialPort(int index)
{
    QStringList portNameList;                                        // 存储所有串口名
    if(index != 0)
    {
        serial->setPortName(ui->PortBox->currentText());             //设置串口号
    }
    else
    {
        ui->PortBox->clear();                                        //关闭串口号
        // ui->PortBox->addItem("刷新");                                //添加刷新
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //添加新串口
        {
            portNameList.append(info.portName());
        }
        ui->PortBox->addItems(portNameList);
        ui->PortBox->setCurrentIndex(1);                             // 当前串口号为COM1
        serial->setPortName(ui->PortBox->currentText());             //设置串口号
    }
}

void MainWindow::on_refreshButton_clicked()
{
    RefreshSerialPort(0);
}


void MainWindow::on_openSerialButton_clicked()
{
    if(serial->isOpen())
    {
        serial->clear();
        serial->close();

        ui->openSerialButton->setStyleSheet("");
        ui->openSerialButton->setText("打开");
    }
    else
    {
        serial->setPortName(ui->PortBox->currentText());
        serial->setBaudRate(ui->baudRateBox->currentText().toInt());

        if(!serial->open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this,tr("提示"),tr("串口打开失败!"),QMessageBox::Ok);
            return;
        }

        serial->setParity(QSerialPort::NoParity);
        serial->setDataBits(QSerialPort::Data8);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        ui->openSerialButton->setStyleSheet("background-color: rgb(0, 170, 0);\ncolor: rgb(0, 0, 0);");
        ui->openSerialButton->setText("关闭");
    }
}


void MainWindow::DataReceived()
{
    char BUF[512] = {0};                                       // 存储转换类型后的数据
    QByteArray data = serial->readAll();                      // 读取数据

    if(!data.isEmpty())                                 // 接收到数据
    {
        QString str = ui->DataReceived->toPlainText();  // 返回纯文本
        str += tr(data);                         // 数据是一行一行传送的，要保存所有数据
        ui->DataReceived->clear();                      // 清空之前的数据
        ui->DataReceived->appendPlainText(str);                  // 将数据放入控件中
        qDebug() << "str info: " << ui->DataReceived->toPlainText();
    }
}

// 发送数据，write ()
void MainWindow::DataSend()
{
    serial->write(ui->DataSend->toPlainText().toLatin1());      // 串口发送数据
}

void MainWindow::on_SendButton_clicked()
{
    DataSend();
}

