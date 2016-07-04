#ifndef SOUNDCLOUD_SITE_SET
#define SOUNDCLOUD_SITE_SET

#include "defines.h"
#include "../interfaces/iset.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Set : public QueryBase, public ISet {
                public:
                    QJsonValue setByType(const SetType & setType, const QString & genre = QStringLiteral("all-music"));
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_SET
