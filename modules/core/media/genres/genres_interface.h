#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <qhash.h>
#include <qstringlist.h>
#include <qregularexpression.h>

namespace Core {
    namespace Media {
        class IGenres {
        public:
            inline IGenres() {}
            inline virtual ~IGenres() {}

            int toInt(QString name) const;
            virtual QString toString(int id) const;
            inline QStringList genresList() { return genres.keys(); }

            virtual int defaultInt() const { return -1; }
        protected:
            inline QString & prepare(QString & name) { return (name = name.replace(QRegularExpression("(\\W|[_])"), QLatin1String(" ")).toLower()); } // check & on replace

            QHash<QString, int> genres;
        };
    }
}

#endif // GENRES_INTERFACE
