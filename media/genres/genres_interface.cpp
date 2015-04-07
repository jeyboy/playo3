#include "genres_interface.h"

int IGenres::toInt(QLatin1String name) const {
    return genres.value(name, defaultInt());
}
