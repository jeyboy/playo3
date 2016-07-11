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
                inline DataSubType siteType() const { return dt_site_vk; }

                void userInfoAsync(const QString & uid, const InfoType & info_type, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::userInfo, uid, info_type, func);
                }

                void trackRecommendationsAsync(const QString & uid, bool byUser, bool randomize, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::trackRecommendations, uid, byUser, randomize, func);
                }

                QString refresh(const QString & audio_uid, const DataMediaType & /*itemMediaType*/) { // TODO: update for video refresh
                    return refresh_postproc(trackUrl(audio_uid));
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByIdOrPerma(uid);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByIdOrPerma(uid);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
            };
        }
    }
}

#endif // VK_QUERIES_H
