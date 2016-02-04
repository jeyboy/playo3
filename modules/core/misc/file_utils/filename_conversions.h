#ifndef FILENAME_CONVERSIONS_H
#define FILENAME_CONVERSIONS_H

#ifdef Q_OS_WIN
    #define FILENAME_MAX_LEN 220
#else
    #define FILENAME_MAX_LEN 320
#endif

#include <qregexp.h>
#include <qstringbuilder.h>
#include <qvector.h>

namespace Core {
    class FilenameConversions {
    public:
        static inline QString takePath(const QString & filePath) {
            QStringList parts = filePath.split('/', QString::SkipEmptyParts);
            parts.removeLast();
            return parts.join('/');
        }

        static inline void splitPath(QString & filePath, QString & fileName) {
            QStringList parts = filePath.split('/', QString::SkipEmptyParts);
            fileName = parts.takeLast();
            filePath = parts.join('/');
        }

        //[\\s\\n\\r\\t]+
        //[^\\S\\w-]+
        static inline QString extraSymbolsFilter(QString title)   		{ return title.replace(QRegExp(QStringLiteral("[\\s]+")), QStringLiteral(" ")); }

        static inline QString cacheTitleFilter(QString title)   		{ return title.remove(QRegExp(QStringLiteral("(\\W|[_])"))).toLower(); }
        static inline QString forwardNumberFilter(QString title)		{ return title.remove(QRegExp(QStringLiteral("^[\\[\\(]+\\d{1,}[\\]\\)]+"))); }
        static QString downloadTitle(QString title, QString extension) {
            QString ret = title.replace(QRegExp(QStringLiteral("[^()\\w\\-&.]|_")), QStringLiteral(" "));
            ret = ret.replace(QRegExp(QStringLiteral("\\s+")), QStringLiteral(" ")).trimmed().mid(0, FILENAME_MAX_LEN);

            if (!extension.isEmpty())
                ret = ret % QStringLiteral(".") % extension;

            return ret;
        }

        //static inline QString sitesFilter(QString title)				{ return title.remove(QRegExp("((http:\\/\\/)?(www\\.)?[\\w-]+\\.(\\w+)(\\.(\\w+))*)")); }
        //static inline QString forwardNumberPreFilter(QString title)     { return title.remove(QRegExp("\\A\\d{1,}.|\\(\\w*\\d{1,}\\w*\\)")); }
        //static inline QString spacesFilter(QString title)               { return title.remove(QRegExp("(\\W|[_])")); }
        //static inline QString forwardNumberFilter(QString title)		{ return title.remove(QRegExp("\\A\\d{1,}")); }
        //static inline QString filenameFilter(QString title)             { return
        //            forwardNumberPreFilter(sitesFilter(title)).remove(QRegExp("[^\\w-\\. ()]*")).mid(0, PATH_MAX_LEN);
        //}
        //static QString downloadTitle(QString title, QString extension) {
        //  QString ret = filenameFilter(title);

        //  if (!extension.isEmpty())
        //      ret = ret + '.' + extension;

        //  ret.replace(QRegExp("\\s+"), " ").trimmed(); // TODO: remove empty brackets
        //  return ret;
        //}
    };
}

#endif // FILENAME_CONVERSIONS_H
