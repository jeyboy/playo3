#include "unicode_decoding.h"

using namespace Core::Web;

QHash<QString, QChar> UnicodeDecoding::html_entities = {
    { QStringLiteral("nbsp"), ' ' },
    { QStringLiteral("amp"), '&' },
    { QStringLiteral("lt"), '<' },
    { QStringLiteral("gt"), '>' },
    { QStringLiteral("quot"), '"' }
};
