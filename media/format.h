#ifndef FORMAT_H
#define FORMAT_H

#include <QString>

class Format {
public:
    static QString toTime(int h, int m, int s, bool forciblyIncludeHours = false);
    static inline QString toInfo(QString size, QString ext) { return size + " :: " + ext.toLower(); }
    static QString toInfo(QString size, QString ext, int bitrate, int freq, int channelsCount);
    static QString toUnits(long long val);
};


#endif // FORMAT_H
