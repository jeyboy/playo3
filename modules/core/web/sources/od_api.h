#ifndef OD_API_H
#define OD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"

#include "od/od_requests.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public Requests, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_od; }

                void objectInfo(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((User *)this, &User::userInfo, uid, func);
                }

                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType); // here refresh_page must by eq to track id
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
