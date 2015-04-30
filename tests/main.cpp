#include <QCoreApplication>
#include <QTest>
//#include <iostream>
//#include <cstdlib>
//#include <cstdio>

#include "test_filename_conversions.h"

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    QTest::qExec(new TestFilenameConversions, argc, argv);

    return a.exec();
}
