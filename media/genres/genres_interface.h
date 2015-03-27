#ifndef GENRES_INTERFACE
#define GENRES_INTERFACE

#include <QHash>
#include <QString>

class IGenres {
public:
    inline IGenres() : initDefault() {}

    int toInt(QString name) const;

private:
    virtual int defaultInt() const = 0;
    virtual void initDefault() = 0;

    QHash<int, QString> genres;
};

#endif // GENRES_INTERFACE
