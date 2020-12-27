#include "plot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    plot w;
    w.show();

    return a.exec();
}
