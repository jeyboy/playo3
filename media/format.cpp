#include "format.h"

QString Format::toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount) {
    return toInfo(size, ext) % " :: " % QString::number(channelsCount) % " ch :: " % QString::number(bitrate) % " kbps :: " % QString::number(freq) % " kHz";
}

QString Format::toUnits(long long val) {
    QString unitList[] = {QStringLiteral("B"), QStringLiteral("Kb"), QStringLiteral("Mb"), QStringLiteral("Gb"), QStringLiteral("Tb"), QStringLiteral("Pb")};
    int size = sizeof(unitList) / sizeof(unitList[0]);

    int loop1 = 0;
    double dVal = val;

    for (; loop1 < size && dVal > 1023; loop1++)
        dVal /= 1024.0;

    return QString().sprintf("%.2f ", dVal) + unitList[loop1];
}
