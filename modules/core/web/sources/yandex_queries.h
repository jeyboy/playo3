#ifndef YANDEX_QUERIES
#define YANDEX_QUERIES

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"

#include "yandex/yandex_requests.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() {}
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_yandex; }

//                inline void objectInfoAsync(const QString & oid, Func * func) {
//                    ThreadUtils::obj().run((Requests *)this, &Requests::objectInfo, oid, func);
//                }

                inline void openSetAsync(const QString & cutomParams, Func * func) {
                    ThreadUtils::obj().run((Set *)this, &Set::openSet, Cmd::extractQuery(cutomParams), func);
                }

//                inline void trackRecommendationsAsync(const QString & track_id, Func * func) {
//                    ThreadUtils::obj().run((Requests *)this, &Requests::trackRecommendations, track_id, 0, SOUNDCLOUD_ITEMS_LIMIT, func);
//                }

//                inline void userRecommendationsAsync(const QString & user_id, Func * func) {
//                    ThreadUtils::obj().run((Requests *)this, &Requests::userRecommendationsAsync, user_id, 0, SOUNDCLOUD_ITEMS_LIMIT, func);
//                }
            };
        }
    }
}

#endif // YANDEX_QUERIES
