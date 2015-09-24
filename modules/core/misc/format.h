#ifndef FORMAT_H
#define FORMAT_H

#include <qstringbuilder.h>
#include <qstringlist.h>

class Info {
    static QString unitList[];
public:
    static inline QString toInfo(QString size, QString ext) { return size % " :: " % ext.toLower(); }
    static QString toInfo(QString size, QString ext, QString bitrate, QString freq, QString channelsCount);
    static QString toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount);
    static QString toUnits(long long val);
    static long long fromUnits(QString val);

    static QString paddedNumber(qint64 time);
};

class Duration {
public:
    static QString fromHMS(int h, int m, int s, bool forciblyIncludeHours);
    static QString fromSeconds(int seconds, bool includeHours = false);
    static QString fromMillis(int millis, bool includeHours = false);

    static bool hasHours(int millis);
    static int toSeconds(QString str);
};

#endif // FORMAT_H
