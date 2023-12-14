#include "langman.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Langman w;
    QIcon windowIcon(":/new/image/icon.png");
    w.setWindowIcon(windowIcon);
    w.show();
    return a.exec();
}
