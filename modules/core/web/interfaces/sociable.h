#ifndef SOCIABLE
#define SOCIABLE

#include <qstringbuilder.h>

#include "modules/core/web/interfaces/friendable.h"
#include "modules/core/web/interfaces/groupable.h"

namespace Core {
    namespace Web {
        class Sociable : public Friendable, public Groupable {
        public:
            void fromJson(const QJsonObject & hash) {
                Friendable::fromJson(hash);
                Groupable::fromJson(hash);
            }
            void toJson(QJsonObject & hash) {
                Friendable::toJson(hash);
                Groupable::toJson(hash);
            }
        };
    }
}

#endif // SOCIABLE
