#ifndef SOUNDCLOUD_ISET
#define SOUNDCLOUD_ISET

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class ISet {
            protected:
                enum SetType { new_hot = 1, top_50, popular };

                QJsonValue byType(const SetType & chartType, const QString & genre = QStringLiteral("all-music"));
            };
        }
    }
}

#endif // SOUNDCLOUD_ISET
