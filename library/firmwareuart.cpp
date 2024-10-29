#include "library/FirmwareUART.h"
#include <QElapsedTimer>
#include <QTextStream>
#include <QDebug>

FirmwareUART::FirmwareUART(QObject *parent)
    : QObject(parent), serialPort(new QSerialPort(this)), logFile(nullptr), loggingEnabled(false) {}

FirmwareUART::~FirmwareUART() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    closeLogFile();
}

bool FirmwareUART::connectToPort(const QString &portName, int baudRate) {
    // Close the port if already open
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    // Set up port parameters
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    // Try to open the port and connect signal for receiving data
    if (serialPort->open(QIODevice::ReadWrite)) {
        connect(serialPort, &QSerialPort::readyRead, this, &FirmwareUART::receiveData);
        emit connectionStatusChanged(true);
        sendData(); // Send initial data upon connection
        return true;
    } else {
        emit connectionStatusChanged(false);
        return false;
    }
}

void FirmwareUART::disconnectFromPort() {
    // Close the port and update connection status
    if (serialPort->isOpen()) {
        serialPort->close();
        emit connectionStatusChanged(false);
    }
}

bool FirmwareUART::isConnected() const {
    return serialPort && serialPort->isOpen();
}

void FirmwareUART::sendData() {
    // Check if serial port is open
    if (!serialPort || !serialPort->isOpen()) {
        return;
    }

    QByteArray data = dataToSend.toUtf8();
    QElapsedTimer timer;
    qint64 totalBytesSent = 0;

    for (int i = 0; i < data.size(); i += PacketSize) {
        QByteArray packet = data.mid(i, PacketSize);
        timer.restart();

        qint64 bytesSent = serialPort->write(packet);
        serialPort->flush();
        serialPort->waitForBytesWritten();
        totalBytesSent += bytesSent;

        // Calculate speed in bits per second
        qint64 elapsedUs = timer.elapsed() * 1000;  // Convert to microseconds
        double speedBps = (elapsedUs > 0) ? (bytesSent * 8 * 1e6) / elapsedUs : 0.0;
        emit transmissionSpeedUpdated(speedBps);

        // Emit data and speed to UI
        QString message = QString("Sent data: %1 - Speed: %2 bps").arg(QString(packet), QString::number(speedBps, 'f', 2));
        emit dataReceived(message.toUtf8());

        // Log if logging is enabled
        if (loggingEnabled && logFile && logFile->isOpen()) {
            QTextStream out(logFile);
            out << message << "\n";
        }
    }

    emit dataSent();
}

void FirmwareUART::receiveData() {
    static QElapsedTimer timer;  // Timer for measuring speed
    if (!timer.isValid()) {
        timer.start();  // Start the timer if not already running
    }

    QByteArray receivedData = serialPort->readAll();
    qint64 bytesReceived = receivedData.size();

    if (bytesReceived > 0) {
        // Calculate receive speed
        qint64 elapsedUs = timer.elapsed() * 1000;  // Elapsed time in microseconds
        double speedBps = (elapsedUs > 0) ? (bytesReceived * 8 * 1e6) / elapsedUs : 0.0;

        emit receiveSpeedUpdated(speedBps);

        QString message = QString("Received data: %1 - Speed: %2 bps").arg(QString(receivedData), QString::number(speedBps, 'f', 2));

        // Print to console
        emit dataReceived(message.toUtf8());

        // Log received data if logging is enabled
        if (loggingEnabled && logFile && logFile->isOpen()) {
            QTextStream out(logFile);
            out << message << "\n";
        }

        // Reset timer for next chunk
        timer.restart();
    }
}

bool FirmwareUART::setupLogFile(const QString &filePath) {
    closeLogFile(); // Close any existing log file
    logFile = new QFile(filePath);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        loggingEnabled = true;
        return true;
    } else {
        delete logFile;
        logFile = nullptr;
        loggingEnabled = false;  // Make sure logging is disabled if the file didn't open
        return false;
    }
}

// Close and delete the log file if it exists
void FirmwareUART::closeLogFile() {
    if (logFile && logFile->isOpen()) {
        logFile->close();
    }
    delete logFile;
    logFile = nullptr;
    loggingEnabled = false;
}

void FirmwareUART::setDataToSend(const QString& data) {
    dataToSend = data;
}


