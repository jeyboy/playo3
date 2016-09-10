#ifndef YANDEX_QUERIES
#define YANDEX_QUERIES

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"

#include "yandex/yandex_requests.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Api : public Requests, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() {}
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_yandex; }

//                void objectInfo(const QString & uid, Func * func) {
//                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
//                }
            };
        }
    }
}

#endif // YANDEX_QUERIES
