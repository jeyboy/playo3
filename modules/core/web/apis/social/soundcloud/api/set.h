#ifndef SOUNDCLOUD_API_SET
#define SOUNDCLOUD_API_SET

#include "defines.h"
#include "../interfaces/iset.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Set : public QueryBase, public ISet {
                public:
                    QJsonValue setByType(const SetType & setType, const QString & genre = QString());
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_SET
