#include "format.h"

QString Info::unitList[] = {QStringLiteral("B"), QStringLiteral("Kb"), QStringLiteral("Mb"), QStringLiteral("Gb"), QStringLiteral("Tb"), QStringLiteral("Pb")};

QString Info::toInfo(QString size, QString ext, QString bitrate, QString freq, QString channelsCount) {
    return toInfo(size, ext) % " :: " % channelsCount % " ch :: " % bitrate % " kbps :: " % freq % " kHz";
}

QString Info::toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount) {
    return toInfo(size, ext) % " :: " % QString::number(channelsCount) % " ch :: " % QString::number(bitrate) % " kbps :: " % QString::number(freq) % " kHz";
}

QString Info::toUnits(long long val) {
    int size = sizeof(unitList) / sizeof(unitList[0]);

    int rel = 0;
    double dVal = val;

    for (; rel < size && dVal > 1023; rel++)
        dVal /= 1024.0;

    return QString().sprintf("%.2f ", dVal) + unitList[rel];
}

long long Info::fromUnits(QString val) {
    int size = sizeof(unitList) / sizeof(unitList[0]);
    QStringList rel = val.split(' ', QString::SkipEmptyParts);

    for(int i = 0; i < size; i++)
        if (unitList[i] == rel.last())
            return rel.first().toFloat() * (1024 * i);

    return 0;
}

QString Info::paddedNumber(qint64 time) {
    QString res = QString::number(time);
    int len = res.length();

    if (len > 3)
        for (int pos = len - 3; pos > 0; pos -= 3)
            res = res.insert(pos, ' ');

    return res;
}

QString Duration::fromHMS(int h, int m, int s, bool forciblyIncludeHours) {
    if (h > 0 || forciblyIncludeHours)
        return QString().sprintf("%02d:%02d:%02d", h, m, s);
    else
        return QString().sprintf("%02d:%02d", m, s);
}

QString Duration::fromSeconds(int seconds, bool includeHours) {
    int m = seconds == 0 ? 0 : abs(seconds / 60) % 60;
    int s = seconds == 0 ? 0 : abs(seconds) % 60;
    int h = seconds == 0 ? 0 : abs(seconds / 3600) % 24;

    return fromHMS(h, m, s, includeHours);
}
QString Duration::fromMillis(int millis, bool includeHours) {
    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;

    return fromHMS(h, m, s, includeHours);
}

bool Duration::hasHours(int millis) {
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
    return h > 0;
}

int Duration::toSeconds(QString str) {
    QStringList parts = str.split(':', QString::SkipEmptyParts);

    int res = 0;

    if (parts.size() > 2)
        res += parts.takeFirst().toInt() * 3600000;

    if (parts.size() > 1)
        res += parts.takeFirst().toInt() * 60000;

    if (parts.size() > 0)
        res += parts.takeFirst().toInt() * 1000;

    return res;
}
