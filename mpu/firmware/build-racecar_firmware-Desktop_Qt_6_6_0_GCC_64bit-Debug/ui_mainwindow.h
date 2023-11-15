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
    QTabWidget *tabWidget;
    QWidget *tab_version;
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
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QLineEdit *lineEdit_5;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QLineEdit *lineEdit_6;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QLineEdit *lineEdit_7;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_15;
    QLabel *ratio;
    QLineEdit *lineEdit_8;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *cmbPort;
    QPushButton *btnOpen;
    QPushButton *btnClose;
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
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab_version = new QWidget();
        tab_version->setObjectName("tab_version");
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
        label_7->setMinimumSize(QSize(100, 0));
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
        label_6->setMinimumSize(QSize(100, 0));
        label_6->setMaximumSize(QSize(100, 16777215));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_6);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setMinimumSize(QSize(50, 0));

        horizontalLayout_7->addWidget(lineEdit);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setMinimumSize(QSize(100, 0));
        label_8->setMaximumSize(QSize(100, 16777215));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(label_8);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setMinimumSize(QSize(50, 0));

        horizontalLayout_9->addWidget(lineEdit_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(100, 0));
        label_9->setMaximumSize(QSize(100, 16777215));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_10->addWidget(label_9);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setMinimumSize(QSize(50, 0));

        horizontalLayout_10->addWidget(lineEdit_3);

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
        label_11->setMinimumSize(QSize(100, 0));
        label_11->setMaximumSize(QSize(100, 16777215));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_12->addWidget(label_11);

        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setMinimumSize(QSize(50, 0));

        horizontalLayout_12->addWidget(lineEdit_5);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName("label_12");
        label_12->setMinimumSize(QSize(100, 0));
        label_12->setMaximumSize(QSize(100, 16777215));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_13->addWidget(label_12);

        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setMinimumSize(QSize(50, 0));

        horizontalLayout_13->addWidget(lineEdit_6);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_10);


        verticalLayout_4->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName("label_13");
        label_13->setMinimumSize(QSize(100, 0));
        label_13->setMaximumSize(QSize(100, 16777215));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_14->addWidget(label_13);

        lineEdit_7 = new QLineEdit(groupBox_2);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setMinimumSize(QSize(50, 0));

        horizontalLayout_14->addWidget(lineEdit_7);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);


        verticalLayout_4->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        ratio = new QLabel(groupBox_2);
        ratio->setObjectName("ratio");
        ratio->setMinimumSize(QSize(100, 0));
        ratio->setMaximumSize(QSize(100, 16777215));
        ratio->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_15->addWidget(ratio);

        lineEdit_8 = new QLineEdit(groupBox_2);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setMinimumSize(QSize(50, 0));

        horizontalLayout_15->addWidget(lineEdit_8);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_12);


        verticalLayout_4->addLayout(horizontalLayout_15);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        tabWidget->addTab(tab_esc, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        tabWidget->addTab(tab, QString());

        horizontalLayout->addWidget(tabWidget);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
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
        lineEdit->setText(QCoreApplication::translate("MainWindow", "1500", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "min", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "max", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Steering Servo", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "offset", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("MainWindow", "1500", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "min", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "-17", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "max", nullptr));
        lineEdit_7->setText(QCoreApplication::translate("MainWindow", "17", nullptr));
        ratio->setText(QCoreApplication::translate("MainWindow", "rate", nullptr));
        lineEdit_8->setText(QCoreApplication::translate("MainWindow", "1000", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_esc), QCoreApplication::translate("MainWindow", "ESC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Page", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "port", nullptr));
        btnOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        btnClose->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        btnRead->setText(QCoreApplication::translate("MainWindow", "Read", nullptr));
        btnWrite->setText(QCoreApplication::translate("MainWindow", "Write", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
