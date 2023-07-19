#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>


QSerialPort serial;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //read avaible comports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            ui->comboBox->addItem(info.portName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

//button send
void MainWindow::on_pushButton_clicked()
{
    //string for lineedit text
    QString str2 = ui->lineEdit->text();

    serial.write(str2.toUtf8());

}

//connect
void MainWindow::on_pushButton_2_clicked()
{
    //select name for our serial port from combobox
    if (serial.portName() != ui->comboBox->currentText())
    {
          serial.close();
          serial.setPortName(ui->comboBox->currentText());
    }

    //setup COM port
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QSerialPort::WriteOnly);

}

void MainWindow::on_pushButton_3_clicked()
{
    //close
    serial.close();
}
