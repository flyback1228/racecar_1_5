#include "mainwindow.h"

#include <QApplication>
#include "paramter_type.h"
#include <QDataStream>
#include <QVariant>
#include <QMetaType>

int main(int argc, char *argv[])
{
//    qRegisterMetaTypeStreamOperators<ParameterTypeDef>("ParameterTypeDef");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
