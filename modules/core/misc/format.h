#ifndef FORMAT_H
#define FORMAT_H

#include <qstringbuilder.h>
#include <qstringlist.h>

class Info {
    static QString unitList[];
public:
    static bool isNumber(const QString & str);
    static bool extractNumber(const QString & info, QString & res, int index = 0);
    static QString extractLimitedBy(const QString & info, const QString & before_predicate, const QString & after_predicate);
    static bool extract(const QString & info, const QString & start_predicate, const QString & end_predicate, QString & res);
    static inline QString str(const QString & size, const QString & ext) { return size % " :: " % ext.toLower(); }
    static QString str(const QString & size, const QString & ext, const QString & bitrate, const QString & freq, const QString & channelsCount);
    static QString str(const QString & size, const QString & ext, int bitrate, int freq, int channelsCount);
    static QString toUnits(long long val);
    static long long fromUnits(const QString & val);

    static QString paddedNumber(qint64 time);
};

class Duration {
public:
    static QString fromHMS(int h, int m, int s, bool forciblyIncludeHours);
    static QString fromSeconds(int seconds, bool includeHours = false);
    static QString fromMillis(qint64 millis, bool includeHours = false);

    static bool hasHours(qint64 millis);
    static qint64 ISO8601StrtoMillis(const QString & str);
    static qint64 toSecs(const QString & str);
    static qint64 toMillis(const QString & str);
};

#endif // FORMAT_H
