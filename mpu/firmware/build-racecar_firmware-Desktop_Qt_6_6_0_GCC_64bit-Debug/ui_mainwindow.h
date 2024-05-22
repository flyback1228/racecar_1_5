/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
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
#include "QtCharts"

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
    QHBoxLayout *horizontalLayout_21;
    QLabel *ratio_3;
    QLineEdit *ledEscRate;
    QSpacerItem *horizontalSpacer_21;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_15;
    QLineEdit *ledEscPrecision;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_20;
    QCheckBox *ckbEscReverse;
    QSpacerItem *horizontalSpacer_20;
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
    QHBoxLayout *horizontalLayout_19;
    QLabel *ratio_2;
    QLineEdit *ledServoPrecision;
    QSpacerItem *horizontalSpacer_17;
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
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QLabel *label_25;
    QLineEdit *ledForceOffset6;
    QLineEdit *ledForceOffset1;
    QLabel *label_38;
    QLineEdit *ledForceRatio5;
    QSpacerItem *horizontalSpacer_18;
    QLabel *label_19;
    QLabel *label_39;
    QLineEdit *ledForceRatio4;
    QLabel *label_36;
    QLabel *label_22;
    QLineEdit *ledForceRatio1;
    QLineEdit *ledForceRatio7;
    QLabel *label_41;
    QLineEdit *ledForceOffset2;
    QLabel *label_24;
    QLineEdit *ledForceRatio6;
    QLabel *label_40;
    QLineEdit *ledForceRatio0;
    QLineEdit *ledForceRatio2;
    QLineEdit *ledForceRatio3;
    QLineEdit *ledForceOffset4;
    QLabel *label_18;
    QLabel *label_20;
    QLineEdit *ledForceOffset0;
    QLabel *label_37;
    QLineEdit *ledForceOffset3;
    QLineEdit *ledForceOffset5;
    QLineEdit *ledForceOffset7;
    QSpacerItem *verticalSpacer_5;
    QWidget *tab_speed;
    QHBoxLayout *horizonLayout_8;
    QChartView *chartView;
    QVBoxLayout *verticalLayout_9;
    QPushButton *btnSpeedStart;
    QPushButton *btnSpeedStop;
    QSpacerItem *verticalSpacer_7;
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
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(951, 689);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(10, -1, -1, -1);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_version = new QWidget();
        tab_version->setObjectName(QString::fromUtf8("tab_version"));
        verticalLayout_6 = new QVBoxLayout(tab_version);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_10 = new QLabel(tab_version);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_11->addWidget(label_10);

        ledVersion = new QLineEdit(tab_version);
        ledVersion->setObjectName(QString::fromUtf8("ledVersion"));
        ledVersion->setEnabled(false);

        horizontalLayout_11->addWidget(ledVersion);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_13);


        verticalLayout_6->addLayout(horizontalLayout_11);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_version, QString());
        tab_pid = new QWidget();
        tab_pid->setObjectName(QString::fromUtf8("tab_pid"));
        verticalLayout_3 = new QVBoxLayout(tab_pid);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 1, 10, 1);
        label_2 = new QLabel(tab_pid);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_2);

        ledKp = new QLineEdit(tab_pid);
        ledKp->setObjectName(QString::fromUtf8("ledKp"));
        ledKp->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(ledKp);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 1, 10, 1);
        label_3 = new QLabel(tab_pid);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(100, 0));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        ledKi = new QLineEdit(tab_pid);
        ledKi->setObjectName(QString::fromUtf8("ledKi"));
        ledKi->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(ledKi);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 1, 10, 1);
        label_4 = new QLabel(tab_pid);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(100, 0));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_4);

        ledKd = new QLineEdit(tab_pid);
        ledKd->setObjectName(QString::fromUtf8("ledKd"));
        ledKd->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_5->addWidget(ledKd);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 1, 0, 6);
        label_5 = new QLabel(tab_pid);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(100, 0));

        horizontalLayout_6->addWidget(label_5);

        ledPIDFrequency = new QLineEdit(tab_pid);
        ledPIDFrequency->setObjectName(QString::fromUtf8("ledPIDFrequency"));
        ledPIDFrequency->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(ledPIDFrequency);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_pid, QString());
        tab_esc = new QWidget();
        tab_esc->setObjectName(QString::fromUtf8("tab_esc"));
        verticalLayout_2 = new QVBoxLayout(tab_esc);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(tab_esc);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(160, 0));
        label_7->setMaximumSize(QSize(100, 16777215));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(label_7);

        ledEscFrequency = new QLineEdit(tab_esc);
        ledEscFrequency->setObjectName(QString::fromUtf8("ledEscFrequency"));
        ledEscFrequency->setMinimumSize(QSize(50, 0));
        ledEscFrequency->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_8->addWidget(ledEscFrequency);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_8);

        groupBox = new QGroupBox(tab_esc);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(150, 0));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_6);

        ledEscOffset = new QLineEdit(groupBox);
        ledEscOffset->setObjectName(QString::fromUtf8("ledEscOffset"));
        ledEscOffset->setMinimumSize(QSize(50, 0));

        horizontalLayout_7->addWidget(ledEscOffset);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(150, 0));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(label_8);

        ledEscMin = new QLineEdit(groupBox);
        ledEscMin->setObjectName(QString::fromUtf8("ledEscMin"));
        ledEscMin->setMinimumSize(QSize(50, 0));

        horizontalLayout_9->addWidget(ledEscMin);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(150, 0));
        label_9->setMaximumSize(QSize(100, 16777215));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_9);

        ledEscMax = new QLineEdit(groupBox);
        ledEscMax->setObjectName(QString::fromUtf8("ledEscMax"));
        ledEscMax->setMinimumSize(QSize(50, 0));

        horizontalLayout_10->addWidget(ledEscMax);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_8);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        ratio_3 = new QLabel(groupBox);
        ratio_3->setObjectName(QString::fromUtf8("ratio_3"));
        ratio_3->setMinimumSize(QSize(150, 0));
        ratio_3->setMaximumSize(QSize(100, 16777215));
        ratio_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_21->addWidget(ratio_3);

        ledEscRate = new QLineEdit(groupBox);
        ledEscRate->setObjectName(QString::fromUtf8("ledEscRate"));
        ledEscRate->setMinimumSize(QSize(50, 0));

        horizontalLayout_21->addWidget(ledEscRate);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_21->addItem(horizontalSpacer_21);


        verticalLayout_5->addLayout(horizontalLayout_21);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(150, 0));
        label_15->setMaximumSize(QSize(100, 16777215));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_18->addWidget(label_15);

        ledEscPrecision = new QLineEdit(groupBox);
        ledEscPrecision->setObjectName(QString::fromUtf8("ledEscPrecision"));
        ledEscPrecision->setMinimumSize(QSize(50, 0));

        horizontalLayout_18->addWidget(ledEscPrecision);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_16);


        verticalLayout_5->addLayout(horizontalLayout_18);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        ckbEscReverse = new QCheckBox(groupBox);
        ckbEscReverse->setObjectName(QString::fromUtf8("ckbEscReverse"));

        horizontalLayout_20->addWidget(ckbEscReverse);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_20);


        verticalLayout_5->addLayout(horizontalLayout_20);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_esc);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(150, 0));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_11);

        ledSteeringOffset = new QLineEdit(groupBox_2);
        ledSteeringOffset->setObjectName(QString::fromUtf8("ledSteeringOffset"));
        ledSteeringOffset->setMinimumSize(QSize(50, 0));

        horizontalLayout_12->addWidget(ledSteeringOffset);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(150, 0));
        label_12->setMaximumSize(QSize(100, 16777215));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_13->addWidget(label_12);

        ledSteeringMin = new QLineEdit(groupBox_2);
        ledSteeringMin->setObjectName(QString::fromUtf8("ledSteeringMin"));
        ledSteeringMin->setMinimumSize(QSize(50, 0));

        horizontalLayout_13->addWidget(ledSteeringMin);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);


        verticalLayout_4->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(150, 0));
        label_13->setMaximumSize(QSize(100, 16777215));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_14->addWidget(label_13);

        ledSteeringMax = new QLineEdit(groupBox_2);
        ledSteeringMax->setObjectName(QString::fromUtf8("ledSteeringMax"));
        ledSteeringMax->setMinimumSize(QSize(50, 0));

        horizontalLayout_14->addWidget(ledSteeringMax);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);


        verticalLayout_4->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        ratio = new QLabel(groupBox_2);
        ratio->setObjectName(QString::fromUtf8("ratio"));
        ratio->setMinimumSize(QSize(150, 0));
        ratio->setMaximumSize(QSize(100, 16777215));
        ratio->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_15->addWidget(ratio);

        ledSteeringRate = new QLineEdit(groupBox_2);
        ledSteeringRate->setObjectName(QString::fromUtf8("ledSteeringRate"));
        ledSteeringRate->setMinimumSize(QSize(50, 0));

        horizontalLayout_15->addWidget(ledSteeringRate);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_12);


        verticalLayout_4->addLayout(horizontalLayout_15);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        ratio_2 = new QLabel(groupBox_2);
        ratio_2->setObjectName(QString::fromUtf8("ratio_2"));
        ratio_2->setMinimumSize(QSize(150, 0));
        ratio_2->setMaximumSize(QSize(100, 16777215));
        ratio_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_19->addWidget(ratio_2);

        ledServoPrecision = new QLineEdit(groupBox_2);
        ledServoPrecision->setObjectName(QString::fromUtf8("ledServoPrecision"));
        ledServoPrecision->setMinimumSize(QSize(50, 0));

        horizontalLayout_19->addWidget(ledServoPrecision);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_17);


        verticalLayout_4->addLayout(horizontalLayout_19);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_esc, QString());
        tab_miscellaneous = new QWidget();
        tab_miscellaneous->setObjectName(QString::fromUtf8("tab_miscellaneous"));
        verticalLayout_7 = new QVBoxLayout(tab_miscellaneous);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_14 = new QLabel(tab_miscellaneous);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(200, 0));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_16->addWidget(label_14);

        ledRosFrequency = new QLineEdit(tab_miscellaneous);
        ledRosFrequency->setObjectName(QString::fromUtf8("ledRosFrequency"));

        horizontalLayout_16->addWidget(ledRosFrequency);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_14);


        verticalLayout_7->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        labelBrakeFrequency = new QLabel(tab_miscellaneous);
        labelBrakeFrequency->setObjectName(QString::fromUtf8("labelBrakeFrequency"));
        labelBrakeFrequency->setMinimumSize(QSize(200, 0));
        labelBrakeFrequency->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_17->addWidget(labelBrakeFrequency);

        ledBrakeFrequency = new QLineEdit(tab_miscellaneous);
        ledBrakeFrequency->setObjectName(QString::fromUtf8("ledBrakeFrequency"));

        horizontalLayout_17->addWidget(ledBrakeFrequency);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_15);


        verticalLayout_7->addLayout(horizontalLayout_17);

        groupBox_3 = new QGroupBox(tab_miscellaneous);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_25 = new QLabel(groupBox_3);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout->addWidget(label_25, 2, 3, 1, 1);

        ledForceOffset6 = new QLineEdit(groupBox_3);
        ledForceOffset6->setObjectName(QString::fromUtf8("ledForceOffset6"));

        gridLayout->addWidget(ledForceOffset6, 4, 2, 1, 1);

        ledForceOffset1 = new QLineEdit(groupBox_3);
        ledForceOffset1->setObjectName(QString::fromUtf8("ledForceOffset1"));

        gridLayout->addWidget(ledForceOffset1, 1, 5, 1, 1);

        label_38 = new QLabel(groupBox_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout->addWidget(label_38, 0, 1, 1, 1);

        ledForceRatio5 = new QLineEdit(groupBox_3);
        ledForceRatio5->setObjectName(QString::fromUtf8("ledForceRatio5"));

        gridLayout->addWidget(ledForceRatio5, 3, 4, 1, 1);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_18, 1, 6, 1, 1);

        label_19 = new QLabel(groupBox_3);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 2, 0, 1, 1);

        label_39 = new QLabel(groupBox_3);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout->addWidget(label_39, 0, 5, 1, 1);

        ledForceRatio4 = new QLineEdit(groupBox_3);
        ledForceRatio4->setObjectName(QString::fromUtf8("ledForceRatio4"));

        gridLayout->addWidget(ledForceRatio4, 3, 1, 1, 1);

        label_36 = new QLabel(groupBox_3);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout->addWidget(label_36, 0, 4, 1, 1);

        label_22 = new QLabel(groupBox_3);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout->addWidget(label_22, 4, 0, 1, 1);

        ledForceRatio1 = new QLineEdit(groupBox_3);
        ledForceRatio1->setObjectName(QString::fromUtf8("ledForceRatio1"));

        gridLayout->addWidget(ledForceRatio1, 1, 4, 1, 1);

        ledForceRatio7 = new QLineEdit(groupBox_3);
        ledForceRatio7->setObjectName(QString::fromUtf8("ledForceRatio7"));

        gridLayout->addWidget(ledForceRatio7, 4, 4, 1, 1);

        label_41 = new QLabel(groupBox_3);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout->addWidget(label_41, 4, 3, 1, 1);

        ledForceOffset2 = new QLineEdit(groupBox_3);
        ledForceOffset2->setObjectName(QString::fromUtf8("ledForceOffset2"));

        gridLayout->addWidget(ledForceOffset2, 2, 2, 1, 1);

        label_24 = new QLabel(groupBox_3);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(label_24, 1, 3, 1, 1);

        ledForceRatio6 = new QLineEdit(groupBox_3);
        ledForceRatio6->setObjectName(QString::fromUtf8("ledForceRatio6"));

        gridLayout->addWidget(ledForceRatio6, 4, 1, 1, 1);

        label_40 = new QLabel(groupBox_3);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout->addWidget(label_40, 3, 3, 1, 1);

        ledForceRatio0 = new QLineEdit(groupBox_3);
        ledForceRatio0->setObjectName(QString::fromUtf8("ledForceRatio0"));

        gridLayout->addWidget(ledForceRatio0, 1, 1, 1, 1);

        ledForceRatio2 = new QLineEdit(groupBox_3);
        ledForceRatio2->setObjectName(QString::fromUtf8("ledForceRatio2"));

        gridLayout->addWidget(ledForceRatio2, 2, 1, 1, 1);

        ledForceRatio3 = new QLineEdit(groupBox_3);
        ledForceRatio3->setObjectName(QString::fromUtf8("ledForceRatio3"));

        gridLayout->addWidget(ledForceRatio3, 2, 4, 1, 1);

        ledForceOffset4 = new QLineEdit(groupBox_3);
        ledForceOffset4->setObjectName(QString::fromUtf8("ledForceOffset4"));

        gridLayout->addWidget(ledForceOffset4, 3, 2, 1, 1);

        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout->addWidget(label_18, 1, 0, 1, 1);

        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout->addWidget(label_20, 3, 0, 1, 1);

        ledForceOffset0 = new QLineEdit(groupBox_3);
        ledForceOffset0->setObjectName(QString::fromUtf8("ledForceOffset0"));

        gridLayout->addWidget(ledForceOffset0, 1, 2, 1, 1);

        label_37 = new QLabel(groupBox_3);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout->addWidget(label_37, 0, 2, 1, 1);

        ledForceOffset3 = new QLineEdit(groupBox_3);
        ledForceOffset3->setObjectName(QString::fromUtf8("ledForceOffset3"));

        gridLayout->addWidget(ledForceOffset3, 2, 5, 1, 1);

        ledForceOffset5 = new QLineEdit(groupBox_3);
        ledForceOffset5->setObjectName(QString::fromUtf8("ledForceOffset5"));

        gridLayout->addWidget(ledForceOffset5, 3, 5, 1, 1);

        ledForceOffset7 = new QLineEdit(groupBox_3);
        ledForceOffset7->setObjectName(QString::fromUtf8("ledForceOffset7"));

        gridLayout->addWidget(ledForceOffset7, 4, 5, 1, 1);


        verticalLayout_7->addWidget(groupBox_3);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_5);

        tabWidget->addTab(tab_miscellaneous, QString());
        tab_speed = new QWidget();
        tab_speed->setObjectName(QString::fromUtf8("tab_speed"));
        horizonLayout_8 = new QHBoxLayout(tab_speed);
        horizonLayout_8->setObjectName(QString::fromUtf8("horizonLayout_8"));
        chartView = new QChartView(tab_speed);
        chartView->setObjectName(QString::fromUtf8("chartView"));

        horizonLayout_8->addWidget(chartView);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(2, 10, 2, -1);
        btnSpeedStart = new QPushButton(tab_speed);
        btnSpeedStart->setObjectName(QString::fromUtf8("btnSpeedStart"));

        verticalLayout_9->addWidget(btnSpeedStart);

        btnSpeedStop = new QPushButton(tab_speed);
        btnSpeedStop->setObjectName(QString::fromUtf8("btnSpeedStop"));

        verticalLayout_9->addWidget(btnSpeedStop);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_7);


        horizonLayout_8->addLayout(verticalLayout_9);

        tabWidget->addTab(tab_speed, QString());

        verticalLayout_8->addWidget(tabWidget);

        pteMessage = new QPlainTextEdit(centralwidget);
        pteMessage->setObjectName(QString::fromUtf8("pteMessage"));

        verticalLayout_8->addWidget(pteMessage);


        horizontalLayout->addLayout(verticalLayout_8);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        cmbPort = new QComboBox(centralwidget);
        cmbPort->setObjectName(QString::fromUtf8("cmbPort"));

        horizontalLayout_2->addWidget(cmbPort);


        verticalLayout->addLayout(horizontalLayout_2);

        btnOpen = new QPushButton(centralwidget);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));

        verticalLayout->addWidget(btnOpen);

        btnClose = new QPushButton(centralwidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        verticalLayout->addWidget(btnClose);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);

        btnLoad = new QPushButton(centralwidget);
        btnLoad->setObjectName(QString::fromUtf8("btnLoad"));

        verticalLayout->addWidget(btnLoad);

        btnSave = new QPushButton(centralwidget);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));

        verticalLayout->addWidget(btnSave);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnRead = new QPushButton(centralwidget);
        btnRead->setObjectName(QString::fromUtf8("btnRead"));

        verticalLayout->addWidget(btnRead);

        btnWrite = new QPushButton(centralwidget);
        btnWrite->setObjectName(QString::fromUtf8("btnWrite"));

        verticalLayout->addWidget(btnWrite);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 951, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Racecar Configuration", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Current Firmware Version", nullptr));
        ledVersion->setText(QApplication::translate("MainWindow", "1.0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_version), QApplication::translate("MainWindow", "Version", nullptr));
#ifndef QT_NO_TOOLTIP
        tab_pid->setToolTip(QApplication::translate("MainWindow", "set PID parameters", nullptr));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("MainWindow", "kp", nullptr));
#ifndef QT_NO_TOOLTIP
        ledKp->setToolTip(QApplication::translate("MainWindow", "set kp value, range -10000 to 10000", nullptr));
#endif // QT_NO_TOOLTIP
        ledKp->setText(QApplication::translate("MainWindow", "1.00", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "ki", nullptr));
        ledKi->setText(QApplication::translate("MainWindow", "1.00", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "kd", nullptr));
        ledKd->setText(QApplication::translate("MainWindow", "0.00", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "PID Frequency", nullptr));
        ledPIDFrequency->setText(QApplication::translate("MainWindow", "10", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_pid), QApplication::translate("MainWindow", "PID", nullptr));
#ifndef QT_NO_TOOLTIP
        tab_esc->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("MainWindow", "Frequency", nullptr));
        ledEscFrequency->setText(QApplication::translate("MainWindow", "64.5", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "ESC", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "offset", nullptr));
        ledEscOffset->setText(QApplication::translate("MainWindow", "1500", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "min duty cycle", nullptr));
        ledEscMin->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "max duty cycle", nullptr));
        ledEscMax->setText(QApplication::translate("MainWindow", "100", nullptr));
        ratio_3->setText(QApplication::translate("MainWindow", "rpm to speed", nullptr));
        ledEscRate->setText(QApplication::translate("MainWindow", "1000", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "ESC PWM Precision", nullptr));
        ledEscPrecision->setText(QApplication::translate("MainWindow", "5", nullptr));
        ckbEscReverse->setText(QApplication::translate("MainWindow", "Allow Reverse", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Steering Servo", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "offset", nullptr));
        ledSteeringOffset->setText(QApplication::translate("MainWindow", "1500", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "min steering angle", nullptr));
        ledSteeringMin->setText(QApplication::translate("MainWindow", "-17", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "max steering angle", nullptr));
        ledSteeringMax->setText(QApplication::translate("MainWindow", "17", nullptr));
        ratio->setText(QApplication::translate("MainWindow", "rate", nullptr));
        ledSteeringRate->setText(QApplication::translate("MainWindow", "1000", nullptr));
        ratio_2->setText(QApplication::translate("MainWindow", "Servo PWM Precision", nullptr));
        ledServoPrecision->setText(QApplication::translate("MainWindow", "5", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_esc), QApplication::translate("MainWindow", "ESC", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "ROS Topic Frequency", nullptr));
        ledRosFrequency->setText(QApplication::translate("MainWindow", "20", nullptr));
        labelBrakeFrequency->setText(QApplication::translate("MainWindow", "Brake PWM Frequency", nullptr));
        ledBrakeFrequency->setText(QApplication::translate("MainWindow", "1000", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Force Calibration", nullptr));
        label_25->setText(QApplication::translate("MainWindow", "Front Right 2", nullptr));
        label_38->setText(QApplication::translate("MainWindow", "Ratio", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "Front Right 1", nullptr));
        label_39->setText(QApplication::translate("MainWindow", "Offset", nullptr));
        label_36->setText(QApplication::translate("MainWindow", "Ratio", nullptr));
        label_22->setText(QApplication::translate("MainWindow", "Rear Right 1", nullptr));
        label_41->setText(QApplication::translate("MainWindow", "Rear Right 2", nullptr));
        label_24->setText(QApplication::translate("MainWindow", "Front Left 2", nullptr));
        label_40->setText(QApplication::translate("MainWindow", "Rear Left 2", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "Front Left 1", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "Rear Left 1", nullptr));
        label_37->setText(QApplication::translate("MainWindow", "Offset", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_miscellaneous), QApplication::translate("MainWindow", "Miscellaneous", nullptr));
        btnSpeedStart->setText(QApplication::translate("MainWindow", "Start Plot", nullptr));
        btnSpeedStop->setText(QApplication::translate("MainWindow", "Stop Plot", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_speed), QApplication::translate("MainWindow", "Speed", nullptr));
        label->setText(QApplication::translate("MainWindow", "port", nullptr));
#ifndef QT_NO_TOOLTIP
        btnOpen->setToolTip(QApplication::translate("MainWindow", "open serial port", nullptr));
#endif // QT_NO_TOOLTIP
        btnOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
#ifndef QT_NO_TOOLTIP
        btnClose->setToolTip(QApplication::translate("MainWindow", "close serial port", nullptr));
#endif // QT_NO_TOOLTIP
        btnClose->setText(QApplication::translate("MainWindow", "Close", nullptr));
#ifndef QT_NO_TOOLTIP
        btnLoad->setToolTip(QApplication::translate("MainWindow", "load configuration file", nullptr));
#endif // QT_NO_TOOLTIP
        btnLoad->setText(QApplication::translate("MainWindow", "Load", nullptr));
#ifndef QT_NO_TOOLTIP
        btnSave->setToolTip(QApplication::translate("MainWindow", "save configurations to file", nullptr));
#endif // QT_NO_TOOLTIP
        btnSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
#ifndef QT_NO_TOOLTIP
        btnRead->setToolTip(QApplication::translate("MainWindow", "read mpu configurations", nullptr));
#endif // QT_NO_TOOLTIP
        btnRead->setText(QApplication::translate("MainWindow", "Read", nullptr));
#ifndef QT_NO_TOOLTIP
        btnWrite->setToolTip(QApplication::translate("MainWindow", "write configurations to mpu", nullptr));
#endif // QT_NO_TOOLTIP
        btnWrite->setText(QApplication::translate("MainWindow", "Write", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
