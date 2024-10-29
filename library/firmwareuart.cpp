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
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connect(serialPort, &QSerialPort::readyRead, this, &FirmwareUART::receiveData);
        emit connectionStatusChanged(true);
        sendData();
        return true;
    } else {
        emit connectionStatusChanged(false);
        return false;
    }
}

void FirmwareUART::disconnectFromPort() {
    if (serialPort->isOpen()) {
        serialPort->close();
        emit connectionStatusChanged(false);
    }
}

bool FirmwareUART::isConnected() const {
    return serialPort && serialPort->isOpen();
}

void FirmwareUART::sendData() {
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

        qint64 elapsedUs = timer.elapsed() * 1000;  // Convert to microseconds
        double speedBps = (elapsedUs > 0) ? (bytesSent * 8 * 1e6) / elapsedUs : 0.0;
        emit transmissionSpeedUpdated(speedBps);

        QString message = QString("Sent data: %1 - Speed: %2 bps").arg(QString(packet), QString::number(speedBps, 'f', 2));

        // Print to console
        emit dataReceived(message.toUtf8());

        // Log if enabled
        if (loggingEnabled && logFile && logFile->isOpen()) {
            QTextStream out(logFile);
            out << message << "\n";
        }
    }

    emit dataSent();
}

/*void FirmwareUART::receiveData() {
    QByteArray receivedData;
    QElapsedTimer timer;
    timer.start();
    qint64 totalBytesReceived = 0;

    while (serialPort->waitForReadyRead(100)) {
        QByteArray chunk = serialPort->readAll();
        receivedData.append(chunk);
        totalBytesReceived += chunk.size();

        qint64 elapsedUs = timer.elapsed() * 1000;  // Convert to microseconds
        double speedBps = (elapsedUs > 0) ? (totalBytesReceived * 8 * 1e6) / elapsedUs : 0.0;
        emit receiveSpeedUpdated(speedBps);

        // Console output
        qDebug() << "Received data:" << QString(chunk) << "- Speed:" << speedBps << "bps";

        // Log if enabled
        if (loggingEnabled && logFile && logFile->isOpen()) {
            QTextStream out(logFile);
            out << "Received data: " << QString(chunk) << " - Speed: " << speedBps << " bps\n";
        }

        timer.restart();
    }

    emit dataReceived(receivedData);
}*/

void FirmwareUART::receiveData() {
    static QElapsedTimer timer;  // Static to keep time between multiple data chunks
    if (!timer.isValid()) {
        timer.start();  // Start the timer on the first call
    }

    QByteArray receivedData = serialPort->readAll();
    qint64 bytesReceived = receivedData.size();

    if (bytesReceived > 0) {
        qint64 elapsedUs = timer.elapsed() * 1000;  // Elapsed time in microseconds
        double speedBps = (elapsedUs > 0) ? (bytesReceived * 8 * 1e6) / elapsedUs : 0.0;

        emit receiveSpeedUpdated(speedBps);

        QString message = QString("Received data: %1 - Speed: %2 bps").arg(QString(receivedData), QString::number(speedBps, 'f', 2));

        // Print to console
        emit dataReceived(message.toUtf8());

        // Log if enabled
        if (loggingEnabled && logFile && logFile->isOpen()) {
            QTextStream out(logFile);
            out << message << "\n";
        }

        // Reset timer for next chunk
        timer.restart();
    }
}

bool FirmwareUART::setupLogFile(const QString &filePath) {
    closeLogFile();
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


