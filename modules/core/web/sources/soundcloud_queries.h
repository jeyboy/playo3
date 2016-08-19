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

                QJsonValue objectInfo(const QString & oid) {
                    QJsonObject res;

                    if (oid[0] == '-')
                        groupInfo(oid.mid(1), res);
                    else
                        userInfo(oid, res);

                    return res;
                }
                inline void objectInfoAsync(const QString & oid, Func * func) {
                    ThreadUtils::obj().run(this, &Queries::objectInfo, oid, func);
                }

                inline void openSetAsync(const QString & cutomParams, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::openSet, Cmd::extractQuery(cutomParams), func);
                }

                inline void trackRelationsAsync(const QString & track_id, Func * func) {
                    ThreadUtils::obj().run((Track *)this, &Track::trackRecommendations, track_id, SOUNDCLOUD_ITEMS_LIMIT, 0, func);
                }


                void groupInfo(const QString & group_id, QJsonObject & object) {
                    object.insert(block_items, tracksByGroup(group_id));
                }

                void userInfo(const QString & user_id, QJsonObject & object) {
                    object.insert(block_items, tracksByUser(user_id));
                    object.insert(block_likes, tracksByUserLikes(user_id));
                    object.insert(block_sets, playlistsByUser(user_id));

                    if (user_id == userID()) { // ignore socials for not current user
                        clearFriends();
                        QThread::msleep(REQUEST_DELAY);
                        jsonToUsers(Friendable::linkables, userFollowings(user_id).toArray());
                        jsonToUsers(Friendable::linkables, userFollowers(user_id).toArray());
                        clearGroups();
                        QThread::msleep(REQUEST_DELAY);
                        jsonToGroups(Groupable::linkables, groupsByUser(user_id).toArray());
                    }
                }


                QString refresh(const QString & track_id, const DataMediaType & /*itemMediaType*/) {
                    return trackUrl(track_id);
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersById(uid).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name, 0, 400).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsById(uid).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name, 0, 400).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

            protected:
                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return playlistsByPredicate(predicate, limits.items_limit, limits.start_offset);
                }

                QJsonValue searchProc(const SearchLimit & limits) { return tracksSearch(limits); }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
