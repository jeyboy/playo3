#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>

#include "modules/core/data_sub_types.h"

namespace Core {
    class ISource {
        virtual QString name() const = 0;
        virtual QString uidStr(const QString & tabId) const = 0;
        virtual DataSubType siteType() = 0;
    };
}

#endif // ISOURCE
