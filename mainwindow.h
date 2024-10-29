// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "library/firmwareuart.h"

namespace Ui {
class MainWindow;
}

// MainWindow class for managing UI and UART interactions
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleConnectDisconnect();                 // Handle connect/disconnect button
    void updateTransmissionSpeed(double speedBps);  // Update UI with transmission speed
    void updateReceiveSpeed(double speedBps);       // Update UI with receive speed
    void updateConnectionStatus(bool isConnected);  // Update UI on connection status change
    void clearConsole();                            // Clear the console output
    void appendReceivedData(const QByteArray &data);// Append received data to console

private:
    Ui::MainWindow *ui;
    FirmwareUART *uart;  // UART object for handling serial communication
    QList<QSerialPortInfo> previousPorts;  // Store the last known list of ports
    void updateAvailablePorts();           // Method to refresh and update the combo box
};

#endif // MAINWINDOW_H

