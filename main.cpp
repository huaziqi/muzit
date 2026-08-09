#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qSetMessagePattern(
        "[%{time hh:mm:ss.zzz}] "
        "[%{type}] "
        "[%{threadid}] "
        "%{file}:%{line} "
        "%{function} | "
        "%{message}"
        );
    MainWidget w;
    w.show();
    return a.exec();
}
