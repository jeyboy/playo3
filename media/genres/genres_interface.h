#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <QHash>
#include <QString>

class IGenres {
public:
    inline IGenres() {
    }

    inline virtual ~IGenres() {}

    int toInt(QLatin1String name) const;

protected:
    virtual int defaultInt() const = 0;

    QHash<QLatin1String, int> genres;
};

#endif // GENRES_INTERFACE
