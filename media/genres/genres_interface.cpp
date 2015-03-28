#include "genres_interface.h"

int IGenres::toInt(QString name) const {
    return genres.key(name, defaultInt());
}
