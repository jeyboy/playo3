#include "test_filename_conversions.h"

void TestFilenameConversions::cacheTitleFilterTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("cacheTitleFilter_data_1") << QStringLiteral("") << QStringLiteral("");
    QTest::newRow("cacheTitleFilter_data_2") << QStringLiteral("") << QStringLiteral("");
}

void TestFilenameConversions::cacheTitleFilterTest() {
    QFETCH(QString, input); QFETCH(QString, result);
    QCOMPARE(cacheTitleFilter(input), result);
}


void TestFilenameConversions::forwardNumberFilterTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("cacheTitleFilter_data_1") << "" << 0;
    QTest::newRow("cacheTitleFilter_data_2") << -1 << 1;
}

void TestFilenameConversions::forwardNumberFilterTest() {
    QFETCH(QString, input); QFETCH(QString, result);
    QCOMPARE(forwardNumberFilter(input), result);
}

void TestFilenameConversions::downloadTitleTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("ext");
    QTest::addColumn<QString>("result");

    QTest::newRow("cacheTitleFilter_data_1") << "" << 0;
    QTest::newRow("cacheTitleFilter_data_2") << -1 << 1;
}

void TestFilenameConversions::downloadTitleTest() {
    QFETCH(QString, input); QFETCH(QString, ext); QFETCH(QString, result);
    QCOMPARE(downloadTitle(input, ext), result);
}
