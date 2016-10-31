#ifndef YOUTUBE_QUERIES
#define YOUTUBE_QUERIES

#include "youtube/youtube_requests.h"
#include "modules/core/interfaces/singleton.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_youtube; }

                void userInfoAsync(const QString & user_id, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::userInfo, user_id, func);
                }
            };
        }
    }
}

#endif // YOUTUBE_QUERIES
