#include "curve.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Curve w;
    w.show();

    return a.exec();
}
