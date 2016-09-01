#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "modules/core/interfaces/singleton.h"
#include "soundcloud/soundcloud_requests.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_sc; }

                inline void objectInfoAsync(const QString & oid, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::objectInfo, oid, func);
                }

                inline void openSetAsync(const QString & cutomParams, Func * func) {
                    ThreadUtils::obj().run((Set *)this, &Set::openSet, Cmd::extractQuery(cutomParams), func);
                }

                inline void trackRecommendationsAsync(const QString & track_id, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::trackRecommendations, track_id, 0, SOUNDCLOUD_ITEMS_LIMIT, func);
                }
            protected:
                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return QJsonArray() << playlistsByPredicate(predicate, limits.items_limit, limits.start_offset);
                }

                QJsonValue searchProc(const SearchLimit & limits) { return QJsonArray() << tracksSearch(limits); }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
