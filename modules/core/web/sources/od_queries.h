#ifndef OD_QUERIES_H
#define OD_QUERIES_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"

#include "od/od_requests.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_od; }

                void userInfoAsync(const QString & uid, Func * func) {
                    if (uid == userID())
                        ThreadUtils::obj().run((Requests *)this, &Requests::userInfo, func);
                    else
                        ThreadUtils::obj().run((User *)this, &User::userMedia, uid, func);
                }
            };
        }
    }
}

#endif // OD_QUERIES_H
