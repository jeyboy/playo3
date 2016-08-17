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
                inline DataSubType sourceType() const { return dt_site_vk; }

                void userInfoAsync(const QString & user_id, Func * func) {
                    if (user_id == userID())
                        ThreadUtils::obj().run((Requests *)this, &Requests::userInfo, user_id, func);
                    else
                        ThreadUtils::obj().run((Requests *)this, &Requests::userMedia, user_id, func);
                }

                void trackRecommendationsAsync(const QString & uid, bool by_user, bool randomize, Func * func) {
                    if (by_user)
                        ThreadUtils::obj().run((Track *)this, &Track::userRecommendations, uid, randomize, func);
                    else
                        ThreadUtils::obj().run((Track *)this, &Track::trackRecommendations, uid, randomize, func);
                }

                QString refresh(const QString & item_uid, const DataMediaType & itemMediaType) {
                    switch(itemMediaType) {
                        case dmt_audio: return cleanUrl(trackUrl(item_uid));
                        case dmt_video: return videoUrl(item_uid);
                        default:;
                    }

                    return QString();
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByIdOrPerma(uid).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByIdOrPerma(uid).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
            };
        }
    }
}

#endif // VK_QUERIES_H
