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
    virtual int defaultInt() const = 0;
protected:

    QHash<QString, int> genres;
};

#endif // GENRES_INTERFACE
