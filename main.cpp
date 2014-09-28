#include "playo.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Playo w;
    w.show();

//    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
