#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <QHash>
#include <QString>

class IGenres {
public:
    inline IGenres() {
    }

    inline virtual ~IGenres() {}

    int toInt(QString name) const;

protected:
    virtual int defaultInt() const = 0;

    QHash<int, QString> genres;
};

#endif // GENRES_INTERFACE
