#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Twitter Timeline");
    MainWindow w;
    w.show();
    return QApplication::exec();
}
