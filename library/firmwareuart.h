#ifndef FIRMWAREUART_H
#define FIRMWAREUART_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>

// The FirmwareUART class handles UART communication, logging, and data transmission
class FirmwareUART : public QObject
{
    Q_OBJECT

public:
    explicit FirmwareUART(QObject *parent = nullptr);
    ~FirmwareUART();
    bool isConnected() const;

    // Connection management
    bool connectToPort(const QString &portName, int baudRate);
    void disconnectFromPort();

    // Data and logging functions
    void sendData();
    bool setupLogFile(const QString &filePath);
    void closeLogFile();
    void setDataToSend(const QString& data);

signals:
    // Signals for updating UI with speed and connection status
    void transmissionSpeedUpdated(double speedBps);
    void receiveSpeedUpdated(double speedBps);
    void dataReceived(QByteArray data);
    void dataSent();
    void connectionStatusChanged(bool isConnected);

private slots:
    void receiveData(); // Slot for receiving incoming data

private:
    QSerialPort *serialPort;
    QFile *logFile;
    bool loggingEnabled;
    QString dataToSend;

    static constexpr int PacketSize = 32; // Size of data packets to be sent
};

#endif // FIRMWAREUART_H
