#ifndef DURATION_H
#define DURATION_H

#include <qstringbuilder.h>
#include <qstringlist.h>

//QTime(0, 0, 0).addMSecs(mpPlayer->duration()).toString("HH:mm:ss")

class Duration {
public:
    static QString fromHMS(int h, int m, int s, bool forciblyIncludeHours);
    static QString fromSeconds(int seconds, bool includeHours = false);
    static QString fromMillis(int millis, bool includeHours = false);

    static bool hasHours(int millis);
    static int toSeconds(QString str);
};

#endif // DURATION_H
