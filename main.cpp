#include "playo.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    a.setStyleSheet(Stylesheets::appStyles());

    Playo w;
    w.show();

//    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
