// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "library/firmwareuart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleConnectDisconnect();
    void updateTransmissionSpeed(double speedBps);
    void updateReceiveSpeed(double speedBps);
    void updateConnectionStatus(bool isConnected);
    void clearConsole();
    void appendReceivedData(const QByteArray &data);

private:
    Ui::MainWindow *ui;
    FirmwareUART *uart;  // UART object
    QList<QSerialPortInfo> previousPorts;  // Store the last known list of ports
    void updateAvailablePorts();           // Method to refresh and update the combo box
};

#endif // MAINWINDOW_H

