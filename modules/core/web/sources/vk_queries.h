#ifndef VK_QUERIES_H
#define VK_QUERIES_H

// http://vk.com/support

#include "vk/vk_requests.h"
#include "modules/core/interfaces/singleton.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT
                friend class Singleton<Queries>;
                inline Queries() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_vk; }

                void userInfoAsync(const QString & user_id, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::userInfo, user_id, func);
                }

                void trackRecommendationsAsync(const QString & uid, Func * func) {
                   ThreadUtils::obj().run((Requests *)this, &Requests::trackRecommendations, uid, true, func);
                }

                void userRecommendationsAsync(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::userRecommendations, uid, true, func);
                }

                inline void openSetAsync(const QString & cutomParams, Func * func) {
                    ThreadUtils::obj().run((Set *)this, &Set::openSet, Cmd::extractQuery(cutomParams), func);
                }
            };
        }
    }
}

#endif // VK_QUERIES_H
