#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>

#include "modules/core/data_sub_types.h"

#define UID_HEAD QStringLiteral("@")

namespace Core {
    class ISource {
    public:
        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline virtual bool isConnected() { return true; }
//        virtual inline bool connectable() { return false; }

        virtual inline bool connect_user() { return false; }
        virtual inline void disconnect_user() { }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }
    };
}

#endif // ISOURCE
