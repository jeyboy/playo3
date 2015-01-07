#ifndef FORMAT_H
#define FORMAT_H

#include <QString>

class Format {
public:
    static QString toTime(int h, int m, int s, bool forciblyIncludeHours = false);
    static QString toInfo(QString size, int bitrate, int freq, int channelsCount);
    static QString toUnits(long long val);
};


#endif // FORMAT_H
