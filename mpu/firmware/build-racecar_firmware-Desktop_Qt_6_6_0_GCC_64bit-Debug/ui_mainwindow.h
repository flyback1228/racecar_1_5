/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_8;
    QTabWidget *tabWidget;
    QWidget *tab_version;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QLineEdit *ledVersion;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab_pid;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *ledKp;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *ledKi;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *ledKd;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *ledPIDFrequency;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_esc;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *ledEscFrequency;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *ledEscOffset;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *ledEscMin;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *ledEscMax;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QLineEdit *ledSteeringOffset;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QLineEdit *ledSteeringMin;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QLineEdit *ledSteeringMax;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_15;
    QLabel *ratio;
    QLineEdit *ledSteeringRate;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_miscellaneous;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_14;
    QLineEdit *ledRosFrequency;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_17;
    QLabel *labelBrakeFrequency;
    QLineEdit *ledBrakeFrequency;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_5;
    QPlainTextEdit *pteMessage;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *cmbPort;
    QPushButton *btnOpen;
    QPushButton *btnClose;
    QSpacerItem *verticalSpacer_6;
    QPushButton *btnLoad;
    QPushButton *btnSave;
    QSpacerItem *verticalSpacer;
    QPushButton *btnRead;
    QPushButton *btnWrite;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        verticalLayout_8->setContentsMargins(10, -1, -1, -1);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab_version = new QWidget();
        tab_version->setObjectName("tab_version");
        verticalLayout_6 = new QVBoxLayout(tab_version);
        verticalLayout_6->setObjectName("verticalLayout_6");
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_10 = new QLabel(tab_version);
        label_10->setObjectName("label_10");

        horizontalLayout_11->addWidget(label_10);

        ledVersion = new QLineEdit(tab_version);
        ledVersion->setObjectName("ledVersion");
        ledVersion->setEnabled(false);

        horizontalLayout_11->addWidget(ledVersion);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_13);


        verticalLayout_6->addLayout(horizontalLayout_11);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_version, QString());
        tab_pid = new QWidget();
        tab_pid->setObjectName("tab_pid");
        verticalLayout_3 = new QVBoxLayout(tab_pid);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(-1, 1, 10, 1);
        label_2 = new QLabel(tab_pid);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_2);

        ledKp = new QLineEdit(tab_pid);
        ledKp->setObjectName("ledKp");
        ledKp->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(ledKp);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(-1, 1, 10, 1);
        label_3 = new QLabel(tab_pid);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(100, 0));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        ledKi = new QLineEdit(tab_pid);
        ledKi->setObjectName("ledKi");
        ledKi->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(ledKi);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(-1, 1, 10, 1);
        label_4 = new QLabel(tab_pid);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(100, 0));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_4);

        ledKd = new QLineEdit(tab_pid);
        ledKd->setObjectName("ledKd");
        ledKd->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_5->addWidget(ledKd);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(-1, 1, 0, 6);
        label_5 = new QLabel(tab_pid);
        label_5->setObjectName("label_5");
        label_5->setMinimumSize(QSize(100, 0));

        horizontalLayout_6->addWidget(label_5);

        ledPIDFrequency = new QLineEdit(tab_pid);
        ledPIDFrequency->setObjectName("ledPIDFrequency");
        ledPIDFrequency->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(ledPIDFrequency);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_pid, QString());
        tab_esc = new QWidget();
        tab_esc->setObjectName("tab_esc");
        verticalLayout_2 = new QVBoxLayout(tab_esc);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_7 = new QLabel(tab_esc);
        label_7->setObjectName("label_7");
        label_7->setMinimumSize(QSize(160, 0));
        label_7->setMaximumSize(QSize(100, 16777215));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(label_7);

        ledEscFrequency = new QLineEdit(tab_esc);
        ledEscFrequency->setObjectName("ledEscFrequency");
        ledEscFrequency->setMinimumSize(QSize(50, 0));
        ledEscFrequency->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_8->addWidget(ledEscFrequency);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_8);

        groupBox = new QGroupBox(tab_esc);
        groupBox->setObjectName("groupBox");
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(150, 0));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_6);

        ledEscOffset = new QLineEdit(groupBox);
        ledEscOffset->setObjectName("ledEscOffset");
        ledEscOffset->setMinimumSize(QSize(50, 0));

        horizontalLayout_7->addWidget(ledEscOffset);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setMinimumSize(QSize(150, 0));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(label_8);

        ledEscMin = new QLineEdit(groupBox);
        ledEscMin->setObjectName("ledEscMin");
        ledEscMin->setMinimumSize(QSize(50, 0));

        horizontalLayout_9->addWidget(ledEscMin);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(150, 0));
        label_9->setMaximumSize(QSize(100, 16777215));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_9);

        ledEscMax = new QLineEdit(groupBox);
        ledEscMax->setObjectName("ledEscMax");
        ledEscMax->setMinimumSize(QSize(50, 0));

        horizontalLayout_10->addWidget(ledEscMax);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);


        verticalLayout_5->addLayout(horizontalLayout_10);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_esc);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName("label_11");
        label_11->setMinimumSize(QSize(150, 0));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_11);

        ledSteeringOffset = new QLineEdit(groupBox_2);
        ledSteeringOffset->setObjectName("ledSteeringOffset");
        ledSteeringOffset->setMinimumSize(QSize(50, 0));

        horizontalLayout_12->addWidget(ledSteeringOffset);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName("label_12");
        label_12->setMinimumSize(QSize(150, 0));
        label_12->setMaximumSize(QSize(100, 16777215));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_13->addWidget(label_12);

        ledSteeringMin = new QLineEdit(groupBox_2);
        ledSteeringMin->setObjectName("ledSteeringMin");
        ledSteeringMin->setMinimumSize(QSize(50, 0));

        horizontalLayout_13->addWidget(ledSteeringMin);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);


        verticalLayout_4->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName("label_13");
        label_13->setMinimumSize(QSize(150, 0));
        label_13->setMaximumSize(QSize(100, 16777215));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_14->addWidget(label_13);

        ledSteeringMax = new QLineEdit(groupBox_2);
        ledSteeringMax->setObjectName("ledSteeringMax");
        ledSteeringMax->setMinimumSize(QSize(50, 0));

        horizontalLayout_14->addWidget(ledSteeringMax);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);


        verticalLayout_4->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        ratio = new QLabel(groupBox_2);
        ratio->setObjectName("ratio");
        ratio->setMinimumSize(QSize(150, 0));
        ratio->setMaximumSize(QSize(100, 16777215));
        ratio->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_15->addWidget(ratio);

        ledSteeringRate = new QLineEdit(groupBox_2);
        ledSteeringRate->setObjectName("ledSteeringRate");
        ledSteeringRate->setMinimumSize(QSize(50, 0));

        horizontalLayout_15->addWidget(ledSteeringRate);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_12);


        verticalLayout_4->addLayout(horizontalLayout_15);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_esc, QString());
        tab_miscellaneous = new QWidget();
        tab_miscellaneous->setObjectName("tab_miscellaneous");
        verticalLayout_7 = new QVBoxLayout(tab_miscellaneous);
        verticalLayout_7->setObjectName("verticalLayout_7");
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        label_14 = new QLabel(tab_miscellaneous);
        label_14->setObjectName("label_14");
        label_14->setMinimumSize(QSize(200, 0));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_16->addWidget(label_14);

        ledRosFrequency = new QLineEdit(tab_miscellaneous);
        ledRosFrequency->setObjectName("ledRosFrequency");

        horizontalLayout_16->addWidget(ledRosFrequency);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_14);


        verticalLayout_7->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        labelBrakeFrequency = new QLabel(tab_miscellaneous);
        labelBrakeFrequency->setObjectName("labelBrakeFrequency");
        labelBrakeFrequency->setMinimumSize(QSize(200, 0));
        labelBrakeFrequency->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_17->addWidget(labelBrakeFrequency);

        ledBrakeFrequency = new QLineEdit(tab_miscellaneous);
        ledBrakeFrequency->setObjectName("ledBrakeFrequency");

        horizontalLayout_17->addWidget(ledBrakeFrequency);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_15);


        verticalLayout_7->addLayout(horizontalLayout_17);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_5);

        tabWidget->addTab(tab_miscellaneous, QString());

        verticalLayout_8->addWidget(tabWidget);

        pteMessage = new QPlainTextEdit(centralwidget);
        pteMessage->setObjectName("pteMessage");

        verticalLayout_8->addWidget(pteMessage);


        horizontalLayout->addLayout(verticalLayout_8);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        cmbPort = new QComboBox(centralwidget);
        cmbPort->setObjectName("cmbPort");

        horizontalLayout_2->addWidget(cmbPort);


        verticalLayout->addLayout(horizontalLayout_2);

        btnOpen = new QPushButton(centralwidget);
        btnOpen->setObjectName("btnOpen");

        verticalLayout->addWidget(btnOpen);

        btnClose = new QPushButton(centralwidget);
        btnClose->setObjectName("btnClose");

        verticalLayout->addWidget(btnClose);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        btnLoad = new QPushButton(centralwidget);
        btnLoad->setObjectName("btnLoad");

        verticalLayout->addWidget(btnLoad);

        btnSave = new QPushButton(centralwidget);
        btnSave->setObjectName("btnSave");

        verticalLayout->addWidget(btnSave);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnRead = new QPushButton(centralwidget);
        btnRead->setObjectName("btnRead");

        verticalLayout->addWidget(btnRead);

        btnWrite = new QPushButton(centralwidget);
        btnWrite->setObjectName("btnWrite");

        verticalLayout->addWidget(btnWrite);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Racecar Configuration", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Current Firmware Version", nullptr));
        ledVersion->setText(QCoreApplication::translate("MainWindow", "1.0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_version), QCoreApplication::translate("MainWindow", "Version", nullptr));
#if QT_CONFIG(tooltip)
        tab_pid->setToolTip(QCoreApplication::translate("MainWindow", "set PID parameters", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("MainWindow", "kp", nullptr));
#if QT_CONFIG(tooltip)
        ledKp->setToolTip(QCoreApplication::translate("MainWindow", "set kp value, range -10000 to 10000", nullptr));
#endif // QT_CONFIG(tooltip)
        ledKp->setText(QCoreApplication::translate("MainWindow", "1.00", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "ki", nullptr));
        ledKi->setText(QCoreApplication::translate("MainWindow", "1.00", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "kd", nullptr));
        ledKd->setText(QCoreApplication::translate("MainWindow", "0.00", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "PID Frequency", nullptr));
        ledPIDFrequency->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_pid), QCoreApplication::translate("MainWindow", "PID", nullptr));
#if QT_CONFIG(tooltip)
        tab_esc->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("MainWindow", "Frequency", nullptr));
        ledEscFrequency->setText(QCoreApplication::translate("MainWindow", "64.5", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "ESC", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "offset", nullptr));
        ledEscOffset->setText(QCoreApplication::translate("MainWindow", "1500", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "min duty cycle", nullptr));
        ledEscMin->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "max duty cycle", nullptr));
        ledEscMax->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Steering Servo", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "offset", nullptr));
        ledSteeringOffset->setText(QCoreApplication::translate("MainWindow", "1500", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "min steering angle", nullptr));
        ledSteeringMin->setText(QCoreApplication::translate("MainWindow", "-17", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "max steering angle", nullptr));
        ledSteeringMax->setText(QCoreApplication::translate("MainWindow", "17", nullptr));
        ratio->setText(QCoreApplication::translate("MainWindow", "rate", nullptr));
        ledSteeringRate->setText(QCoreApplication::translate("MainWindow", "1000", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_esc), QCoreApplication::translate("MainWindow", "ESC", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "ROS Topic Frequency", nullptr));
        ledRosFrequency->setText(QCoreApplication::translate("MainWindow", "20", nullptr));
        labelBrakeFrequency->setText(QCoreApplication::translate("MainWindow", "Brake PWM Frequency", nullptr));
        ledBrakeFrequency->setText(QCoreApplication::translate("MainWindow", "1000", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_miscellaneous), QCoreApplication::translate("MainWindow", "Miscellaneous", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "port", nullptr));
#if QT_CONFIG(tooltip)
        btnOpen->setToolTip(QCoreApplication::translate("MainWindow", "open serial port", nullptr));
#endif // QT_CONFIG(tooltip)
        btnOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(tooltip)
        btnClose->setToolTip(QCoreApplication::translate("MainWindow", "close serial port", nullptr));
#endif // QT_CONFIG(tooltip)
        btnClose->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
#if QT_CONFIG(tooltip)
        btnLoad->setToolTip(QCoreApplication::translate("MainWindow", "load configuration file", nullptr));
#endif // QT_CONFIG(tooltip)
        btnLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
#if QT_CONFIG(tooltip)
        btnSave->setToolTip(QCoreApplication::translate("MainWindow", "save configurations to file", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(tooltip)
        btnRead->setToolTip(QCoreApplication::translate("MainWindow", "read mpu configurations", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRead->setText(QCoreApplication::translate("MainWindow", "Read", nullptr));
#if QT_CONFIG(tooltip)
        btnWrite->setToolTip(QCoreApplication::translate("MainWindow", "write configurations to mpu", nullptr));
#endif // QT_CONFIG(tooltip)
        btnWrite->setText(QCoreApplication::translate("MainWindow", "Write", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
