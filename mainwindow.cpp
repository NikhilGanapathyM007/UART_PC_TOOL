// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>

QString dataToSend = "Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one. In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs,\" Rajan said in the note.\" Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), uart(new FirmwareUART(this)) {
    ui->setupUi(this);

    uart->setDataToSend(dataToSend);

    // Populate available COM ports
    // const auto ports = QSerialPortInfo::availablePorts();
    // for (const QSerialPortInfo &port : ports) {
    //     ui->comboBoxCom->addItem(port.portName());
    // }
    // Connect signals and slots
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::toggleConnectDisconnect);
    connect(uart, &FirmwareUART::transmissionSpeedUpdated, this, &MainWindow::updateTransmissionSpeed);
    connect(uart, &FirmwareUART::receiveSpeedUpdated, this, &MainWindow::updateReceiveSpeed);
    connect(uart, &FirmwareUART::connectionStatusChanged, this, &MainWindow::updateConnectionStatus);
    connect(ui->btnClear,  &QPushButton::clicked, this, &MainWindow::clearConsole);

    // Connect dataReceived signal to appendReceivedData slot
    connect(uart, &FirmwareUART::dataReceived, this, &MainWindow::appendReceivedData);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateAvailablePorts);
    timer->start(2000);  // Adjust the interval as needed, e.g., every 2 seconds

    updateAvailablePorts();
}

MainWindow::~MainWindow() {
    delete uart;
    delete ui;
}

void MainWindow::toggleConnectDisconnect() {
    if (ui->btnConnect->text() == "Connect") {
        // If logging is enabled, try to set up the log file
        if (ui->radioButtonLog->isChecked()) {
            QString filePath = ui->lineEditLocation->text();
            if (filePath.isEmpty()) {
                QMessageBox::warning(this, "File Path Required", "Please specify a file path for logging.");
                return;
            }
            if (!uart->setupLogFile(filePath)) {
                QMessageBox::critical(this, "Logging Error", "Failed to set up log file. Please check the file path.");
                return;
            }
        } else {
            uart->closeLogFile();  // Ensure no log file is open if logging is disabled
        }

        // Get port name and baud rate from UI and connect to the port
        QString portName = ui->comboBoxCom->currentText();
        int baudRate = ui->lineEditBaudRate->text().toInt();
        if (uart->connectToPort(portName, baudRate)) {
            ui->btnConnect->setText("Disconnect");
        } else {
            QMessageBox::critical(this, "Connection Failed", "Could not connect to the port.");
        }
    } else {
        // Disconnect from the port and close any open log file
        uart->disconnectFromPort();
        uart->closeLogFile();
        ui->btnConnect->setText("Connect");
    }
}

void MainWindow::appendReceivedData(const QByteArray &data) {
    // Directly append the received data to the txtConsole
    ui->txtConsole->append(QString::fromUtf8(data));
}

void MainWindow::updateTransmissionSpeed(double speedBps) {
    ui->txtConsole->append(QString("Live Transmission Speed: %1 bps").arg(speedBps, 0, 'f', 2));
}

void MainWindow::updateReceiveSpeed(double speedBps) {
    ui->txtConsole->append(QString("Live Receive Speed: %1 bps").arg(speedBps, 0, 'f', 2));
}

void MainWindow::updateConnectionStatus(bool isConnected) {
    if (!isConnected && ui->btnConnect->text() == "Disconnect") {
        QMessageBox::warning(this, "Disconnected", "Lost connection to the device.");
        ui->btnConnect->setText("Connect");
    }
}

void MainWindow::updateAvailablePorts() {
    // Get the currently selected port
    QString currentPortName = ui->comboBoxCom->currentText();

    // Get the list of available ports
    const auto currentPorts = QSerialPortInfo::availablePorts();

    // Convert currentPorts and previousPorts to lists of port names for comparison
    QStringList currentPortNames;
    for (const auto &port : currentPorts) {
        currentPortNames << port.portName();
    }

    QStringList previousPortNames;
    for (const auto &port : previousPorts) {
        previousPortNames << port.portName();
    }

    // Check if the list of port names has changed
    if (currentPortNames != previousPortNames) {
        // Clear and repopulate the combo box
        ui->comboBoxCom->clear();
        for (const QSerialPortInfo &port : currentPorts) {
            ui->comboBoxCom->addItem(port.portName());
        }

        // Try to reselect the previously selected port if it still exists
        int index = ui->comboBoxCom->findText(currentPortName);
        if (index != -1) {
            ui->comboBoxCom->setCurrentIndex(index);
        } else if (uart->isConnected()) {
            // Notify the user only if there was an active connection
            QMessageBox::information(this, "Port Disconnected", "The selected COM port is no longer available.");
        }

        // Update the previousPorts list with the current list
        previousPorts = currentPorts;
    }
}

void MainWindow::clearConsole()
{
    ui->txtConsole->clear();
}

