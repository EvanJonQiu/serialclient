#include "serialclient.h"
#include "./ui_serialclient.h"

#include <QSerialPortInfo>
#include <QMessageBox>

SerialClient::SerialClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialClient), serialPort(NULL)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->cbPort->addItem(info.portName());
    }

    ui->cbBaudRate->addItem("4800", QSerialPort::Baud4800);
    ui->cbBaudRate->addItem("9600", QSerialPort::Baud9600);
    ui->cbBaudRate->addItem("19200", QSerialPort::Baud19200);
    ui->cbBaudRate->addItem("115200", QSerialPort::Baud115200);
    ui->cbBaudRate->setCurrentIndex(3);

    ui->cbCharacterSize->addItem("5", QSerialPort::Data5);
    ui->cbCharacterSize->addItem("6", QSerialPort::Data6);
    ui->cbCharacterSize->addItem("7", QSerialPort::Data7);
    ui->cbCharacterSize->addItem("8", QSerialPort::Data8);
    ui->cbCharacterSize->setCurrentIndex(3);

    ui->cbStopBits->addItem("1", QSerialPort::OneStop);
    ui->cbStopBits->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->cbStopBits->addItem("2", QSerialPort::TwoStop);
    ui->cbStopBits->setCurrentIndex(0);

    ui->cbParity->addItem("none", QSerialPort::NoParity);
    ui->cbParity->addItem("even", QSerialPort::EvenParity);
    ui->cbParity->addItem("odd", QSerialPort::OddParity);
    ui->cbParity->addItem("space", QSerialPort::SpaceParity);
    ui->cbParity->addItem("mark", QSerialPort::MarkParity);
    ui->cbParity->setCurrentIndex(0);

    ui->btnSend->setEnabled(false);
    ui->ctrlInput->setDisabled(true);
    ui->ctrlReceiveEdit->setReadOnly(true);
}

SerialClient::~SerialClient()
{
    delete ui;

    if (this->serialPort != NULL) {
        if (this->serialPort->isOpen()) {
            this->serialPort->close();
        }
        delete this->serialPort;
    }
}

void SerialClient::on_btnOpenPort_clicked()
{
    if (ui->btnOpenPort->text().compare("打开") == 0) {
        if (this->serialPort == NULL) {
            this->serialPort = new QSerialPort(this);
            this->serialPort->setPortName(ui->cbPort->currentText());

            this->serialPort->setBaudRate(ui->cbBaudRate->itemData(ui->cbBaudRate->currentIndex()).value<QSerialPort::BaudRate>());
            this->serialPort->setDataBits(ui->cbCharacterSize->itemData(ui->cbCharacterSize->currentIndex()).value<QSerialPort::DataBits>());
            this->serialPort->setStopBits(ui->cbStopBits->itemData(ui->cbStopBits->currentIndex()).value<QSerialPort::StopBits>());
            this->serialPort->setParity(ui->cbParity->itemData(ui->cbParity->currentIndex()).value<QSerialPort::Parity>());

            if (this->serialPort->open(QIODevice::ReadWrite)) {
                QMessageBox::information(this, "提示", "成功");
                ui->btnOpenPort->setText("关闭");
                this->disableUi(true);

                connect(this->serialPort, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
            } else {
                QMessageBox::critical(this, "提示", "失败");
            }
        }
    } else {
        if (this->serialPort != NULL) {
            if (this->serialPort->isOpen()) {
                this->serialPort->close();
            }
            delete this->serialPort;
            this->serialPort = NULL;
            ui->btnOpenPort->setText("打开");
            this->disableUi(false);
        }
    }
}

void SerialClient::receiveMessage() {
    QString buf = this->serialPort->readAll();
    ui->ctrlReceiveEdit->appendPlainText(buf);
}

void SerialClient::disableUi(bool bFlag) {
    ui->cbPort->setEnabled(!bFlag);
    ui->cbBaudRate->setEnabled(!bFlag);
    ui->cbCharacterSize->setEnabled(!bFlag);
    ui->cbStopBits->setEnabled(!bFlag);
    ui->cbParity->setEnabled(!bFlag);

    ui->ctrlInput->setDisabled(!bFlag);
    if (!bFlag) {
        ui->ctrlInput->setText("");
    }
    ui->btnSend->setEnabled(bFlag);
}


void SerialClient::on_cleanOutput_clicked()
{
    ui->ctrlReceiveEdit->clear();
}


void SerialClient::on_btnSend_clicked()
{
    if (this->serialPort != NULL && this->serialPort->isOpen()) {
        QString cmd = ui->ctrlInput->text();
        cmd.append("\r\n");
        this->serialPort->write(cmd.toStdString().c_str());
    }
}

