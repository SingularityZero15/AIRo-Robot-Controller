#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSerialPort *serial;

    void SerialPortInit();
    void RefreshSerialPort(int index);

    void DataReceived();
    void DataSend();

private slots:
    void on_PortBox_clicked();

    void on_refreshButton_clicked();

    void on_openSerialButton_clicked();

    void on_SendButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
