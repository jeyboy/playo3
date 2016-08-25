#include "format.h"
#include <qdebug.h>
#include <qregularexpression.h>

QString Info::unitList[] = {QStringLiteral("B"), QStringLiteral("Kb"), QStringLiteral("Mb"), QStringLiteral("Gb"), QStringLiteral("Tb"), QStringLiteral("Pb")};

bool Info::isNumber(const QString & str) {
    QRegularExpression r("\\D+");
    return !r.match(str).hasMatch();
}

bool Info::extractNumber(const QString & info, QString & res, int index) {
    QStringList digits = info.split(QRegularExpression("\\D+"), QString::SkipEmptyParts);

    if (digits.length() < index) return false;

    res = digits[index];
    return true;
}

QString Info::extractLimitedBy(const QString & info, const QString & before_predicate, const QString & after_predicate) {
    return info.section(before_predicate, 1).section(after_predicate, 0, 0);
}

bool Info::extract(const QString & info, const QString & start_predicate, const QString & end_predicate, QString & res) {
    int part_index = info.indexOf(start_predicate);

    if (part_index != -1) {
        int end_part_index = info.indexOf(end_predicate, part_index);
        res = info.mid(part_index, end_part_index - part_index);
        return true;
    }
    else return false;
}

QString Info::str(const QString & size, const QString & ext, const QString & bitrate, const QString & freq, const QString & channelsCount) {
    return str(size, ext) % " :: " % channelsCount % " ch :: " % bitrate % " kbps :: " % freq % " kHz";
}

QString Info::str(const QString & size, const QString & ext, int bitrate, int freq, int channelsCount) {
    return str(size, ext) % " :: " % QString::number(channelsCount) % " ch :: " % QString::number(bitrate) % " kbps :: " % QString::number(freq) % " kHz";
}

QString Info::toUnits(long long val) {
    if (val == 0) return QStringLiteral("*");

    int size = sizeof(unitList) / sizeof(unitList[0]);

    int rel = 0;
    double dVal = val;

    for (; rel < size && dVal > 1023; rel++)
        dVal /= 1024.0;

    return QString().sprintf("%.2f ", dVal) + unitList[rel];
}

long long Info::fromUnits(const QString & val) {
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
QString Duration::fromMillis(qint64 millis, bool includeHours) {
    int m = millis == 0 ? 0 : abs(millis / 60000) % 60;
    int s = millis == 0 ? 0 : abs(millis / 1000) % 60;
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;

    return fromHMS(h, m, s, includeHours);
}

bool Duration::hasHours(qint64 millis) {
    int h = millis == 0 ? 0 : abs(millis / 3600000) % 24;
    return h > 0;
}

// PT3M48S // youtube
qint64 Duration::ISO8601StrtoMillis(const QString & str) {
    qint64 summ = 0;
    int pos = 2, offset = 2, mult = 1;
    for(QString::ConstIterator ch = str.cbegin() + 2; ch != str.cend(); ch++, offset++) {
        switch((*ch).unicode()) {
            case 87: {
                mult = 604800;
                summ += mult * str.midRef(pos, offset - pos).toInt() * 1000;
                pos = offset + 1;
            break;}// w
            case 68: {
                mult = 86400;
                summ += mult * str.midRef(pos, offset - pos).toInt() * 1000;
                pos = offset + 1;
            break;}// d
            case 72: {
                mult = 3600;
                summ += mult * str.midRef(pos, offset - pos).toInt() * 1000;
                pos = offset + 1;
            break;}// h
            case 77: {
                mult = 60;
                summ += mult * str.midRef(pos, offset - pos).toInt() * 1000;
                pos = offset + 1;
            break;}// m
            case 83: {
                mult = 1;
                summ += mult * str.midRef(pos, offset - pos).toInt() * 1000;
                pos = offset + 1;
            break;}// s
            default:;
        }
    }

    return summ;
}

qint64 Duration::toSecs(const QString & str) {
    QStringList parts = str.split(':', QString::SkipEmptyParts);

    int res = 0;

    if (parts.size() > 2)
        res += parts.takeFirst().toInt() * 3600;

    if (parts.size() > 1)
        res += parts.takeFirst().toInt() * 60;

    if (parts.size() > 0)
        res += parts.takeFirst().toInt();

    return res;
}

qint64 Duration::toMillis(const QString & str) {
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
