#include "unicode_decoding.h"

using namespace Core::Web;

QHash<QString, QChar> UnicodeDecoding::html_entities = {
    { QLatin1String("nbsp"), ' ' },
    { QLatin1String("amp"), '&' },
    { QLatin1String("lt"), '<' },
    { QLatin1String("gt"), '>' },
    { QLatin1String("quot"), '"' }
};
