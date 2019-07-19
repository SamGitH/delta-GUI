#include "mainwindow.h"
#include <QApplication>
#include "QDebug"



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
 //   QCoreApplication::setApplicationName("MySerial");

    MainWindow w;
    w.show();

    return a.exec();
}





