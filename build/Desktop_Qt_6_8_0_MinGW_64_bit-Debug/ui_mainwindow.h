/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextBrowser *txtConsole;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QComboBox *comboBoxCom;
    QLabel *label_2;
    QLineEdit *lineEditBaudRate;
    QPushButton *btnConnect;
    QLabel *label_3;
    QLineEdit *lineEditLocation;
    QLabel *label_4;
    QRadioButton *radioButtonLog;
    QPushButton *btnClear;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(642, 495);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        txtConsole = new QTextBrowser(centralwidget);
        txtConsole->setObjectName("txtConsole");

        gridLayout->addWidget(txtConsole, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName("gridLayout_3");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        comboBoxCom = new QComboBox(groupBox);
        comboBoxCom->setObjectName("comboBoxCom");

        gridLayout_3->addWidget(comboBoxCom, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 0, 2, 1, 1);

        lineEditBaudRate = new QLineEdit(groupBox);
        lineEditBaudRate->setObjectName("lineEditBaudRate");

        gridLayout_3->addWidget(lineEditBaudRate, 0, 3, 1, 1);

        btnConnect = new QPushButton(groupBox);
        btnConnect->setObjectName("btnConnect");

        gridLayout_3->addWidget(btnConnect, 0, 4, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        gridLayout_3->addWidget(label_3, 1, 0, 1, 1);

        lineEditLocation = new QLineEdit(groupBox);
        lineEditLocation->setObjectName("lineEditLocation");

        gridLayout_3->addWidget(lineEditLocation, 1, 1, 1, 3);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        gridLayout_3->addWidget(label_4, 2, 1, 1, 1);

        radioButtonLog = new QRadioButton(groupBox);
        radioButtonLog->setObjectName("radioButtonLog");

        gridLayout_3->addWidget(radioButtonLog, 1, 4, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");

        gridLayout->addWidget(btnClear, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 642, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "UART", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "SERIAL", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "COM PORT", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "BAUD RATE", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "FILE NAME", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "location/file.txt", nullptr));
        radioButtonLog->setText(QCoreApplication::translate("MainWindow", "Log", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
