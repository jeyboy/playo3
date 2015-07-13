#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <qhash.h>
#include <qstringlist.h>

class IGenres {
public:
    inline IGenres() {}
    inline virtual ~IGenres() {}

    int toInt(QString name) const;
    QString toString(int id) const;
    inline QStringList genresList() { return genres.keys(); }

    virtual int defaultInt() const { return 0; }
protected:
    QHash<QString, int> genres;
};

#endif // GENRES_INTERFACE
