#ifndef FILENAME_CONVERSIONS_H
#define FILENAME_CONVERSIONS_H

#ifdef Q_OS_WIN
    #define FILENAME_MAX_LEN 220
#else
    #define FILENAME_MAX_LEN 320
#endif

#include <qregexp.h>
#include <qstringbuilder.h>

namespace Core {
    class FilenameConversions {
    public:
        static inline bool extractExtension(QString & fileName, QString & sExt) {
            sExt = fileName.section('.', -1, -1);
            if (sExt != fileName && sExt.indexOf(' ') == -1) {
                fileName = fileName.section('.', 0, -2);
                return true;
            } else {
                sExt.clear();
                return false;
            }
        }

        static inline QString cacheTitleFilter(QString title)   		{ return title.remove(QRegExp(QStringLiteral("(\\W|[_])"))).toLower(); }
        static inline QString forwardNumberFilter(QString title)		{ return title.remove(QRegExp(QStringLiteral("\\A(\\[|\\()+\\d{1,}(\\]|\\))"))); }
        static QString downloadTitle(QString title, QString extension) {
          QString ret = title.replace(QRegExp(QStringLiteral("[^()\\w\\-&.]|_")), QStringLiteral(" "));

          if (!extension.isEmpty())
              ret = ret % QStringLiteral(".") % extension;

          return ret.replace(QRegExp(QStringLiteral("\\s+")), QStringLiteral(" ")).trimmed().mid(0, FILENAME_MAX_LEN);
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