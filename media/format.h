#ifndef FORMAT_H
#define FORMAT_H

#include <qstringbuilder.h>
#include <qstringlist.h>

class Format {
    static QString unitList[];
public:
    static inline QString toInfo(QString size, QString ext) { return size % " :: " % ext.toLower(); }
    static QString toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount);
    static QString toUnits(long long val);
    static long long fromUnits(QString val);
};


#endif // FORMAT_H
