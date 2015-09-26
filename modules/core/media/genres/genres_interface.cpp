#include "genres_interface.h"

using namespace Core::Media;

int IGenres::toInt(QString name) const {
    return genres.value(name, defaultInt());
}

QString IGenres::toString(int id) const {
    return genres.key(id, QStringLiteral("Other"));
}
