#include "test_filename_conversions.h"

using namespace Core;

// (muzimir.ru)_301. Seismicbeats - Raindrops (Original Mix) - 3lime.mp3
// 29. Frontliner – Symbols.mp3
// blue_manta-see_the_light_(van_gelder_mix).mp3
// Sarah Connor - From Sarah With Love (Kayrob mix) (vmuzike.net).mp3
// (02) [Metallica] Master Of Puppets.flac
// 02 - Remember The Name.flac
// 02-a_beautiful_lie.flac

void TestFilenameConversions::extractExtension_data() {
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("res_filename");
    QTest::addColumn<QString>("res_extension");

    QTest::newRow("has and extract extension")
            << QStringLiteral("gungirl2.ogg") << QStringLiteral("gungirl2") << QStringLiteral("ogg");
    QTest::newRow("did not has extension")
            << QStringLiteral("gungirl2") << QStringLiteral("gungirl2") << QStringLiteral("");
    QTest::newRow("did not has extension? but has dot")
            << QStringLiteral("gungirl2.") << QStringLiteral("gungirl2") << QStringLiteral("");
    QTest::newRow("has extra dots in name")
            << QStringLiteral("gung.irl2.ogg") << QStringLiteral("gung.irl2") << QStringLiteral("ogg");
}

void TestFilenameConversions::extractExtension() {
    QFETCH(QString, filename);
    QFETCH(QString, res_filename);
    QFETCH(QString, res_extension);
    QString extension;

    Extensions::extractExtension(filename, extension);

    QCOMPARE(filename, res_filename);
    QCOMPARE(extension, res_extension);
}

///////////////////////////////////////////////////////////////////////////////

void TestFilenameConversions::cacheTitleFilterTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("remove extra symbols 1")
            << QStringLiteral("Gun()gir-l2") << QStringLiteral("gungirl2");
    QTest::newRow("remove extra symbols 2")
            << QStringLiteral("Gungirl(2)") << QStringLiteral("gungirl2");
}

void TestFilenameConversions::cacheTitleFilterTest() {
    QFETCH(QString, input); QFETCH(QString, result);
    QCOMPARE(FilenameConversions::cacheTitleFilter(input), result);
}

///////////////////////////////////////////////////////////////////////////////

void TestFilenameConversions::forwardNumberFilterTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("result");

    QTest::newRow("forward number in [ brackets")
            << QStringLiteral("[2]gungirl2") << QStringLiteral("gungirl2");
    QTest::newRow("forward number in ( brackets")
            << QStringLiteral("(2)gungirl2") << QStringLiteral("gungirl2");
    QTest::newRow("skip forward number without brackets")
            << QStringLiteral("2 gungirl2") << QStringLiteral("2 gungirl2");
}

void TestFilenameConversions::forwardNumberFilterTest() {
    QFETCH(QString, input); QFETCH(QString, result);
    QCOMPARE(FilenameConversions::forwardNumberFilter(input), result);
}

///////////////////////////////////////////////////////////////////////////////

void TestFilenameConversions::downloadTitleTest_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("ext");
    QTest::addColumn<QString>("result");

    QTest::newRow("clean download title")
            << QStringLiteral("(02) [Metallica]   Master Of  ***Puppets") << QStringLiteral("flac") << QStringLiteral("(02) Metallica Master Of Puppets.flac");
}

void TestFilenameConversions::downloadTitleTest() {
    QFETCH(QString, input); QFETCH(QString, ext); QFETCH(QString, result);
    QCOMPARE(FilenameConversions::downloadTitle(input, ext), result);
}
