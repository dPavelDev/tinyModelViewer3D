#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    w._timer.start(1000.0 / w.fps, &w); // start redraw timer

    return a.exec();
}
