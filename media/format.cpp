#include "format.h"

QString Format::toTime(int h, int m, int s, bool forciblyIncludeHours) {
    if (h > 0 || forciblyIncludeHours) {
        return QString().sprintf("%02d:%02d:%02d", h, m, s);
    } else {
        return QString().sprintf("%02d:%02d", m, s);
    }
}

QString Format::toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount) {
    return toInfo(size, ext) % " :: " % QString::number(channelsCount) % " ch :: " % QString::number(bitrate) % " kbps :: " % QString::number(freq) % " kHz";
}

QString Format::toUnits(long long val) {
    QString unitList[] = {"B", "Kb", "Mb", "Gb", "Tb", "Pb"};
    int size = sizeof(unitList) / sizeof(unitList[0]);

    int loop1 = 0;
    double dVal = val;

    for (; loop1 < size && dVal > 1023; loop1++)
        dVal /= 1024.0;

    return QString().sprintf("%.2f ", dVal) + unitList[loop1];
}
