#ifndef FILENAME_CONVERSIONS_H
#define FILENAME_CONVERSIONS_H

//TODO: need limit relative to the current system
#define PATH_MAX_LEN 200

#include <QObject>

static inline QString sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(\\w+)(\\.(\\w+))*)")); }
static inline QString forwardNumberPreFilter(QString title)     { return title.remove(QRegExp("\\A\\d{1,}.|\\(\\w*\\d{1,}\\w*\\)")); }
static inline QString spacesFilter(QString title)               { return title.remove(QRegExp("(\\W|[_])")); }
static inline QString forwardNumberFilter(QString title)		{ return title.remove(QRegExp("\\A\\d{1,}")); }
static inline QString filenameFilter(QString title)             { return
            forwardNumberPreFilter(sitesFilter(title)).remove(QRegExp("[^\\w-\\. ()]*")).mid(0, PATH_MAX_LEN);
}

#endif // FILENAME_CONVERSIONS_H
