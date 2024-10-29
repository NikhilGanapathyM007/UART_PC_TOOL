#ifndef FIRMWAREUART_H
#define FIRMWAREUART_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>

class FirmwareUART : public QObject
{
    Q_OBJECT

public:
    explicit FirmwareUART(QObject *parent = nullptr);
    ~FirmwareUART();
    bool isConnected() const;

    bool connectToPort(const QString &portName, int baudRate);
    void disconnectFromPort();
    void sendData();
    bool setupLogFile(const QString &filePath);
    void closeLogFile();
    void setDataToSend(const QString& data);

signals:
    void transmissionSpeedUpdated(double speedBps);
    void receiveSpeedUpdated(double speedBps);
    void dataReceived(QByteArray data);
    void dataSent();
    void connectionStatusChanged(bool isConnected);

private slots:
    void receiveData();

private:
    QSerialPort *serialPort;
    QFile *logFile;
    bool loggingEnabled;
    QString dataToSend;

    static constexpr int PacketSize = 32;
};

#endif // FIRMWAREUART_H
