#include "duration.h"

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
