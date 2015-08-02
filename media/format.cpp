#include "format.h"

QString Format::unitList[] = {QStringLiteral("B"), QStringLiteral("Kb"), QStringLiteral("Mb"), QStringLiteral("Gb"), QStringLiteral("Tb"), QStringLiteral("Pb")};

QString Format::toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount) {
    return toInfo(size, ext) % " :: " % QString::number(channelsCount) % " ch :: " % QString::number(bitrate) % " kbps :: " % QString::number(freq) % " kHz";
}

QString Format::toUnits(long long val) {
    int size = sizeof(unitList) / sizeof(unitList[0]);

    int rel = 0;
    double dVal = val;

    for (; rel < size && dVal > 1023; rel++)
        dVal /= 1024.0;

    return QString().sprintf("%.2f ", dVal) + unitList[rel];
}

long long Format::fromUnits(QString val) {
    int size = sizeof(unitList) / sizeof(unitList[0]);
    QStringList rel = val.split(' ', QString::SkipEmptyParts);

    for(int i = 0; i < size; i++)
        if (unitList[i] == rel.last())
            return rel.first().toFloat() * (1024 * i);

    return 0;
}
