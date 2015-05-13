#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <QHash>
#include <QStringList>

class IGenres {
public:
    inline IGenres() {
    }

    inline virtual ~IGenres() {}

    int toInt(QString name) const;
    QString toString(int id) const;
    inline QStringList genresList() { return genres.keys(); }

    virtual int defaultInt() const = 0;
protected:

    QHash<QString, int> genres;
};

#endif // GENRES_INTERFACE
