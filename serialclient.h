#ifndef SERIALCLIENT_H
#define SERIALCLIENT_H

#include <QWidget>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class SerialClient;
}
QT_END_NAMESPACE

class SerialClient : public QWidget
{
    Q_OBJECT

public:
    SerialClient(QWidget *parent = nullptr);
    ~SerialClient();

private slots:
    void on_btnOpenPort_clicked();
    void receiveMessage();

    void on_cleanOutput_clicked();

    void on_btnSend_clicked();

private:
    void disableUi(bool bFlag);

private:
    Ui::SerialClient *ui;

    QSerialPort *serialPort;
};
#endif // SERIALCLIENT_H
