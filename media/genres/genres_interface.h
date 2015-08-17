#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <qhash.h>
#include <qstringlist.h>

class IGenres {
public:
    inline IGenres() {}
    inline virtual ~IGenres() {}

    int toInt(QString name) const;
    virtual QString toString(int id) const;
    inline QStringList genresList() { return genres.keys(); }

    virtual int defaultInt() const { return -1; }
protected:
    inline QString & prepare(QString & name) { return (name = name.replace(QRegularExpression(QStringLiteral("(\\W|[_])")), QString()).toLower()); }

    QHash<QString, int> genres;
};

#endif // GENRES_INTERFACE
