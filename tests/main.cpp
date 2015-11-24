#include <QCoreApplication>
#include <QTest>
//#include <iostream>
//#include <cstdlib>
//#include <cstdio>

#include "test_filename_conversions.h"

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    QTest::qExec(new Core::TestFilenameConversions, argc, argv);

    return a.exec();
}


//QLineEdit a;
//QTest::keyClicks(&a, "abCDEf123-");

//QCOMPARE(a.text(), QString("abCDEf123-"));
//QVERIFY(a.isModified());

//void Test_Smart::min_data()
//{
//    QTest::addColumn<int>("first");
//    QTest::addColumn<int>("second");
//    QTest::addColumn<int>("result");

//    QTest::newRow("min_data_1") << 1 << 0 << 0;
//    QTest::newRow("min_data_2") << -1 << 1 << -1;
//    QTest::newRow("min_data_3") << 4 << 8 << 4;
//    QTest::newRow("min_data_4") << 0 << 0 << 0;
//    QTest::newRow("min_data_5") << 1 << 1 << 1;
//    QTest::newRow("min_data_6") << -10 << -5 << -10;
//}

//void Test_Smart::min()
//{
//    Smart a;
//    QFETCH(int, first);
//    QFETCH(int, second);
//    QFETCH(int, result);
//    QCOMPARE(a.min(first, second), result);
//}
